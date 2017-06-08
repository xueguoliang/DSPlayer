#include "DSPlayer.h"

DSPlayer::DSPlayer(QObject *parent) : QThread(parent)
{
}

DSPlayer::~DSPlayer()
{

}

void DSPlayer::run()
{

    play();
}

void DSPlayer::initSwr(double speed)
{
    if(speed < 0.5) speed = 0.5;
    if(speed > 4) speed = 4;

    if(swr) swr_free(&swr);
    swr = swr_alloc_set_opts(NULL, aCtx->channel_layout,
                             AV_SAMPLE_FMT_S16,
                             format.sampleRate()/speed,
                             aCtx->channel_layout,
                             aCtx->sample_fmt,
                             aCtx->sample_rate,
                             0, NULL);
    swr_init(swr);

}

int DSPlayer::play()
{
    int ret = avformat_open_input(&ic, this->strFilename.toLocal8Bit().data(), NULL, NULL);

    reader.pause = false;
    reader.ic = ic;
    reader.buffer = 100;

    if(this->isNetworkFile)
        reader.buffer = 2000;

    reader.start();

    if(ret < 0)
    {
        qDebug() << "open input error" << ret;
        return 0;
    }

    if(avformat_find_stream_info(ic, NULL) != 0)
    {
        qDebug() << "find stream info error";
        return 0;
    }

    reader.ic = ic;
    aIndex = 0;
    vIndex = 0;
    AVCodecContext* vCtx;

    for(int i=0; i<ic->nb_streams; ++i)
    {
        if(ic->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO)
        {
            aCtx = ic->streams[i]->codec;
            aIndex = i;

            AVCodec* codec = avcodec_find_decoder(aCtx->codec_id);
            if(codec == NULL)
            {
                qDebug() << "error find codec" << aCtx->codec_id;
                return 0;
            }

            ret = avcodec_open2(aCtx, codec, NULL);
            if(ret != 0)
            {
                qDebug() << "error open codec";
                return 0;
            }

        }
        else if(ic->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            vCtx = ic->streams[i]->codec;
            vIndex = i;

            AVCodec* codec = avcodec_find_decoder(vCtx->codec_id);
            if(codec == NULL)
            {
                qDebug() << "error find codec" << vCtx->codec_id;
                return 0;
            }

            ret = avcodec_open2(vCtx, codec, NULL);
            if(ret != 0)
            {
                qDebug() << "error open codec";
                return 0;
            }
        }
    }

    qDebug() << "open codec success";


    format.setSampleRate(44100);
    format.setChannelCount(2);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    QAudioDeviceInfo info = QAudioDeviceInfo::defaultOutputDevice();
    if(!info.isFormatSupported(format))
        format = info.nearestFormat(format);


    output = new QAudioOutput(format);

    initSwr(1);
    SwsContext* sws = sws_getCachedContext(NULL,
                                           ic->streams[vIndex]->codec->width,
                                           ic->streams[vIndex]->codec->height,
                                           ic->streams[vIndex]->codec->pix_fmt,
                                           ic->streams[vIndex]->codec->width,
                                           ic->streams[vIndex]->codec->height,
                                           AV_PIX_FMT_RGBA,
                                           SWS_BICUBIC, NULL, NULL, NULL);

    QImage image = QImage(ic->streams[vIndex]->codec->width,
                          ic->streams[vIndex]->codec->height,
                          QImage::Format_RGBA8888);

    AVFrame* frame = av_frame_alloc();
    //  AVPacket* pkt = av_packet_alloc();

    int out_size = 320000*2;
    uint8_t* play_buf = (uint8_t*)av_malloc(out_size);

    emit this->mediaReady();
    QThread::msleep(100);

    QIODevice* io = output->start();

    while(!exit)
    {
        while(pause)
        {
            QThread::msleep(100);
        }

#if 0
        int ret = av_read_frame(ic, pkt);
        if(ret < 0)
            break;
#endif

        AVPacket* pkt = reader.getPacket();
        if(pkt == NULL){
            QThread::msleep(100);
            continue;
        }

        if(pkt->stream_index == aIndex)
        {
            if(avcodec_send_packet(aCtx, pkt) == 0)
            {
                if(avcodec_receive_frame(aCtx, frame) == 0)
                {
                    ret = swr_convert(swr, &play_buf, out_size,
                                      (const uint8_t**)frame->data, frame->nb_samples);

                    while(output->bytesFree() < ret*4)
                    {
                        QThread::msleep(20);
                    }
                    io->write((char*)play_buf, ret  * 4);
                }
            }
        }
        else
        {
            if(avcodec_send_packet(vCtx, pkt) == 0)
            {
                if(avcodec_receive_frame(vCtx, frame) == 0)
                {
                    uint8_t* dst[1] = {image.bits()};
                    int dstStride[1] = {4*image.width()};
                    sws_scale(sws, frame->data, frame->linesize,
                              0, frame->height, dst, dstStride);

                    emit imageReady(image);
                }
            }
        }

        av_packet_unref(pkt);
        reader.setFreePacket(pkt);
    }

    reader.exit = true;
    QThread::msleep(200);

    av_free(play_buf);
    //   av_packet_free(&pkt);
    av_frame_free(&frame);
    avformat_free_context(ic);
    swr_free(&swr);
    sws_freeContext(sws);
    delete output;

    return 0;
}

AVPacket *DSReader::getFreePacket()
{
    {
        DSLock(mutex);
        if(this->pkts_free.size() > 0)
        {
            AVPacket* pkt = pkts_free.first();
            pkts_free.removeFirst();
            return pkt;
        }
    }

    return av_packet_alloc();
}

void DSReader::setFreePacket(AVPacket *pkt)
{
    {
        DSLock(mutex);
        pkts_free.push_back(pkt);
    }
}

AVPacket *DSReader::getPacket()
{
    AVPacket* pkt = NULL;
    {
        DSLock(mutex);
        if(pkts.size() == 0) return NULL;

        pkt = pkts.first();
        pkts.removeFirst();
    }

    return pkt;
}

void DSReader::setPacket(AVPacket *pkt)
{
    DSLock(mutex);
    pkts.push_back(pkt);
}

void DSReader::run()
{
    while(!exit)
    {
        while(pause)
        {
            QThread::msleep(100);
        }

        if(pkts.size() > buffer)
        {
            QThread::msleep(100);
            continue;
        }

        AVPacket* pkt = getFreePacket();

        {
            DSLock(mutex);
            int ret = av_read_frame(ic, pkt);
            if(ret < 0)
            {
                setFreePacket(pkt);
                continue;
            }
        }

        setPacket(pkt);
    }

    foreach (AVPacket* pkt, pkts) {
        av_packet_free(&pkt);
    }
    foreach (AVPacket* pkt, pkts_free) {
        av_packet_free(&pkt);
    }

    pkts.clear();
    pkts_free.clear();
}
