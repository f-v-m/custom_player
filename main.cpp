/******************************************************************************
    Simple Player:  this file is part of QtAV examples
    Copyright (C) 2012-2015 Wang Bin <wbsecg1@gmail.com>

*   This file is part of QtAV

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/
#include <QApplication>
#include "playerwindow.h"
#include <QtAVWidgets>







int main(int argc, char *argv[])
{
    QtAV::Widgets::registerRenderers();
    QApplication a(argc, argv);
    a.setApplicationName("ADAS ONE PC Viewer");

    PlayerWindow player;
    player.setWindowFlags(Qt::FramelessWindowHint);
    //player.move(10,10);
    player.setFixedSize(1000, 832);
    //player.setMaximumSize(1920,1080);
    player.show();
    //player.resize(800, 600);

    //__________


    //__________
    return a.exec();
}
