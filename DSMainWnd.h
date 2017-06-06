#ifndef DSMAINWND_H
#define DSMAINWND_H

#include "DSDef.h"
#include "DSPlayer.h"
#include <QLabel>
#include <QSlider>
#include <QPushButton>
#include <QDoubleSpinBox>
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
