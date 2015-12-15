
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

            graphSectionWidg->hide();
            bottomStack->hide();
            m_vo2->widget()->hide();
            container->hide();
            mpPlayList->hide();

            //this->showMaximized();

            //this->showFullScreen();
            m_vo->widget()->setFixedSize(this->width(), this->height()-110);
            sliderButtonsWidg->setFixedWidth(this->width());
            strangeButtonsWidg->setFixedWidth(m_vo->widget()->width()-270);

            b7->setStyleSheet("QPushButton { background-image: url(:/images/images/music_player/fullscreen_active.png);"
                              "border: none;} ");

        } else {
            isFullscreen = !isFullscreen;
            //this->showNormal();
            m_vo->widget()->setMinimumSize(620, 435);
            m_vo->widget()->setMaximumSize(1300, 900);
            sliderButtonsWidg->setMinimumWidth(620);
            sliderButtonsWidg->setMaximumWidth(1300);
            graphSectionWidg->show();
            bottomStack->show();
            m_vo2->widget()->show();
            container->show();
            mpPlayList->show();

            strangeButtonsWidg->setFixedWidth(m_vo->widget()->width()-270);
            b7->setStyleSheet("QPushButton { background-image: url(:/images/images/music_player/fullscreen_hover.png);"
                              "border: none;} ");
        }
    }
  /*  else{
        if (!isFullscreen){
            isFullscreen = !isFullscreen;

            graphSectionWidg->hide();
            bot_buttons->hide();
            //m_vo2->widget()->hide();
            container->hide();
            mpPlayList->hide();
            //this->showFullScreen();
            b7->setStyleSheet("QPushButton { background-image: url(:/images/images/music_player/fullscreen_active.png);"
                              "border: none;} ");

        } else {
            isFullscreen = !isFullscreen;

            graphSectionWidg->show();
            bot_buttons->show();
            //m_vo2->widget()->hide();
            container->show();
            mpPlayList->show();

            b7->setStyleSheet("QPushButton { background-image: url(:/images/images/music_player/fullscreen_hover.png);"
                              "border: none;} ");

        }
    }
*/



}

void PlayerWindow::exitFullScreen(){
    this->showNormal();
    m_vo->widget()->setFixedSize(674, 397);

    sliderButtonsWidg->show();
    graphSectionWidg->show();
    bot_buttons->show();
}

void PlayerWindow::nextFile(){
    m_player->stop();
    m_player2->stop();
    //play(mpPlayList->itemAt(rowForPlay+1).mUrl);
    rowForPlay+=1;
    mpPlayList->isClicked = false;
    //mpPlayList->rowIndex = rowForPlay;
    if (rowForPlay >= mpPlayList->rowsQuantity){
        rowForPlay = 0;
    }
    play(mpPlayList->itemAt(rowForPlay).url());

}
void PlayerWindow::prevFile(){
    m_player->stop();
    m_player2->stop();
    //play(mpPlayList->itemAt(rowForPlay+1).mUrl);
    rowForPlay-=1;
    mpPlayList->isClicked = false;
    //mpPlayList->rowIndex = rowForPlay;
    if (rowForPlay < 0){
        rowForPlay = mpPlayList->rowsQuantity -1;
    }
    play(mpPlayList->itemAt(rowForPlay).url());

}

void PlayerWindow::mirrorVert(){
    if (isMirrorVert){
        m_vo->setOrientation(0);
        isMirrorVert = !isMirrorVert;
        b4->setStyleSheet("QPushButton { background-image: url(:/images/images/music_player/invert_upside_down_hover.png);"
                          "border: none;} "
                                 "QPushButton:hover { background-image: url(:/images/images/music_player/invert_upside_down_normal.png);"
                          "border: none;}");
    }else{
        m_vo->setOrientation(-180);
        isMirrorVert = !isMirrorVert;
        b4->setStyleSheet("QPushButton { background-image: url(:/images/images/music_player/invert_upside_down_active.png);"
                          "border: none;} "
                                 "QPushButton:hover { background-image: url(:/images/images/music_player/invert_upside_down_normal.png);"
                          "border: none;}");
    }
}

void PlayerWindow::mirrorHor(){
    if (isMirrorHor){
        //m_vo->setOrientation(0);
        isMirrorHor = !isMirrorHor;
        b5->setStyleSheet("QPushButton { background-image: url(:/images/images/music_player/invert_left_To_right_hover.png);"
                          "border: none;} "
                                 "QPushButton:hover { background-image: url(:/images/images/music_player/invert_left_To_right_normal.png);"
                          "border: none;}");
    }else{

        isMirrorHor = !isMirrorHor;
        b5->setStyleSheet("QPushButton { background-image: url(:/images/images/music_player/invert_left_To_right_active.png);"
                          "border: none;} "
                                 "QPushButton:hover { background-image: url(:/images/images/music_player/invert_left_To_right_normal.png);"
                          "border: none;}");
    }
}

void PlayerWindow::zoomVideo(){
    if (!isZoomed){
        m_vo->setRegionOfInterest(480,270,960,540);
        isZoomed = !isZoomed;
        b6->setStyleSheet("QPushButton { background-image: url(:/images/images/music_player/crop_active.png);"
                          "border: none;} "
                                 "QPushButton:hover { background-image: url(:/images/images/music_player/crop_normal.png);"
                          "border: none;}");
    }else{
        m_vo->setRegionOfInterest(0,0,1920, 1080);
        isZoomed = !isZoomed;
        b6->setStyleSheet("QPushButton { background-image: url(:/images/images/music_player/crop_hover.png);"
                          "border: none;} "
                                 "QPushButton:hover { background-image: url(:/images/images/music_player/crop_normal.png);"
                          "border: none;}");

    }
}

