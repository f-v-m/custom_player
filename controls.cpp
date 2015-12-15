

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
#include <QDebug>
using namespace std;


void PlayerWindow::updateSlider()
{

    m_slider->setRange(0, int(m_player->duration()/1000LL));
    cout << int(m_player->duration()) << " pl duration" << endl;
    m_slider->setValue(int(m_player->position()/1000LL));
    cout << int(m_player->position()/1000LL) << " pl position" << endl;

    if (totalTime->text() == "00:00:00"){
        int seconds, minutes, hours;
        seconds = int(m_player->duration())/1000LL;
        hours = seconds / 3600;
        minutes = (seconds - hours*3600) / 60;
        seconds = (seconds - hours*3600 - minutes*60);
        totalTime->setText(QString("%1").arg(hours, 2, 10, QChar('0')) +":"+QString("%1").arg(minutes, 2, 10, QChar('0'))+":"+QString("%1").arg(seconds, 2, 10, QChar('0')));
    }

    int seconds, minutes, hours;
    seconds = int(m_player->position())/1000LL;
    hours = seconds / 3600;
    minutes = (seconds - hours*3600) / 60;
    seconds = (seconds - hours*3600 - minutes*60);
    currentTime->setText(QString("%1").arg(hours, 2, 10, QChar('0')) +":"+QString("%1").arg(minutes, 2, 10, QChar('0'))+":"+QString("%1").arg(seconds, 2, 10, QChar('0')));


}


void PlayerWindow::rearCamera(){
    front = false;
    rear = true;
    both = false;
    b1->setStyleSheet("QPushButton { background-image: url(:/images/images/music_player/front_camera_hover.png);"
                      "border: none;} ");
    b2->setStyleSheet("QPushButton { background-image: url(:/images/images/music_player/rear_view_active.png);"
                      "border: none;} ");
    b3->setStyleSheet("QPushButton { background-image: url(:/images/images/music_player/both_camera_hover.png);"
                      "border: none;} ");
    m_player->setRenderer(m_vo2);

    m_player2->setRenderer(m_vo);
}

void PlayerWindow::frontCamera(){
    front = true;
    rear = false;
    both = false;
    b1->setStyleSheet("QPushButton { background-image: url(:/images/images/music_player/front_camera_active.png);"
                      "border: none;} ");
    b2->setStyleSheet("QPushButton { background-image: url(:/images/images/music_player/rear_view_hover.png);"
                      "border: none;} ");
    b3->setStyleSheet("QPushButton { background-image: url(:/images/images/music_player/both_camera_hover.png);"
                      "border: none;} ");
    m_player->setRenderer(m_vo);

    m_player2->setRenderer(m_vo2);
}

void PlayerWindow::bothCamera(){
    front = false;
    rear = false;
    both = true;
    b1->setStyleSheet("QPushButton { background-image: url(:/images/images/music_player/front_camera_hover.png);"
                      "border: none;} ");
    b2->setStyleSheet("QPushButton { background-image: url(:/images/images/music_player/rear_view_hover.png);"
                      "border: none;} ");
    b3->setStyleSheet("QPushButton { background-image: url(:/images/images/music_player/both_camera_active.png);"
                      "border: none;} ");
    m_player->setRenderer(m_vo);

    m_player2->setRenderer(m_vo2);
}

void PlayerWindow::forwardSeek(){
    m_player->seekForward();
    m_player2->seekForward();
}

void PlayerWindow::backSeek(){
    m_player->seekBackward();
    m_player2->seekBackward();
}

void PlayerWindow::hideMainWindow(){
    this->showMinimized();
}

void PlayerWindow::maxNormalView(){
    if (!isMaxSize){
        this->setFixedSize(1920,1112);
        isMaxSize = !isMaxSize;
        //top
        topPanel->setFixedSize(1920,32);
        topPanel->setStyleSheet("QWidget {background-image: url(:/images/images/top/top_bg2.png);}");
        title->setFixedWidth(1720);
        //m_vo
        m_vo->widget()->setFixedSize(1220, 780);//715+65=780

        //m_vo2
        m_vo2->widget()->setFixedSize(700,400);//365+35=400
        m_vo->setOutAspectRatio(1.5641);
        m_vo2->setOutAspectRatio(1.75);
        //container
        container->setFixedSize(700,385);//350+30 = 385
        mpPlayList->setFixedSize(700, 301);//366-65 = 301
        mpPlayListEvent->setFixedSize(700,301);
        sliderButtonsWidg->setFixedSize(1220, 114);
        graphSectionWidg->setFixedSize(1220, 126);


        bottomStack->setFixedSize(1220, 60);
        bottomStack->setCurrentIndex(1);


        plot->setFixedSize(880, 120);



        gSensorsWidg->setFixedWidth(940);
        strangeButtonsWidg->setFixedWidth(m_vo->widget()->width()-270);

    }else{
        this->setFixedSize(1000, 832);
        isMaxSize = !isMaxSize;
        //top
        topPanel->setFixedSize(1000,32);
        topPanel->setStyleSheet("QWidget {background-image: url(:/images/images/top/top_bg.png);}");
        title->setFixedWidth(800);
        //m_vo
        //m_vo->widget()->setFixedSize(620, 440);
        m_vo->widget()->setMinimumSize(620, 435);
        m_vo->widget()->setMaximumSize(1300, 715);
        //m_vo2
        m_vo2->widget()->setFixedSize(380,225);
        m_vo->setOutAspectRatio(1.4091);
        m_vo2->setOutAspectRatio(1.6889);
        container->setFixedSize(380,215);
        mpPlayList->setFixedSize(380, 366);
        mpPlayListEvent->setFixedSize(380,366);

        sliderButtonsWidg->setFixedSize(620,114);
        graphSectionWidg->setFixedSize(620, 126);
        bottomStack->setFixedSize(620, 125);
        bottomStack->setCurrentIndex(0);



        plot->setFixedSize(280, 120);


        gSensorsWidg->setFixedWidth(340);
        strangeButtonsWidg->setFixedWidth(m_vo->widget()->width()-270);

    }
}

