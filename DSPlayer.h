#ifndef DSPLAYER_H
#define DSPLAYER_H

#include "DSDef.h"

class DSPlayer : public QThread
{
    Q_OBJECT
public:
    explicit DSPlayer(QObject *parent = 0);
    ~DSPlayer();

    QString strFilename;
    void run();

    AVFormatContext* ic = NULL;
    QAudioOutput* output;
    bool pause = false;
    bool exit = false;
    int vIndex;
    int aIndex;
    SwrContext* swr = NULL;
    AVCodecContext* aCtx;
    QAudioFormat format;

    void initSwr(double speed);
protected:
    int play();

signals:
    void imageReady(QImage image);
    void mediaReady();

public slots:
};

#endif // DSPLAYER_H
