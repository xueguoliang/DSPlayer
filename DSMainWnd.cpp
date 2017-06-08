#include "DSMainWnd.h"
#include <QMenuBar>
#include <QMetaMethod>
#include <QFileDialog>
#include <QPainter>
#include <QStatusBar>

DSMainWnd::DSMainWnd(QWidget *parent) : QMainWindow(parent)
{
    this->grabKeyboard();
    this->screen = new QWidget;
    setCentralWidget(screen);
    initMenus();
    screen->installEventFilter(this);

    status = new QLabel("没有播放任何东西");
    statusBar()->addWidget(status);
    statusBar()->addWidget(this->progress = new QSlider, 1);
    statusBar()->addWidget(this->speedLabel = new QLabel("速度:"));
    statusBar()->addWidget(this->speed = new QDoubleSpinBox);
    speed->setMinimum(0.5);
    speed->setMaximum(4);
    speed->setValue(1);

    connect(speed, SIGNAL(valueChanged(double)), this, SLOT(onSpeedChanged(double)));

    this->progress->setMinimum(0);
    this->progress->setOrientation(Qt::Horizontal);
    connect(progress, SIGNAL(sliderPressed()), this, SLOT(sliderPressed()));
    connect(progress, SIGNAL(sliderReleased()), this, SLOT(sliderReleased()));
    connect(progress, SIGNAL(sliderMoved(int)), this, SLOT(sliderMoved(int)));
    connect(progress, SIGNAL(valueChanged(int)), this, SLOT(valueChanged(int)));
}

void DSMainWnd::initMenus()
{
    QAction* action;
    QMenu* menu;
    menu = menuBar()->addMenu("File");
    action = menu->addAction(QIcon(), "打开", this, SLOT(onOpen()), QKeySequence::Open);
    actions.insert("打开", action);
}

bool DSMainWnd::eventFilter(QObject *watched, QEvent *ev)
{
    if(watched == screen)
    {
        if(ev->type() == QEvent::Paint)
        {
            QPainter p(screen);
            if(!image.isNull())
            {

                p.drawImage(0, 0,
                            image.scaled(screen->size(),
                                         Qt::IgnoreAspectRatio,
                                         Qt::SmoothTransformation));


                if(player && player->pause)
                {
                    p.drawText(screen->rect(), "暂停", QTextOption(Qt::AlignCenter));
                }
            }
        }
        else if(ev->type() == QEvent::MouseButtonPress)
        {
            qDebug() << ev->type() ;
            QMouseEvent* mouseEv = (QMouseEvent*)ev;
            qDebug( ) << mouseEv->button();
            if(mouseEv->button() == Qt::LeftButton)
            {
                if(player)
                    player->pause = !player->pause;
                update();
            }
        }
    }
    return QMainWindow::eventFilter(watched, ev);
}


void DSMainWnd::keyPressEvent(QKeyEvent *ev)
{
    if(ev->key() == Qt::Key_Left)
    {
        int value = progress->value() - 10;
        if(value < 0 ) value = 0;
        progress->setValue(value);

    }
    else if(ev->key() == Qt::Key_Right)
    {
        int value = progress->value() + 10;
        if(value > progress->maximum()) value = progress->maximum()-1;
        progress->setValue(value);
    }
}

void DSMainWnd::imageReady(QImage image)
{
    this->image = image;
    screen->update();

    this->progress->setValue(this->progress->value()+1);
}

void DSMainWnd::onOpen()
{
    if(player) player->exit = true;
    QThread::msleep(100);

    player = new DSPlayer;
    player->strFilename = "c:/users/xueguoliang/desktop/1.mp4";
    //    player->strFilename = "rtmp://192.168.1.152/live/abc";
    player->start();
    connect(player, SIGNAL(imageReady(QImage)),
            this, SLOT(imageReady(QImage)));
    connect(player, SIGNAL(mediaReady()), this, SLOT(mediaReady()));
    status->setText("正在播放:"+player->strFilename);

    progressValue = 0;


    return;

#if 0
    QString strFile = QFileDialog::getOpenFileName();
    if(strFile.length())
    {
        strFilename = strFile;
        DSPlayer* player = DSPlayer::instance();
        player->open(strFile, this->videoPlayer);
        player->start();
    }
#endif
}

void DSMainWnd::mediaReady()
{
    qDebug() << "nb_frames" << player->ic->streams[player->vIndex]->nb_frames;
    this->progress->setMaximum(player->ic->streams[player->vIndex]->nb_frames);
    qDebug() << this->progress->maximum();
    qDebug() << "duration:" << player->ic->duration;
    qDebug() << AV_TIME_BASE;
    this->progress->setValue(0);
}

void DSMainWnd::sliderPressed()
{

}

void DSMainWnd::sliderReleased()
{

}

void DSMainWnd::sliderMoved(int)
{

}

void DSMainWnd::valueChanged(int value)
{
    if(!player) return;
    if(value - progressValue != 1)
    {
        bool pause = player->pause;
        player->pause = true;
        QThread::msleep(20);

        qDebug() << "video duration" <<player->ic->streams[player->vIndex]->duration;
        float percent = value*1.0/progress->maximum();
        int64_t stamp = percent * player->ic->streams[player->vIndex]->duration*percent;

        {
            DSLock(player->reader.mutex);

            foreach (AVPacket* pkt, player->reader.pkts) {
                av_packet_unref(pkt);
                player->reader.pkts_free.push_back(pkt);
            }
            player->reader.pkts.clear();

            av_seek_frame(player->ic,
                          player->vIndex,
                          stamp,
                          AVSEEK_FLAG_BACKWARD);
        }
        player->pause = pause;
    }
    progressValue = value;
}

void DSMainWnd::onSpeedChanged(double v)
{
    bool pause = player->pause;
    QThread::msleep(20);

    player->initSwr(v);

    player->pause = pause;
}



