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

#ifndef DSPLAYER_H
#define DSPLAYER_H

#include "DSDef.h"

class DSAutoLock
{
public:
    DSAutoLock(QMutex& mutex) : mutex(mutex)
    {
        mutex.lock();
    }
    ~DSAutoLock()
    {
        mutex.unlock();
    }
public:
    QMutex& mutex;
};

#define DSLock(mutex) DSAutoLock __lock__xx__(mutex); Q_UNUSED(__lock__xx__)

class DSReader : public QThread
{
public:
    AVFormatContext* ic = NULL;
    QList<AVPacket*> pkts;
    QList<AVPacket*> pkts_free;

    AVPacket *getFreePacket();
    void setFreePacket(AVPacket* pkt);

    AVPacket* getPacket();
    void setPacket(AVPacket* pkt);

    QMutex mutex;

    bool pause = true;
    bool exit = false;
    int buffer;
    void run();
};

class DSPlayer : public QThread
{
    Q_OBJECT
public:
    explicit DSPlayer(QObject *parent = 0);
    ~DSPlayer();

    QString strFilename;
    bool isNetworkFile =false;
    void run();

    DSReader reader;

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
