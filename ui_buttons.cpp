#include "playerwindow.h"
#include <QPushButton>
#include <QSlider>
#include <QLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <iostream>
#include <sstream>
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

void PlayerWindow::parseSub(){

    sub = mpSubtitle->m_s->getText();
    if (sub.length() > 0 ){
        cout << sub.toStdString() << " subtitles origin" << endl;
        QStringList lst = QString(sub).split(" ", QString::SkipEmptyParts);
        cout << lst[0].toStdString() << " " << lst[1].toStdString() << " LOADED" << endl;

        if (isFirstSubt){
            tmpX = lst[0];
            tmpY = lst[1];
            isFirstSubt = false;
            cout << "FIRST" << endl;
        }else{
           // cout << tmpX.toStdString() << " " << tmpY.toStdString() << " temp" <<endl;
            //cout << lst[0].toStdString() << " " << lst[1].toStdString() << " novi dani" << endl;

            if (counter == 4){

                if (tmpX!=lst[0] && tmpY!=lst[1]){
                    if (isLoadRoute){
                        QMetaObject::invokeMethod(object, "changeCoorinates",
                                                                  Q_ARG(QVariant, QVariant::fromValue(tmpX)),
                                                                  Q_ARG(QVariant, QVariant::fromValue(tmpY)),
                                                                  Q_ARG(QVariant, QVariant::fromValue(lst[0])),
                                                                  Q_ARG(QVariant, QVariant::fromValue(lst[1]))
                                                                  );
                        QMetaObject::invokeMethod(object, "calcRoute", Qt::QueuedConnection);
                        cout << "gotovo" << endl;
                    }
                    isLoadRoute = !isLoadRoute;
                }
                else{
                    cout << "FAIL" << endl;
                }

            }
            counter = counter - 1;
            if (counter == 0)
                counter = 4;
        }

//LEFT RIGHT
        if (lst[15].toInt() == 0){
            left_signal->setStyleSheet("QPushButton { border-image: url(E:/workspace/QtAV/examples/simpleplayer/images/bottom_buttons/left_signal_normal.png);} ");
            right_signal->setStyleSheet("QPushButton { border-image: url(E:/workspace/QtAV/examples/simpleplayer/images/bottom_buttons/right_signal_normal.png);} ");
        }else if (lst[15].toInt() == 1){
            right_signal->setStyleSheet("QPushButton { border-image: url(E:/workspace/QtAV/examples/simpleplayer/images/bottom_buttons/right_signal_active.png);} ");
            left_signal->setStyleSheet("QPushButton { border-image: url(E:/workspace/QtAV/examples/simpleplayer/images/bottom_buttons/left_signal_normal.png);} ");
        }else if (lst[15].toInt() == 2){
            left_signal->setStyleSheet("QPushButton { border-image: url(E:/workspace/QtAV/examples/simpleplayer/images/bottom_buttons/left_signal_active.png);} ");
            right_signal->setStyleSheet("QPushButton { border-image: url(E:/workspace/QtAV/examples/simpleplayer/images/bottom_buttons/right_signal_normal.png);} ");
        }

        //BREAK
        if (lst[16].toInt() == 0){
            brake->setStyleSheet("QPushButton { border-image: url(E:/workspace/QtAV/examples/simpleplayer/images/bottom_buttons/brake_normal.png);} ");

        }else if (lst[16].toInt() == 1){
            brake->setStyleSheet("QPushButton { border-image: url(E:/workspace/QtAV/examples/simpleplayer/images/bottom_buttons/brake_active.png);} ");

        }
        //Steering angle:
        wheel_angle->setText("   "+lst[22].remove(0, 2)+"°");

        //CURRENT SPEED:
        speedButton->setText(lst[26].remove(0, 2)+" km/h");

        //battery:
        voltage->setText("   "+lst[40]+" V");
        //fuel:
        patrol->setText("   "+lst[41]+" %");

        //speed progress:
        speedProgress->setValue(lst[26].toInt());
    }
    else
        cout << "NOT LOADED" << endl;
}
