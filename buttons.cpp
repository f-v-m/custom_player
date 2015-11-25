
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

#include "playerwindow.h"
#include <QPushButton>
#include <QSlider>
#include <QLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <iostream>
#include <QWidget>

//#include <QtGui>
#include <QtCore/QStringList>
#include <QtCore/QTextStream>
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlEngine>
#include <QtQml/QQmlContext>
#include <QtQuick/QQuickView>
#include <QtQuick/QQuickItem>
#include <QPalette>

#include "playlist/PlayList.h"
#include <QWidgetAction>


void PlayerWindow::setButtonsStyle(){
    //SIZE:
    changeButtonSize(m_playBtn);
    changeButtonSize(m_stopBtn);
    changeButtonSize(next_button);
    changeButtonSize(prev_button);

    changeButtonSize(back_button);
    changeButtonSize(forward_button);
    changeButtonSize(b1, 49, 46);
    changeButtonSize(b2, 49, 46);
    changeButtonSize(b3, 49, 46);
    changeButtonSize(b4, 49, 46);
    changeButtonSize(b5, 49, 46);
    changeButtonSize(b6, 49, 46);
    changeButtonSize(b7, 49, 46);

    changeButtonSize(obdi, 123, 115);
    changeButtonSize(left_signal, 111, 51);
    changeButtonSize(wheel_angle, 111, 51);
    changeButtonSize(brake, 111, 51);
    changeButtonSize(temperature, 111, 51);
    changeButtonSize(accelerate, 111, 51);
    changeButtonSize(voltage, 111, 51);
    changeButtonSize(right_signal, 111, 51);
    changeButtonSize(patrol, 111, 51);
    //changeButtonSize(speedButton, 150, 50);

    b1->setContentsMargins(0,0,0,0);
    b2->setContentsMargins(0,0,0,0);
    b3->setContentsMargins(0,0,0,0);
    b4->setContentsMargins(0,0,0,0);
    b5->setContentsMargins(0,0,0,0);
    b6->setContentsMargins(0,0,0,0);
    b7->setContentsMargins(0,0,0,0);


    //PLAYBUTTON
    m_playBtn->setStyleSheet("QPushButton { border-image: url(:/images/images/music_player/play_hover.png);} "
                             "QPushButton:hover { border-image: url(:/images/images/music_player/play_normal.png)}");
    //PREV BUTTON
    prev_button->setStyleSheet("QPushButton { border-image: url(:/images/images/music_player/Previous_File_hover_and_active.png);} "
                             "QPushButton:hover { border-image: url(:/images/images/music_player/Previous_File_normal.png)}");
    //BACK_BUTTON
    back_button->setStyleSheet("QPushButton { border-image: url(:/images/images/music_player/rewinding_hover_and_active.png);} "
                             "QPushButton:hover { border-image: url(:/images/images/music_player/rewinding_normal.png)}");
    //FORWARD_BUTTON
    forward_button->setStyleSheet("QPushButton { border-image: url(:/images/images/music_player/fowarding_active.png);} "
                             "QPushButton:hover { border-image: url(:/images/images/music_player/fowarding_normal.png)}");
    //NEXT BUTTON
    next_button->setStyleSheet("QPushButton { border-image: url(:/images/images/music_player/next_file_active.png);} "
                             "QPushButton:hover { border-image: url(:/images/images/music_player/next_file_normal.png)}");
    //STOP BUTTON

    //1 BUTTON
    b1->setStyleSheet("QPushButton { background-image: url(:/images/images/music_player/front_camera_hover.png);"
                      "border: none;} "
                             "QPushButton:hover { background-image: url(:/images/images/music_player/front_camera_normal.png);"
                      "border: none;}");
    //2 BUTTON
    b2->setStyleSheet("QPushButton { background-image: url(:/images/images/music_player/rear_view_hover.png);"
                      "border: none;} "
                             "QPushButton:hover { background-image: url(:/images/images/music_player/rear_view_normal.png);"
                      "border: none;}");
    //3 BUTTON
    b3->setStyleSheet("QPushButton { background-image: url(:/images/images/music_player/both_camera_hover.png);"
                      "border: none;} "
                             "QPushButton:hover { background-image: url(:/images/images/music_player/both_camera_normal.png);"
                      "border: none;}");
    //4 BUTTON
    b4->setStyleSheet("QPushButton { background-image: url(:/images/images/music_player/invert_upside_down_hover.png);"
                      "border: none;} "
                             "QPushButton:hover { background-image: url(:/images/images/music_player/invert_upside_down_normal.png);"
                      "border: none;}");
    //5 BUTTON
    b5->setStyleSheet("QPushButton { background-image: url(:/images/images/music_player/invert_left_To_right_hover.png);"
                      "border: none;} "
                             "QPushButton:hover { background-image: url(:/images/images/music_player/invert_left_To_right_normal.png);"
                      "border: none;}");
    //6 BUTTON
    b6->setStyleSheet("QPushButton { background-image: url(:/images/images/music_player/crop_hover.png);"
                      "border: none;} "
                             "QPushButton:hover { background-image: url(:/images/images/music_player/crop_normal.png);"
                      "border: none;}");
    //7 BUTTON
    b7->setStyleSheet("QPushButton { background-image: url(:/images/images/music_player/fullscreen_hover.png);"
                      "border: none;} "
                             "QPushButton:hover { background-image: url(:/images/images/music_player/fullscreen_normal.png);"
                      "border: none;}");

    //OBDI
    obdi->setStyleSheet("QPushButton { border-image: url(:/images/images/bottom_buttons/OBDI_normal.png);} "
                             "QPushButton:hover { border-image: url(:/images/images/bottom_buttons/OBDI_hover.png)}");
    //LEFT SIGNAL
    left_signal->setStyleSheet("QPushButton { border-image: url(:/images/images/bottom_buttons/left_signal_normal.png);} "
                                "QPushButton:hover { border-image: url(:/images/images/bottom_buttons/left_signal_hover.png)}");
    //WHEEL ANGLE
    wheel_angle->setStyleSheet("QPushButton { border-image: url(:/images/images/bottom_buttons/whell_angle_normal.png);} "
                                "QPushButton:hover { border-image: url(:/images/images/bottom_buttons/whell_angle_hover.png);}"
                               "QPushButton { color: white ;}"
                               "QPushButton { text-align: center }");
    //BRAKE
    brake->setStyleSheet("QPushButton { border-image: url(:/images/images/bottom_buttons/brake_normal.png);} "
                                "QPushButton:hover { border-image: url(:/images/images/bottom_buttons/brake_hover.png)}");

    //temperature
    temperature->setStyleSheet("QPushButton { border-image: url(:/images/images/bottom_buttons/temprature_normal.png);} "
                                "QPushButton:hover { border-image: url(:/images/images/bottom_buttons/temprature_hover.png)}");
    //ACCELERATE
    accelerate->setStyleSheet("QPushButton { border-image: url(:/images/images/bottom_buttons/accelerate_normal.png);} "
                                "QPushButton:hover { border-image: url(:/images/images/bottom_buttons/accelerate_hover.png)}");
    //VOLTAGE
    voltage->setStyleSheet("QPushButton { border-image: url(:/images/images/bottom_buttons/voltage_normal.png);} "
                                "QPushButton:hover { border-image: url(:/images/images/bottom_buttons/voltage_hover.png);}"
                           "QPushButton {color: white;}"
                           "QPushButton { text-align: center }");
    //RIGHT SIGNAL
    right_signal->setStyleSheet("QPushButton { border-image: url(:/images/images/bottom_buttons/right_signal_normal.png);} "
                                "QPushButton:hover { border-image: url(:/images/images/bottom_buttons/right_signal_hover.png)}");
    //PETROL
    patrol->setStyleSheet("QPushButton { border-image: url(:/images/images/bottom_buttons/petrol_normal.png);} "
                                "QPushButton:hover { border-image: url(:/images/images/bottom_buttons/petrol_hover.png);}"
                          "QPushButton {color: white;}"
                          "QPushButton { text-align: center }");
    //SPEED
    speedButton->setStyleSheet("QLabel { background-image: url(:/images/images/graph_section/speed_metter_bg.png); }"
                               "QLabel {color: white;}"
                               "QLabel {font-weight: bold;}"
                               "QLabel {font-size: 15pt;}");
    speedButton->setFixedSize(150, 50);

    //SLIDER TEXT:

    //PROGRESS BAR:
    speedProgress->setStyleSheet("QProgressBar:horizontal {border: 0px solid gray;}"
                                 "QProgressBar:horizontal {background-image: url(:/images/images/graph_section/speed_metter_dot_normal2.png);}"
                                 "QProgressBar::chunk:horizontal {background-image: url(:/images/images/graph_section/speed_metter_dot_active2.png);}");
    speedProgress->setMaximumWidth(160);
    speedProgress->setFixedHeight(13);
    speedProgress->setTextVisible(false);

    speedSliderLabel->setFixedSize(12, 12);
    speedSliderLabel->setStyleSheet("QPushButton { border-image: url(:/images/images/graph_section/small_forwarding_icon.png);}");

    brightnessSliderLabel->setFixedSize(12, 12);
    brightnessSliderLabel->setStyleSheet("QPushButton { border-image: url(:/images/images/graph_section/small_brightness-icon.png);}");

    volumeSliderLabel->setFixedSize(12, 12);
    volumeSliderLabel->setStyleSheet("QPushButton { border-image: url(:/images/images/graph_section/smal_volume_icon.png);}");


    gSensorX->setStyleSheet("QLabel {color: green;"
                            "font-size: 15px}");
    gSensorY->setStyleSheet("QLabel {color: red;"
                            "font-size: 15px}");
    gSensorZ->setStyleSheet("QLabel {color: blue;"
                            "font-size: 15px}");
    pLabel->setFixedSize(29, 29);
    rLabel->setFixedSize(29, 29);
    nLabel->setFixedSize(29, 29);
    dLabel->setFixedSize(29, 29);
    mLabel->setFixedSize(29, 29);

    pLabel->setContentsMargins(0,0,0,0);
    rLabel->setContentsMargins(0,0,0,0);
    nLabel->setContentsMargins(0,0,0,0);
    dLabel->setContentsMargins(0,0,0,0);
    mLabel->setContentsMargins(0,0,0,0);

    pLabel->setStyleSheet("QLabel {color: white;"
                            "font-size: 14px}");
    rLabel->setStyleSheet("QLabel {color: white;"
                            "font-size: 14px}");
    nLabel->setStyleSheet("QLabel {color: white;"
                            "font-size: 14px}");
    dLabel->setStyleSheet("QLabel {color: white;"
                            "font-size: 14px}");
    mLabel->setStyleSheet("QLabel {color: white;"
                            "font-size: 14px}");
    pLabel->setAlignment(Qt::AlignCenter);
    rLabel->setAlignment(Qt::AlignCenter);
    nLabel->setAlignment(Qt::AlignCenter);
    dLabel->setAlignment(Qt::AlignCenter);
    mLabel->setAlignment(Qt::AlignCenter);
}



void PlayerWindow::changeButtonSize(QPushButton *button){
    button->setFixedSize(15, 18);
}

void PlayerWindow::changeButtonSize(QPushButton *button, int w, int h){
    button->setFixedSize(w, h);
}


void PlayerWindow::initButtons(){
    currentTime = new QLabel();
    totalTime = new QLabel();
    m_playBtn = new QPushButton(tr(""));
    m_stopBtn = new QPushButton(tr(""));
    //new_button = new QPushButton(tr("azaza"));
    next_button = new QPushButton(tr(""));
    prev_button = new QPushButton(tr(""));
    back_button = new QPushButton(tr(""));
    forward_button = new QPushButton(tr(""));
    b1 = new QPushButton();
    b2 = new QPushButton();
    b3 = new QPushButton();
    b4 = new QPushButton();
    b5 = new QPushButton();
    b6 = new QPushButton();
    b7 = new QPushButton();

    obdi = new QPushButton(tr(""));
    left_signal = new QPushButton(tr(""));
    wheel_angle = new QPushButton(tr(""));
    brake = new QPushButton(tr(""));
    temperature = new QPushButton(tr(""));
    accelerate = new QPushButton(tr(""));
    voltage = new QPushButton(tr(""));
    right_signal = new QPushButton(tr(""));
    patrol = new QPushButton(tr(""));

    speedButton = new QLabel("Current Speed");

    speedProgress = new QProgressBar();

    speedSliderLabel = new QPushButton();
    brightnessSliderLabel = new QPushButton();
    volumeSliderLabel = new QPushButton();

    gSensorX = new QLabel(" X: 0000");
    gSensorY = new QLabel(" Y: 0000");
    gSensorZ = new QLabel(" Z: 0000");

    pLabel = new QLabel("P");
    rLabel = new QLabel("R");
    nLabel = new QLabel("N");
    dLabel = new QLabel("D");
    mLabel = new QLabel("M");

}


