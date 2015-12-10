
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
#include <qwt_plot.h>
#include <qwt_plot_grid.h>

#include <qwt_legend.h>

#include <qwt_plot_curve.h>
#include <qwt_symbol.h>

#include <qwt_plot_magnifier.h>

#include <qwt_plot_panner.h>

#include <qwt_plot_picker.h>
#include <qwt_picker_machine.h>

void PlayerWindow::setGridLay(){
    firstLayout->addWidget(topPanel, 0, 0, 1, 6);
    firstLayout->addWidget(m_vo->widget(), 1, 0, 4, 4);
    firstLayout->addWidget(m_vo2->widget(), 1, 4, 2, 2);
    firstLayout->addWidget(container, 3, 4, 2, 2);
    firstLayout->addWidget(mpPlayList, 5, 4, 3, 2);
    firstLayout->addWidget(sliderButtonsWidg, 5, 0, 1, 4);
    firstLayout->addWidget(graphSectionWidg, 6, 0, 1, 4);
    firstLayout->addWidget(bot_buttons, 7, 0, 1, 4);


}

void PlayerWindow::setTopPanel(){
    topPanel = new QWidget();
    QHBoxLayout *topLayout = new  QHBoxLayout();
    topPanel->setContentsMargins(0,0,0,0);
    topPanel->setLayout(topLayout);
    topPanel->setFixedSize(1000,32);
    topPanel->setStyleSheet("QWidget {background-image: url(:/images/images/top/top_bg.png);}");

    QWidget *logo = new QWidget();
    logo->setStyleSheet("QWidget {background-image: url(:/images/images/top/logo1.png);}");
    logo->setContentsMargins(0,0,0,0);
    logo->setFixedSize(118,18);

    QLabel *title = new QLabel("Viewer");
    title->setStyleSheet("QLabel {background: transparent}"
                         "QLabel {color: white}");
    title->setContentsMargins(0,0,0,0);
    title->setFixedSize(800, 18);

    QPushButton *minimize = new QPushButton();
    minimize->setStyleSheet("QPushButton {background-image: url(:/images/images/top/minimize_btn_hover.png);}"
                            "QPushButton {background-image: url(:/images/images/top/minimize_btn_normal.png);}");
    minimize->setContentsMargins(0,0,0,0);
    minimize->setFixedSize(27,27);

    QPushButton *close = new QPushButton();
    close->setStyleSheet("QPushButton {background-image: url(:/images/images/top/close_btn_hover.png);}"
                         "QPushButton {background-image: url(:/images/images/top/close_btn_normal.png);}");
    close->setContentsMargins(0,0,0,0);
    close->setFixedSize(27,27);

    topLayout->addWidget(logo);
    topLayout->addWidget(title);
    topLayout->addWidget(minimize);
    topLayout->addWidget(close);
}

void PlayerWindow::setWidgetsSize(){
    //mpPlayList->setMaximumHeight(200);
    //mpPlayList->setMinimumWidth(340);

    m_vo->widget()->setMinimumSize(620, 435);
    m_vo->widget()->setMaximumSize(1220, 715);
    m_vo->widget()->setStyleSheet("QWidget {background-image: url(:/images/images/main_camera/main_camera_box.png);}");
    //m_vo2->widget()->setFixedHeight(196);
    m_vo2->widget()->setMinimumSize(380,225);
    m_vo2->widget()->setMaximumSize(700,365);
    
    //m_vo2->widget()->setBaseSize(675,380);
    m_vo2->widget()->setStyleSheet("QWidget {background-image: url(:/images/images/rear_camera/rear_camera_box.png);}");
    //m_vo2->widget()->setContentsMargins(5,0,0,0);
    //container->setFixedHeight(196);
    container->setMinimumSize(380,210);
    container->setMaximumSize(700,350);
    //container->setBaseSize(675,380);


    container->setFocusPolicy(Qt::TabFocus);
    container->setStyleSheet("QWidget {background-image: url(:/images/images/google_map/map_border_box.png);}");
    container->setContentsMargins(0,0,0,0);
}

void PlayerWindow::initMainVertWidgets(){
    vertLeft = new QVBoxLayout();
    vertLeft->setContentsMargins(0,0,0,0);
    vertRight = new QVBoxLayout();
    vertRight->setContentsMargins(0,0,0,0);
    vertRight->setAlignment(Qt::AlignTop);
    leftVertWidg = new QWidget();
    rightVertWidg = new QWidget();

    leftVertWidg->setStyleSheet("QWidget { background-color: rgb(18, 18, 18); }");
    rightVertWidg->setStyleSheet("QWidget { background-color: rgb(18, 18, 18); }");
    //rightVertWidg->setMaximumWidth(425);
    leftVertWidg->setLayout(vertLeft);
    rightVertWidg->setLayout(vertRight);
    vertLeft->setMargin(0);
    vertRight->setMargin(0);
    vertLeft->setSpacing(0);
    vertRight->setSpacing(0);


}

void PlayerWindow::initSliderButtonsWidgets(){
    m_slider = new QSlider();
    m_slider->setOrientation(Qt::Horizontal);
    m_slider->setContentsMargins(0,0,0,0);

    setSliderStyleSheet();


    widg = new QWidget();
    hb = new QHBoxLayout();
    //widg->setStyleSheet("QWidget { background-image: url(:/images/images/music_player/player_background.png); }");
    hb->setContentsMargins(0,0,0,0);
    widg->setContentsMargins(0,0,0,0);

    //widg->setFixedHeight(55);
    //widg->setMaximumWidth(1000);
    //widg->setFixedHeight(114);
    //widg->setMinimumWidth(435);
    //widg->setMaximumWidth(715);
    widg->setLayout(hb);

    sliderWidg = new QWidget();
    sliderWidg->setContentsMargins(0,0,0,0);
    //sliderWidg->setStyleSheet("QWidget { background-color: rgb(18, 18, 18); }");
    sliderWidg->setMaximumHeight(15);
    sliderLayout = new QHBoxLayout();
    sliderLayout->setContentsMargins(0,0,0,0);
    sliderWidg->setLayout(sliderLayout);


    currentTime->setText("00:00:00");
    totalTime->setText("00:00:00");
    currentTime->setContentsMargins(0,0,0,0);
    totalTime->setContentsMargins(0,0,0,0);
    currentTime->setStyleSheet("QLabel {color: white;}");
    totalTime->setStyleSheet("QLabel {color: white;}");

    sliderLayout->addWidget(currentTime);
    sliderLayout->addSpacing(20);
    sliderLayout->addWidget(m_slider);
    sliderLayout->addSpacing(20);
    sliderLayout->addWidget(totalTime);

//merge slider and buttons:
    sliderButtonsWidg = new QWidget();
    sliderButtonsLayout = new QVBoxLayout();
    sliderButtonsWidg->setLayout(sliderButtonsLayout);
    sliderButtonsWidg->setStyleSheet("QWidget { background-image: url(:/images/images/music_player/player_background.png); }");
    sliderButtonsWidg->setContentsMargins(0,0,0,0);
    //sliderButtonsWidg->setFixedSize(674, 98);
    sliderButtonsWidg->setFixedHeight(114);
    sliderButtonsWidg->setMinimumWidth(620);
    sliderButtonsWidg->setMaximumWidth(1220);

    widg->setStyleSheet("QWidget {background: transparent}");
    sliderWidg->setStyleSheet("QWidget {background: transparent}");
    sliderButtonsLayout->addWidget(sliderWidg);
    sliderButtonsLayout->addWidget(widg);

    hb->addWidget(prev_button);
    hb->addWidget(back_button);
    hb->addWidget(m_playBtn);
    hb->addWidget(forward_button);
    hb->addWidget(next_button);
    //hb->addWidget(m_stopBtn);

    strangeButtons = new QHBoxLayout();
    strangeButtonsWidg = new QWidget();
    strangeButtonsWidg->setLayout(strangeButtons);
    hb->addSpacing(40);
    hb->addWidget(strangeButtonsWidg);
    //strangeButtonsWidg->setMinimumHeight(55);
    strangeButtonsWidg->setMaximumWidth(350);
    strangeButtons->setContentsMargins(0,0,0,0);
    strangeButtonsWidg->setContentsMargins(0,0,0,0);
    strangeButtons->setAlignment(Qt::AlignRight);
    strangeButtons->setSpacing(0);
    strangeButtons->addWidget(b1);
    strangeButtons->addWidget(b2);
    strangeButtons->addWidget(b3);
    strangeButtons->addWidget(b4);
    strangeButtons->addWidget(b5);
    strangeButtons->addWidget(b6);
    strangeButtons->addWidget(b7);



}

void PlayerWindow::setSliderStyleSheet(){

    m_slider->setStyleSheet("QSlider::groove:horizontal { border: 1px solid #000000; "
                                "background-image: url(:/images/images/music_player/player_bg.png); "
                                "height: 10px; border-radius: 4px; } "
                            "QSlider::sub-page:horizontal { background: #ff9666; "
                                "border: 1px solid #777; "
                                "height: 10px; "
                                "border-radius: 4px; } "
                            "QSlider::handle:horizontal { background-image: url(:/images/images/graph_section/small_player_handle.png); "
                                "width: 20px; "
                                "margin-left: -5px; "
                                "margin-top: -4px; "
                                "margin-right: -5px; "
                                "margin-bottom: -5px; }");
}

void PlayerWindow::initGraphSectionWidgets(){
    graphSectionWidg = new QWidget();
    graphSectionWidg->setStyleSheet("QWidget { background-image: url(:/images/images/graph_section/graph_section_bg.png); }");
    graphSectionLayer = new QHBoxLayout();
    graphSectionWidg->setLayout(graphSectionLayer);
    graphSectionLayer->setContentsMargins(0,0,0,0);
    //graphSectionWidg->setFixedSize(674, 126);
    graphSectionWidg->setFixedHeight(126);
    graphSectionWidg->setMinimumWidth(620);
    graphSectionWidg->setMaximumWidth(1220);
    //graphSectionLayer->setSpacing(0);

    //LINE WITH SPEED
        lineWithSpeedWidg = new QWidget();
        lineWithSpeedWidg->setStyleSheet("QWidget {background-image: url(:/images/images/bg/bg.png))}");
        //lineWithSpeedWidg->setMaximumHeight(35);
        layerWithSpeed = new QVBoxLayout();
        layerWithSpeed->setAlignment(Qt::AlignCenter);
        lineWithSpeedWidg->setLayout(layerWithSpeed);
        lineWithSpeedWidg->setFixedHeight(124);
        graphSectionLayer->addWidget(lineWithSpeedWidg);
        layerWithSpeed->setContentsMargins(0,0,0,0);
        lineWithSpeedWidg->setContentsMargins(0,0,0,0);
        layerWithSpeed->addWidget(speedButton);

        speedButton->setAlignment(Qt::AlignCenter);


        layerWithSpeed->addWidget(speedProgress);
        speedProgress->setContentsMargins(50, 0, 0, 0);
        speedProgress->setAlignment(Qt::AlignCenter);
        //speedProgress->setStyleSheet("QWidget {background-image: url(:/images/images/bg/bg.png))}");
        lineWithSpeedWidg->setFixedWidth(160);
        prndm = new QHBoxLayout();
        prndmWidg = new QWidget();
        prndmWidg->setLayout(prndm);
        prndmWidg->setStyleSheet("QWidget { background-image: url(:/images/images/graph_section/gear_bg.png); }");
        prndm->setContentsMargins(0,0,0,0);
        prndmWidg->setContentsMargins(0,0,0,0);
        prndm->addWidget(pLabel);
        prndm->addWidget(rLabel);
        prndm->addWidget(nLabel);
        prndm->addWidget(dLabel);
        prndm->addWidget(mLabel);
        layerWithSpeed->addWidget(prndmWidg);


        //bottom sliders
            smallSlidersLayout = new QVBoxLayout();
            QWidget *smallSlidersWidget = new QWidget();
            smallSlidersWidget->setFixedWidth(120);
            smallSlidersWidget->setLayout(smallSlidersLayout);
            smallSlidersWidget->setStyleSheet("QWidget {background: transparent; }");
            speedLayout = new QHBoxLayout();
            brightnessLayout = new QHBoxLayout();
            soundLayout = new QHBoxLayout();

            smallSlidersLayout->setSpacing(0);


            play_speed_slider = new QSlider();
            play_speed_slider->setMinimum(-2);
            play_speed_slider->setMaximum(2);


            play_speed_slider->setSingleStep(1);
            play_speed_slider->setValue(0);

            play_speed_slider->setOrientation(Qt::Horizontal);

            //Brightness slider:
            brightness_slider = new QSlider();
            brightness_slider->setMinimum(-10);
            brightness_slider->setMaximum(10);


            brightness_slider->setSingleStep(1);
            brightness_slider->setValue(0);
            brightness_slider->setOrientation(Qt::Horizontal);

            //VOLUME SLIDER
            volume_slider = new QSlider();
            volume_slider = new QSlider();
            volume_slider->setMinimum(0);
            volume_slider->setMaximum(100);


            volume_slider->setSingleStep(1);
            volume_slider->setValue(50);
            volume_slider->setOrientation(Qt::Horizontal);
            setSmallSliders();

            speedLayout->addWidget(speedSliderLabel);
            speedLayout->addSpacing(10);
            speedLayout->addWidget(play_speed_slider);

            brightnessLayout->addWidget(brightnessSliderLabel);
            brightnessLayout->addSpacing(10);
            brightnessLayout->addWidget(brightness_slider);

            soundLayout->addWidget(volumeSliderLabel);
            soundLayout->addSpacing(10);
            soundLayout->addWidget(volume_slider);

            graphSectionLayer->addWidget(smallSlidersWidget);
            smallSlidersLayout->addLayout(speedLayout);
            smallSlidersLayout->addLayout(brightnessLayout);
            smallSlidersLayout->addLayout(soundLayout);

            //G-SENSOR VALUE:
                initGraph();
                gSensorsAllLayout = new QHBoxLayout;
                gSensorsWidg = new QWidget();
                gSensorsWidg->setLayout(gSensorsAllLayout);
                //gSensorsWidg->setContentsMargins(10,10,10,10);
                //gSensorsAllLayout->setContentsMargins(10,10,10,10);
                gSensorsWidg->setStyleSheet("QWidget { background-image: url(:/images/images/graph_section/graph_gray_bg.png); }");
                gSensorsWidg->setMaximumWidth(355);
                graphSectionLayer->addWidget(gSensorsWidg);

                gSensorsLabels = new QVBoxLayout();
                gSensorsAllLayout->setAlignment(Qt::AlignLeft);

                gSensorsAllLayout->addLayout(gSensorsLabels);
                gSensorsAllLayout->addWidget(plot);

                gSensorsLabels->setAlignment(Qt::AlignLeft);
                //gSensorX->setFixedWidth(70);
                //gSensorY->setFixedWidth(70);
                //gSensorZ->setFixedWidth(70);
                gSensorsLabels->addWidget(gSensorX);
                gSensorsLabels->addWidget(gSensorY);
                gSensorsLabels->addWidget(gSensorZ);
}

void PlayerWindow::setSmallSliders(){
    play_speed_slider->setStyleSheet("QSlider::groove:horizontal { border: 1px solid #000000; "
                                "background-image: url(:/images/images/graph_section/small_unfilled_line.png); "
                                "height: 9px; border-radius: 4px; } "
                            "QSlider::sub-page:horizontal { background: #ff9666; "
                                "border: 1px solid #777; "
                                "height: 8px; "
                                "border-radius: 4px; } "
                            "QSlider::handle:horizontal { background-image: url(:/images/images/graph_section/small_player_handle.png); "
                                "width: 20px; "
                                "margin-left: -5px; "
                                "margin-top: -4px; "
                                "margin-right: -5px; "
                                "margin-bottom: -5px; }");
    volume_slider->setStyleSheet("QSlider::groove:horizontal { border: 1px solid #000000; "
                                 "background-image: url(:/images/images/graph_section/small_unfilled_line.png); "
                                 "height: 9px; border-radius: 4px; } "
                             "QSlider::sub-page:horizontal { background: #ff9666; "
                                 "border: 1px solid #777; "
                                 "height: 8px; "
                                 "border-radius: 4px; } "
                             "QSlider::handle:horizontal { background-image: url(:/images/images/graph_section/small_player_handle.png); "
                                 "width: 20px; "
                                 "margin-left: -5px; "
                                 "margin-top: -4px; "
                                 "margin-right: -5px; "
                                 "margin-bottom: -5px; }");
    brightness_slider->setStyleSheet("QSlider::groove:horizontal { border: 1px solid #000000; "
                                     "background-image: url(:/images/images/graph_section/small_unfilled_line.png); "
                                     "height: 8px; border-radius: 4px; } "
                                 "QSlider::sub-page:horizontal { background: #ff9666; "
                                     "border: 1px solid #777; "
                                     "height: 9px; "
                                     "border-radius: 4px; } "
                                 "QSlider::handle:horizontal { background-image: url(:/images/images/graph_section/small_player_handle.png); "
                                     "width: 20px; "
                                     "margin-left: -5px; "
                                     "margin-top: -4px; "
                                     "margin-right: -5px; "
                                     "margin-bottom: -5px; }");
}

void PlayerWindow::initBottomButtonsWidget(){
    //DESIGN OF BOTTOM BUTTONS:
    bot_buttons = new QWidget();
    bot_buttons->setStyleSheet("QWidget { background-image: url(:/images/images/buttons_bg.png); }");
    //bot_buttons->setMaximumHeight(140);
    bottom_buttons = new QHBoxLayout();
    bottom_buttons->setContentsMargins(0,0,0,0);
    bot_buttons->setLayout(bottom_buttons);
    //bot_buttons->setMaximumWidth(1000);
    //bottom_buttons->setAlignment(Qt::AlignCenter);
    bottom_buttons->setSpacing(12);
    //bot_buttons->setFixedSize(674, 125);
    bot_buttons->setFixedHeight(125);
    bot_buttons->setMinimumWidth(620);
    bot_buttons->setMaximumWidth(1220);




    bot1 = new QVBoxLayout();
    bot2 = new QVBoxLayout();
    bot3 = new QVBoxLayout();
    bot4 = new QVBoxLayout();
    bot5 = new QVBoxLayout();
    bot1->setContentsMargins(0,0,0,0);
    bot2->setContentsMargins(0,0,0,0);
    bot3->setContentsMargins(0,0,0,0);
    bot4->setContentsMargins(0,0,0,0);
    bot5->setContentsMargins(0,0,0,0);

    botW1 = new QWidget();
    botW1->setStyleSheet("QWidget { background-image: url(:/images/images/graf_section/graph_gray_bg.png); }");
    botW1->setMaximumHeight(130);
    botW1->setLayout(bot1);

    botW2 = new QWidget();
    botW2->setStyleSheet("QWidget { background-image: url(:/images/images/graf_section/graph_gray_bg.png); }");
    botW2->setMaximumHeight(130);
    botW2->setLayout(bot2);

    botW3 = new QWidget();
    botW3->setStyleSheet("QWidget { background-image: url(:/images/images/graf_section/graph_gray_bg.png); }");
    botW3->setMaximumHeight(130);
    botW3->setLayout(bot3);
    botW4 = new QWidget();
    botW4->setStyleSheet("QWidget { background-image: url(:/images/images/graf_section/graph_gray_bg.png); }");
    botW4->setMaximumHeight(130);
    botW4->setLayout(bot4);
    botW5 = new QWidget();
    botW5->setStyleSheet("QWidget { background-image: url(:/images/images/graf_section/graph_gray_bg.png); }");
    botW5->setMaximumHeight(130);
    botW5->setLayout(bot5);

    bottom_buttons->addWidget(botW1);
    bottom_buttons->addSpacing(10);
    bottom_buttons->addWidget(botW2);
    bottom_buttons->addSpacing(10);
    bottom_buttons->addWidget(botW3);
    bottom_buttons->addSpacing(10);
    bottom_buttons->addWidget(botW4);
    bottom_buttons->addSpacing(10);
    bottom_buttons->addWidget(botW5);

    bot1->addWidget(obdi);

    bot2->addWidget(left_signal);
    bot2->addWidget(wheel_angle);

    bot3->addWidget(brake);
    bot3->addWidget(temperature);

    bot4->addWidget(accelerate);
    bot4->addWidget(voltage);

    bot5->addWidget(right_signal);
    bot5->addWidget(patrol);


}



void PlayerWindow::initPlaylistWidget(){
    //_______PLAYLIST_____
    mpPlayList = new PlayList(this);
    mpPlayList->load();
    //mpPlayList->setMinimumSize(323, 357);
    //mpPlayList->setFixedWidth(323);
    mpPlayList->setFixedHeight(365);
    mpPlayList->setMinimumWidth(380);
    mpPlayList->setMaximumWidth(700);
    connect(mpPlayList, SIGNAL(aboutToPlay(QString)), SLOT(play(QString)));

}

