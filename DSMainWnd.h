/*
 * This file is part of the DSPlayer.
 *
 * Copyright (C) 2017 by Guoliang Xue <me@xueguoliang.com>
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

#ifndef DSMAINWND_H
#define DSMAINWND_H

#include "DSDef.h"
#include "DSPlayer.h"
#include <QLabel>
#include <QSlider>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QTreeWidget>
#include <QDockWidget>
#include <QComboBox>

class DSMainWnd : public QMainWindow
{
    Q_OBJECT
public:
    explicit DSMainWnd(QWidget *parent = 0);

    QWidget* screen;
    DSPlayer* player = NULL;

    QMap<QString, QAction*> actions;
    void initMenus();

    bool eventFilter(QObject *watched, QEvent *ev);
    QImage image;

    QLabel* status;
    QSlider* progress;
    int progressValue = 0;
    QLabel* speedLabel;
    QDoubleSpinBox* speed;

    QTreeWidget* treeWidget;
    QDockWidget* dock;

    void keyPressEvent(QKeyEvent *ev);
signals:

public slots:
    void imageReady(QImage image);
    void onOpen();
    void mediaReady();

    void sliderPressed();
    void sliderReleased();
    void sliderMoved(int);
    void valueChanged(int);

    void onSpeedChanged(double);

 //   connect(progress, SIGNAL(sliderPressed()), this, SLOT(sliderPressed()));
 //   connect(progress, SIGNAL(sliderReleased()), this, SLOT(sliderReleased()));
 //   connect(progress, SIGNAL(sliderMoved(int)), this, SLOT(sliderMoved(int)));
 //   connect(progress, SIGNAL(valueChanged(int)), this, SLOT(valueChanged()));

};

#endif // DSMAINWND_H
