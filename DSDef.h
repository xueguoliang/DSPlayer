
#ifndef DSDEF_H
#define DSDEF_H

#include <QEvent>
#include <QApplication>
#include <QMainWindow>
#include <QAudioOutput>
#include <QMap>
#include <QAction>
#include <QObject>
#include <QThread>
#include <QObject>
#include <QThread>
#include <QTimer>
#include <QDebug>
#include <QTimerEvent>
#include <QMouseEvent>
#include <QMutex>
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavdevice/avdevice.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
}

#endif
