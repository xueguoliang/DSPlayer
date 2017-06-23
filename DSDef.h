/*
 * This file is part of the DSPlayer.
 *
 * Copyright (C) 2017 by Guoliang Xue <me@xueguoliang.cn>
 * WebSite http://www.xueguoliang.cn
 *
 *
 * The DSPlayer is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The DSPlayer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with the xs Library. If not, see <http://www.gnu.org/licenses/>.
 */

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
