QT += multimedia widgets gui

win32{
INCLUDEPATH += C:\MinGW\msys\1.0\local\include
LIBS += -LC:\MinGW\msys\1.0\local\lib -lavdevice -lavfilter  \
-lpostproc -lavformat -lavcodec -lavutil -lswscale -lswresample
}

HEADERS += \
    DSPlayer.h \
    DSMainWnd.h \
    DSDef.h

SOURCES += \
    main.cpp \
    DSPlayer.cpp \
    DSMainWnd.cpp
