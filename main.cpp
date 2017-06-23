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
