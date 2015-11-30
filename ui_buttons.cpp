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

            QMetaObject::invokeMethod(object, "mapcenter",
                                                      Q_ARG(QVariant, QVariant::fromValue(lst[0])),
                                                      Q_ARG(QVariant, QVariant::fromValue(lst[1]))
                                                      );
            cout << "FIRST" << endl;
        }else{
           // cout << tmpX.toStdString() << " " << tmpY.toStdString() << " temp" <<endl;
            //cout << lst[0].toStdString() << " " << lst[1].toStdString() << " novi dani" << endl;

            if (counter == 4){

                if (tmpX!=lst[0] && tmpY!=lst[1]){
                    if (isLoadRoute){
                        /*
                        QMetaObject::invokeMethod(object, "changeCoorinates",
                                                                  Q_ARG(QVariant, QVariant::fromValue(tmpX)),
                                                                  Q_ARG(QVariant, QVariant::fromValue(tmpY)),
                                                                  Q_ARG(QVariant, QVariant::fromValue(lst[0])),
                                                                  Q_ARG(QVariant, QVariant::fromValue(lst[1]))
                                                                  );
                        QMetaObject::invokeMethod(object, "calcRoute", Qt::QueuedConnection);
                        */
                        QMetaObject::invokeMethod(object, "createTag",
                                                                  Q_ARG(QVariant, QVariant::fromValue(lst[0])),
                                                                  Q_ARG(QVariant, QVariant::fromValue(lst[1]))
                                                                  );
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
            left_signal->setStyleSheet("QPushButton { border-image: url(:/images/images/bottom_buttons/left_signal_normal.png);} ");
            right_signal->setStyleSheet("QPushButton { border-image: url(:/images/images/bottom_buttons/right_signal_normal.png);} ");
        }else if (lst[15].toInt() == 1){
            right_signal->setStyleSheet("QPushButton { border-image: url(:/images/images/bottom_buttons/right_signal_active.png);} ");
            left_signal->setStyleSheet("QPushButton { border-image: url(:/images/images/bottom_buttons/left_signal_normal.png);} ");
        }else if (lst[15].toInt() == 2){
            left_signal->setStyleSheet("QPushButton { border-image: url(:/images/images/bottom_buttons/left_signal_active.png);} ");
            right_signal->setStyleSheet("QPushButton { border-image: url(:/images/images/bottom_buttons/right_signal_normal.png);} ");
        }

        //BREAK
        if (lst[16].toInt() == 0){
            brake->setStyleSheet("QPushButton { border-image: url(:/images/images/bottom_buttons/brake_normal.png);} ");

        }else if (lst[16].toInt() == 1){
            brake->setStyleSheet("QPushButton { border-image: url(:/images/images/bottom_buttons/brake_active.png);} ");

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

        //graph:

        //*pointsX << QPointF( 1 , lst[3].toInt() );
        const double v = double (x);
        *pointsX << QPointF( v , lst[3].toInt() );
        curveX->setSamples(*pointsX);

        const double v2 = double (y);
        *pointsY << QPointF( v2 , lst[4].toInt() );
        curveY->setSamples(*pointsY);

        const double v3 = double (z);
        *pointsZ << QPointF( v3 , lst[5].toInt() );
        curveZ->setSamples(*pointsZ);

        plot->replot();
        plot->setAxisScale(plot->xBottom, x-10, x, 0);

        x += 1;
        y += 1;
        z += 1;
        gSensorX->setText(" X: "+lst[3]);
        gSensorY->setText(" Y: "+lst[4]);
        gSensorZ->setText(" Z: "+lst[5]);


        //gear position:

        if (lst[20] == "0"){
            pLabel->setStyleSheet("QLabel {color: green;}");
            rLabel->setStyleSheet("QLabel {color: white;}");
            nLabel->setStyleSheet("QLabel {color: white;}");
            dLabel->setStyleSheet("QLabel {color: white;}");
        }else if (lst[20] == "1"){
            rLabel->setStyleSheet("QLabel {color: green;}");
            pLabel->setStyleSheet("QLabel {color: white;}");
            nLabel->setStyleSheet("QLabel {color: white;}");
            dLabel->setStyleSheet("QLabel {color: white;}");
        }else if (lst[20] == "2"){
            nLabel->setStyleSheet("QLabel {color: green;}");
            pLabel->setStyleSheet("QLabel {color: white;}");
            rLabel->setStyleSheet("QLabel {color: white;}");
            dLabel->setStyleSheet("QLabel {color: white;}");
        }else if (lst[20] == "3"){
            dLabel->setStyleSheet("QLabel {color: green;}");
            pLabel->setStyleSheet("QLabel {color: white;}");
            rLabel->setStyleSheet("QLabel {color: white;}");
            nLabel->setStyleSheet("QLabel {color: white;}");
        }


    }
    else{
        cout << "NOT LOADED" << endl;

    }
}
