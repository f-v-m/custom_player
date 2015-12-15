
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
    firstLayout->setSpacing(0);
    firstLayout->setContentsMargins(0,0,0,0);


    firstLayout->addWidget(topPanel, 0, 0, 1, 6);
    firstLayout->addWidget(m_vo->widget(), 1, 0, 4, 4);
    firstLayout->addWidget(m_vo2->widget(), 1, 4, 2, 1);

    firstLayout->addWidget(container, 3, 4, 2, 1);
    firstLayout->addWidget(mpPlayList, 5, 4, 3, 1);
    firstLayout->addWidget(sliderButtonsWidg, 5, 0, 1, 4);
    firstLayout->addWidget(graphSectionWidg, 6, 0, 1, 4);
    firstLayout->addWidget(bottomStack, 7, 0, 1, 4);
    container->hide();
    //firstLayout->addWidget(bottomButtonsWidg2, 8,0,1,4);

    //firstLayout->setAlignment(Qt::AlignLeft);


}

void PlayerWindow::setTopPanel(){
    topPanel = new QWidget();
    QHBoxLayout *topLayout = new  QHBoxLayout();
    topLayout->setContentsMargins(0,0,0,0);
    topPanel->setContentsMargins(0,0,0,0);
    topPanel->setLayout(topLayout);
    topPanel->setFixedSize(1000,32);
    topPanel->setStyleSheet("QWidget {background-image: url(:/images/images/top/top_bg.png);}");
    topLayout->setAlignment(Qt::AlignVCenter);
    topLayout->setSpacing(0);

    logo = new QWidget();
    logo->setStyleSheet("QWidget {background-image: url(:/images/images/top/logo1.png);}");
    logo->setContentsMargins(0,0,0,0);
    logo->setFixedSize(118,18);

    title = new QLabel("Viewer");
    title->setStyleSheet("QLabel {background: transparent}"
                         "QLabel {color: white}");
    title->setAlignment(Qt::AlignCenter);
    title->setContentsMargins(0,0,0,0);
    title->setFixedSize(800, 18);

    minimize = new QPushButton();
    minimize->setStyleSheet("QPushButton {border-image: url(:/images/images/top/minimize_btn_normal.png);}"
                            "QPushButton {border-image: url(:/images/images/top/minimize_btn_hover.png);}");
    minimize->setContentsMargins(0,0,0,0);
    minimize->setFixedSize(27,27);

    maximize = new QPushButton();
    maximize->setStyleSheet("QPushButton {border-image: url(:/images/images/top/maximize_normal.png);}"
                            "QPushButton {border-image: url(:/images/images/top/maximize_hover.png);}");
    maximize->setContentsMargins(0,0,0,0);
    maximize->setFixedSize(27,27);

    close = new QPushButton();
    close->setStyleSheet("QPushButton {border-image: url(:/images/images/top/close_btn_normal.png);}"
                         "QPushButton {border-image: url(:/images/images/top/close_btn_hover.png);}");
    close->setContentsMargins(0,0,0,0);
    close->setFixedSize(27,27);

    topLayout->addWidget(logo);
    topLayout->addWidget(title);
    topLayout->addWidget(minimize);
    topLayout->addWidget(maximize);
    topLayout->addWidget(close);

    connect(minimize, SIGNAL(clicked()), SLOT(hideMainWindow()));
    connect(maximize, SIGNAL(clicked()), SLOT(maxNormalView()));
    connect(close, SIGNAL(clicked()), qApp, SLOT(closeAllWindows()));
}

void PlayerWindow::setWidgetsSize(){

    //m_vo->widget()->setFixedSize(620, 440);
    m_vo->widget()->setMinimumSize(620, 435);
    m_vo->widget()->setMaximumSize(1300, 715);
    //m_vo->widget()->setMaximumSize(1220, 715);
    m_vo->widget()->setStyleSheet("QWidget {background-image: url(:/images/images/main_camera/main_camera_box.png);}");

    //m_vo2->widget()->setContentsMargins(0,0,0,0);
    m_vo2->widget()->setFixedSize(380,225);
    //m_vo2->widget()->setMaximumSize(700,365);
    m_vo2->widget()->setStyleSheet("QWidget {background-image: url(:/images/images/rear_camera/rear_camera_box.png);}");

    container->setFixedSize(380,215);
    //container->setMaximumSize(700,350);
    container->setFocusPolicy(Qt::TabFocus);
    container->setStyleSheet("QWidget {background-color: #121212;}");
    container->setContentsMargins(0,0,0,0);
}



void PlayerWindow::initSliderButtonsWidgets(){
    m_slider = new QSlider();
    m_slider->setOrientation(Qt::Horizontal);
    m_slider->setContentsMargins(0,0,0,0);

    setSliderStyleSheet();


    widg = new QWidget();
    hb = new QHBoxLayout();
    hb->setContentsMargins(0,0,0,0);
    hb->setAlignment(Qt::AlignCenter);
    widg->setContentsMargins(0,0,0,0);
    widg->setLayout(hb);

    sliderWidg = new QWidget();
    sliderWidg->setContentsMargins(0,0,0,0);
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
    sliderButtonsWidg->setFixedSize(620,114);

   // sliderButtonsWidg->setMaximumWidth(1220);

    widg->setStyleSheet("QWidget {background: transparent}");
    sliderWidg->setStyleSheet("QWidget {background: transparent}");
    sliderButtonsLayout->addWidget(sliderWidg);
    sliderButtonsLayout->addWidget(widg);

    QWidget *playerButtons = new QWidget();
    QHBoxLayout *playerButtonsLay = new QHBoxLayout();
    playerButtons->setLayout(playerButtonsLay);
    playerButtons->setFixedWidth(200);
    playerButtonsLay->setContentsMargins(0,0,0,0);
    playerButtons->setContentsMargins(0,0,0,0);
    playerButtonsLay->setSpacing(15);

    playerButtonsLay->addWidget(prev_button);
    playerButtonsLay->addWidget(back_button);
    playerButtonsLay->addWidget(m_playBtn);
    playerButtonsLay->addWidget(forward_button);
    playerButtonsLay->addWidget(next_button);
    playerButtonsLay->setAlignment(Qt::AlignLeft);
    hb->addWidget(playerButtons);


    strangeButtons = new QHBoxLayout();
    strangeButtonsWidg = new QWidget();
    strangeButtonsWidg->setLayout(strangeButtons);
    strangeButtons->setAlignment(Qt::AlignRight);
    hb->addSpacing(20);
    hb->addWidget(strangeButtonsWidg);
    //strangeButtonsWidg->setMinimumHeight(55);
    strangeButtonsWidg->setFixedWidth(m_vo->widget()->width()-270);
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
    graphSectionWidg->setFixedSize(620, 126);

    //graphSectionWidg->setMaximumWidth(1220);
    graphSectionLayer->setSpacing(0);

    //LINE WITH SPEED
        lineWithSpeedWidg = new QWidget();
        lineWithSpeedWidg->setStyleSheet("QWidget {border-image: url(:/images/images/bg/bg.png))}");
        //lineWithSpeedWidg->setMaximumHeight(35);
        layerWithSpeed = new QVBoxLayout();
        layerWithSpeed->setAlignment(Qt::AlignCenter);
        lineWithSpeedWidg->setLayout(layerWithSpeed);
        layerWithSpeed->setSpacing(0);
        graphSectionLayer->addWidget(lineWithSpeedWidg);
        layerWithSpeed->setContentsMargins(0,0,0,0);
        lineWithSpeedWidg->setContentsMargins(0,0,0,0);
        layerWithSpeed->addWidget(speedButton);
        speedButton->setAlignment(Qt::AlignCenter);


        layerWithSpeed->setAlignment(Qt::AlignHCenter);

        layerWithSpeed->addWidget(speedProgress);
        speedProgress->setContentsMargins(10, 0, 0, 0);
        speedProgress->setAlignment(Qt::AlignRight);
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
                gSensorsWidg->setFixedWidth(340);
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
    //bottom_buttons->setAlignment(Qt::AlignCenter);
    bottom_buttons->setSpacing(10);
    bot_buttons->setContentsMargins(0,0,0,0);
    bot_buttons->setFixedSize(620, 125);
    //bot_buttons->setMaximumWidth(1220);




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
    botW1->setFixedWidth(123);
    botW1->setLayout(bot1);

    botW2 = new QWidget();
    botW2->setStyleSheet("QWidget { background-image: url(:/images/images/graf_section/graph_gray_bg.png); }");
    botW2->setFixedWidth(111);
    botW2->setLayout(bot2);

    botW3 = new QWidget();
    botW3->setStyleSheet("QWidget { background-image: url(:/images/images/graf_section/graph_gray_bg.png); }");
    botW3->setFixedWidth(111);
    botW3->setLayout(bot3);
    botW4 = new QWidget();
    botW4->setStyleSheet("QWidget { background-image: url(:/images/images/graf_section/graph_gray_bg.png); }");
    botW4->setFixedWidth(111);
    botW4->setLayout(bot4);
    botW5 = new QWidget();
    botW5->setStyleSheet("QWidget { background-image: url(:/images/images/graf_section/graph_gray_bg.png); }");
    botW5->setFixedWidth(111);
    botW5->setLayout(bot5);

    bottom_buttons->addWidget(botW1);
    //bottom_buttons->addSpacing(10);
    bottom_buttons->addWidget(botW2);
    //bottom_buttons->addSpacing(10);
    bottom_buttons->addWidget(botW3);
    //bottom_buttons->addSpacing(10);
    bottom_buttons->addWidget(botW4);
    //bottom_buttons->addSpacing(10);
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

    //full screen bot buttons:
    bottomButtonsWidg2 = new QWidget();
    bottomButtonsLayout2 = new QHBoxLayout();
    bottomButtonsWidg2->setLayout(bottomButtonsLayout2);
    bottomButtonsWidg2->setStyleSheet("QWidget { background-image: url(:/images/images/buttons_bg.png); }");
    bottomButtonsWidg2->setFixedSize(1220, 60);
    bottomButtonsWidg2->setContentsMargins(0,0,0,0);

    bottomButtonsLayout2->addWidget(obdi2);

    bottomButtonsLayout2->addWidget(left_signal2);
    bottomButtonsLayout2->addWidget(brake2);
    bottomButtonsLayout2->addWidget(accelerate2);
    bottomButtonsLayout2->addWidget(right_signal2);
    bottomButtonsLayout2->addWidget(wheel_angle2);

    bottomButtonsLayout2->addWidget(temperature2);

    bottomButtonsLayout2->addWidget(voltage2);

    bottomButtonsLayout2->addWidget(patrol2);

    bottomStack = new QStackedWidget();

    bottomStack->addWidget(bot_buttons);
    bottomStack->addWidget(bottomButtonsWidg2);

    bottomStack->setContentsMargins(0,0,0,0);
    bottomStack->setFixedSize(620, 125);
    bottomStack->setCurrentIndex(0);
}





void PlayerWindow::initPlaylistWidget(){
    //_______PLAYLIST_____
    mpPlayList = new PlayList(this);
    mpPlayList->load();
    mpPlayList->setFixedSize(380, 366);
    mpPlayList->setContentsMargins(0,0,0,0);
    //mpPlayList->setMaximumWidth(700);
    connect(mpPlayList, SIGNAL(aboutToPlay(QString)), SLOT(play(QString)));

    mpPlayListEvent = new PlayList(this);
    mpPlayListEvent->load();
    mpPlayListEvent->setFixedSize(380, 366);
    mpPlayListEvent->setContentsMargins(0,0,0,0);
    //mpPlayList->setMaximumWidth(700);
    connect(mpPlayListEvent, SIGNAL(aboutToPlay(QString)), SLOT(play(QString)));

}

void PlayerWindow::initBottomButtons2(){
    int i = 0;
}
