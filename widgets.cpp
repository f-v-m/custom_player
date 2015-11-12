
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

void PlayerWindow::setWidgetsSize(){
    mpPlayList->setMaximumHeight(200);
    mpPlayList->setMinimumWidth(340);
    m_vo->widget()->setMinimumSize(640, 360);
    m_vo->widget()->setMaximumSize(1120, 630);
    m_vo2->widget()->setMinimumSize(320, 180);
    container->setMinimumSize(320, 180);
    //container->setMaximumSize(400, 200);
    container->setFocusPolicy(Qt::TabFocus);

    m_vo2->widget()->setMinimumSize(320, 180);
}


