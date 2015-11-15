
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
    cout << file.toStdString() << " from open func" << endl;
    /*
    m_player->play(file);

    m_player2->play(file);
    m_player2->setVideoStream(1);
    */
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


//    QMetaObject::invokeMethod(object, "createMap", Qt::QueuedConnection);
    /*
    QMetaObject::invokeMethod(object, "changeCoorinates",
                              Q_ARG(QVariant, QVariant::fromValue(a)),
                              Q_ARG(QVariant, QVariant::fromValue(b)),
                              Q_ARG(QVariant, QVariant::fromValue(37.498571)),
                              Q_ARG(QVariant, QVariant::fromValue(126.985085))
                              );*/
    //QMetaObject::invokeMethod(object, "calcRoute", Qt::QueuedConnection);
    //QMetaObject::invokeMethod(object, "mapcenter", Qt::QueuedConnection);

    using namespace std;
    if (!m_player->isPlaying()) {
        m_player->play();
        m_player2->play();
        return;
    }
    m_player->pause(!m_player->isPaused());
    m_player2->pause(!m_player2->isPaused());
    QMetaObject::invokeMethod(object, "calcRoute2", Qt::QueuedConnection);
    //cout << m_player->subtitleStreamCount() << mpSubtitle->isEnabled() <<"| azaza" <<endl;
    //cout << mpSubtitle->m_s->getText().toStdString() << endl;


}

