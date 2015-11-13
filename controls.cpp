

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


void PlayerWindow::updateSlider()
{

    m_slider->setRange(0, int(m_player->duration()/1000LL));
    m_slider->setValue(int(m_player->position()/1000LL));

    QString sub = mpSubtitle->m_s->getText();
    if (sub.length() > 0 ){
        //cout << sub.toStdString() << " subtitles origin" << endl;
        QStringList lst = QString(sub).split(" ", QString::SkipEmptyParts);
        cout << lst[0].toStdString() << " " << lst[1].toStdString() << " LOADED" << endl;


        if (tmpX != ""){
            if (isLoadRoute){
                QMetaObject::invokeMethod(object, "changeCoorinates",
                                          Q_ARG(QVariant, QVariant::fromValue(tmpX)),
                                          Q_ARG(QVariant, QVariant::fromValue(tmpY)),
                                          Q_ARG(QVariant, QVariant::fromValue(lst[0])),
                                          Q_ARG(QVariant, QVariant::fromValue(lst[1]))
                                          );
                QMetaObject::invokeMethod(object, "calcRoute", Qt::QueuedConnection);

            }
            isLoadRoute = !isLoadRoute;
        }

        tmpX = lst[0];
        tmpY = lst[1];
        QMetaObject::invokeMethod(object, "mapcenter",
                                  Q_ARG(QVariant, QVariant::fromValue(tmpX)),
                                  Q_ARG(QVariant, QVariant::fromValue(tmpY)));

    }
    else
        cout << "NOT LOADED" << endl;
}
