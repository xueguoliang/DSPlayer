#include <QApplication>
#include "DSMainWnd.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    av_register_all();
    avformat_network_init();

    DSMainWnd wnd;
    wnd.resize(800, 600);
    wnd.show();

    return app.exec();
}
