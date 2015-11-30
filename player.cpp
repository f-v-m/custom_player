
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


using namespace std;

void PlayerWindow::openMedia()
{
    QString file = QFileDialog::getOpenFileName(0, tr("Open a video"));
    if (file.isEmpty())
        return;


    play(file);


    //m_player->setSubtitleStream(3);
}

void PlayerWindow::seek(int pos)
{
    if (!m_player->isPlaying())
        return;
    m_player->seek(pos*1000LL); // to msecs
    if (!m_player2->isPlaying())
        return;
    m_player2->seek(pos*1000LL);
}

void PlayerWindow::playPause()
{

    using namespace std;
    if (!m_player->isPlaying()) {
        m_player->play();
        m_player2->play();
        return;
    }
    m_player->pause(!m_player->isPaused());
    m_player2->pause(!m_player2->isPaused());
    QMetaObject::invokeMethod(object, "calcRoute2", Qt::QueuedConnection);
    if (m_player->isPaused()){
        m_playBtn->setStyleSheet("QPushButton { border-image: url(:/images/images/music_player/play_normal.png);}");
    }else {
        m_playBtn->setStyleSheet("QPushButton { border-image: url(:/images/images/music_player/pause_hover_and_active.png);}");
    }


}

void PlayerWindow::changeBrightness(int br){
    double playerBr = double(br)/10;
    cout << playerBr << "brightness" << endl;
    m_vo->setBrightness(playerBr);
    m_vo2->setBrightness(playerBr);
}

void PlayerWindow::changeSpeed(int sp){
    double playSp = 2^sp;

    m_player->setSpeed(playSp);
    m_player2->setSpeed(playSp);
}

void PlayerWindow::changeVolume(int vol){
    double playerVol = vol;
    m_player->audio()->setVolume(playerVol);
}

void PlayerWindow::fulscreen(){
    if (!both){
        if (!isFullscreen){
            isFullscreen = !isFullscreen;
            this->showFullScreen();
            //rightVertWidg->hide();
            //sliderButtonsWidg->hide();
            graphSectionWidg->hide();
            bot_buttons->hide();
            m_vo2->widget()->hide();
            container->hide();
            mpPlayList->hide();

            m_vo->widget()->setFixedSize(this->width(), this->height() - 100);
            sliderButtonsWidg->setFixedWidth(this->width());
            vertLeft->setAlignment(Qt::AlignTop);

            b7->setStyleSheet("QPushButton { background-image: url(:/images/images/music_player/fullscreen_active.png);"
                              "border: none;} ");
        } else {
            isFullscreen = !isFullscreen;
            this->showNormal();
            //this->showMinimized();
            //this->showMaximized();

            m_vo->widget()->setFixedSize(674, 397);
            sliderButtonsWidg->setFixedWidth(674);
            //vertRight->setAlignment(Qt::AlignRight);
            vertRight->setAlignment(Qt::AlignTop);
            //rightVertWidg->show();
            sliderButtonsWidg->show();
            graphSectionWidg->show();
            bot_buttons->show();

            m_vo2->widget()->show();
            container->show();
            mpPlayList->show();

            b7->setStyleSheet("QPushButton { background-image: url(:/images/images/music_player/fullscreen_hover.png);"
                              "border: none;} ");
        }
    }else{
        if (!isFullscreen){
            isFullscreen = !isFullscreen;
            this->showFullScreen();
            //rightVertWidg->hide();
            //sliderButtonsWidg->hide();
            graphSectionWidg->hide();
            bot_buttons->hide();
            container->hide();
            mpPlayList->hide();



            m_vo2->widget()->setFixedSize((this->width())/2, this->height() - 200);
            m_vo2->widget()->move(this->width() /2, 0);
            //m_vo2->widget()->setFixedHeight(this->height() - 100);
            //sliderButtonsWidg->setFixedWidth((this->width())/2);
            vertLeft->setAlignment(Qt::AlignTop);
            //vertRight->setAlignment(Qt::AlignRight);
            vertRight->setAlignment(Qt::AlignTop);
            b7->setStyleSheet("QPushButton { background-image: url(:/images/images/music_player/fullscreen_active.png);"
                              "border: none;} ");
        } else {
            isFullscreen = !isFullscreen;


            //m_vo->widget()->setFixedSize(674, 397);
            //m_vo2->widget()->setFixedHeight(196);
            //sliderButtonsWidg->setFixedWidth(674);
            //rightVertWidg->show();
            //m_vo2->widget()->move(-this->width() /2, 0);

            m_vo2->widget()->setFixedHeight(196);
            m_vo2->widget()->setMinimumWidth(323);
            //sliderButtonsWidg->show();
            graphSectionWidg->show();
            bot_buttons->show();
            container->show();
            mpPlayList->show();
            this->showMinimized();
            vertRight->setAlignment(Qt::AlignTop);

            b7->setStyleSheet("QPushButton { background-image: url(:/images/images/music_player/fullscreen_hover.png);"
                              "border: none;} ");
        }
    }




}

void PlayerWindow::exitFullScreen(){
    this->showNormal();
    m_vo->widget()->setFixedSize(674, 397);
    rightVertWidg->show();
    sliderButtonsWidg->show();
    graphSectionWidg->show();
    bot_buttons->show();
}
