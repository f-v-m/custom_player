

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
    if (m_player->isPlaying()){
        if (rearCamIsEnabled){
            m_player2->stop();

            rearCamIsEnabled = !rearCamIsEnabled;
        }else {

            m_player2->setStartPosition(m_player->position());

            m_player2->play();
            //m_player2->seek(m_player->position());
            rearCamIsEnabled = !rearCamIsEnabled;
        }
    }
}

void PlayerWindow::forwardSeek(){
    m_player->seekForward();
    m_player2->seekForward();
}

void PlayerWindow::backSeek(){
    m_player->seekBackward();
    m_player2->seekBackward();
}

