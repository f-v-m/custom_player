/******************************************************************************
    Simple Player:  this file is part of QtAV examples
    Copyright (C) 2012-2015 Wang Bin <wbsecg1@gmail.com>

*   This file is part of QtAV

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

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



using namespace QtAV;
using namespace std;


static bool parseArgs(QStringList& args, QVariantMap& parameters)
{

    while (!args.isEmpty()) {

        QString param = args.takeFirst();

        if (param.startsWith("--help")) {
            QTextStream out(stdout);
            out << "Usage: " << endl;
            out << "--plugin.<parameter_name> <parameter_value>    -  Sets parameter = value for plugin" << endl;
            out.flush();
            return true;
        }

        if (param.startsWith("--plugin.")) {

            param.remove(0, 9);

            if (args.isEmpty() || args.first().startsWith("--")) {
                parameters[param] = true;
            } else {

                QString value = args.takeFirst();

                if (value == "true" || value == "on" || value == "enabled") {
                    parameters[param] = true;
                } else if (value == "false" || value == "off"
                           || value == "disable") {
                    parameters[param] = false;
                } else {
                    parameters[param] = value;
                }
            }
        }
    }
    return false;
}

PlayerWindow::PlayerWindow(QWidget *parent) : QWidget(parent)
  , mpSubtitle(0)
{

    QPalette palette;
    QColor oldBackColor = palette.color( QPalette::Background );
    palette.setColor(QPalette::Background, Qt::black);
    this->setPalette(palette);

    setWindowTitle(QString::fromLatin1("Player"));
    setWindowIcon(QIcon("E:/workspace/QtAV/examples/simpleplayerimages/top/logo.png"));
    initMyPlayer();
    initMySubtitles();




    QHBoxLayout *mainLayout = new QHBoxLayout();
    setLayout (mainLayout);
    QVBoxLayout *vertLeft = new QVBoxLayout();
    QVBoxLayout *vertRight = new QVBoxLayout();

    QWidget *widg = new QWidget();
    widg->setStyleSheet("QWidget { background-image: url(E:/workspace/QtAV/examples/simpleplayer/images/music_player/player_background.png); }");
    widg->setMaximumHeight(35);
    QHBoxLayout *hb = new QHBoxLayout();
    widg->setLayout(hb);
    m_vo = new VideoOutput(this);
    m_vo2 = new VideoOutput(this);

    if (!m_vo->widget()) {
        QMessageBox::warning(0, QString::fromLatin1("QtAV error"), tr("Can not create video renderer"));
        return;
    }
    m_player->setRenderer(m_vo);
    m_player2->setRenderer(m_vo2);
    mpSubtitle->installTo(m_vo);

    //_______MAP

    QVariantMap parameters;
    QStringList args(QCoreApplication::arguments());

    if (parseArgs(args, parameters)) exit(0);

    const QString mainQmlApp = QLatin1String("qrc:///mapviewer.qml");
    QQuickView *view = new QQuickView();

    view->engine()->addImportPath(QLatin1String(":/imports"));

    view->rootContext()->setContextProperty("appDirPath", QCoreApplication::applicationDirPath());
    view->setSource(QUrl(mainQmlApp));




    view->setResizeMode(QQuickView::SizeRootObjectToView);

    object = view->rootObject();

    QMetaObject::invokeMethod(object, "setPluginParameters", Q_ARG(QVariant, QVariant::fromValue(parameters)));

    QObject::connect(view->engine(), SIGNAL(quit()), qApp, SLOT(quit()));
    view->setGeometry(QRect(100, 100, 400, 300));
    container = QWidget::createWindowContainer(view, this);

    //______

    //_______PLAYLIST_____
    mpPlayList = new PlayList(this);
    //mpPlayList->setSaveFile(Config::instance().defaultDir() + QString::fromLatin1("/playlist.qds"));
    mpPlayList->load();

    connect(mpPlayList, SIGNAL(aboutToPlay(QString)), SLOT(play(QString)));



    QWidget *leftVertWidg = new QWidget();
    QWidget *rightVertWidg = new QWidget();

    leftVertWidg->setStyleSheet("QWidget { background-image: url(E:/workspace/QtAV/examples/simpleplayer/images/bg/sep.png); }");
    rightVertWidg->setStyleSheet("QWidget { background-image: url(E:/workspace/QtAV/examples/simpleplayer/images/bg/sep.png); }");

    leftVertWidg->setLayout(vertLeft);
    rightVertWidg->setLayout(vertRight);
    mainLayout->addWidget(leftVertWidg);
    mainLayout->addWidget(rightVertWidg);
    vertLeft->addWidget(m_vo->widget());
    vertRight->addWidget(m_vo2->widget());
    //hor->addWidget(container);

    setWidgetsSize();


    vertRight->addWidget(container);

    vertRight->addWidget(mpPlayList);

    m_slider = new QSlider();
    m_slider->setOrientation(Qt::Horizontal);
    m_slider->setStyleSheet("QSlider { background-image: url(E:/workspace/QtAV/examples/simpleplayer/images/music_player/player_background.png); }");
    connect(m_slider, SIGNAL(sliderMoved(int)), SLOT(seek(int)));
    connect(mpPlayList, SIGNAL(clicked(QString,QString)), SLOT(onPlayListClick(QString,QString)));

    connect(m_player, SIGNAL(positionChanged(qint64)), SLOT(updateSlider()));
    connect(m_player, SIGNAL(started()), SLOT(updateSlider()));
    m_slider->setMinimumHeight(32);
    m_slider->setStyleSheet("QSlider::handle:horizontal {image: url(E:/workspace/QtAV/examples/simpleplayer/images/graph_section/small_player_handle.png);"
                            "width: 30px; "
                            "margin: -8px,0 ;"
                            "height: 30px; }");


    vertLeft->addWidget(m_slider);
    vertLeft->addWidget(widg);




    initButtons();
    setButtonsStyle();



    hb->addWidget(prev_button);
    hb->addWidget(back_button);
    hb->addWidget(m_playBtn);
    hb->addWidget(forward_button);
    hb->addWidget(next_button);
    hb->addWidget(m_stopBtn);
    hb->addWidget(b1);
    hb->addWidget(b2);
    hb->addWidget(b3);
    hb->addWidget(b4);
    hb->addWidget(b5);
    hb->addWidget(b6);
    hb->addWidget(b7);


    //DESIGN OF BOTTOM BUTTONS:
    QWidget *bot_buttons = new QWidget();
    bot_buttons->setStyleSheet("QWidget { background-image: url(E:/workspace/QtAV/examples/simpleplayer/images/graf_section/graph_gray_bg.png); }");
    bot_buttons->setMaximumHeight(150);
    QHBoxLayout *bottom_buttons = new QHBoxLayout();
    bot_buttons->setLayout(bottom_buttons);



    vertLeft->addWidget(bot_buttons);

    QVBoxLayout *bot1 = new QVBoxLayout();
    QVBoxLayout *bot2 = new QVBoxLayout();
    QVBoxLayout *bot3 = new QVBoxLayout();
    QVBoxLayout *bot4 = new QVBoxLayout();
    QVBoxLayout *bot5 = new QVBoxLayout();

    QWidget *botW1 = new QWidget();
    botW1->setStyleSheet("QWidget { background-image: url(E:/workspace/QtAV/examples/simpleplayer/images/graf_section/graph_gray_bg.png); }");
    botW1->setMaximumHeight(130);
    botW1->setLayout(bot1);

    QWidget *botW2 = new QWidget();
    botW2->setStyleSheet("QWidget { background-image: url(E:/workspace/QtAV/examples/simpleplayer/images/graf_section/graph_gray_bg.png); }");
    botW2->setMaximumHeight(130);
    botW2->setLayout(bot2);

    QWidget *botW3 = new QWidget();
    botW3->setStyleSheet("QWidget { background-image: url(E:/workspace/QtAV/examples/simpleplayer/images/graf_section/graph_gray_bg.png); }");
    botW3->setMaximumHeight(130);
    botW3->setLayout(bot3);
    QWidget *botW4 = new QWidget();
    botW4->setStyleSheet("QWidget { background-image: url(E:/workspace/QtAV/examples/simpleplayer/images/graf_section/graph_gray_bg.png); }");
    botW4->setMaximumHeight(130);
    botW4->setLayout(bot4);
    QWidget *botW5 = new QWidget();
    botW5->setStyleSheet("QWidget { background-image: url(E:/workspace/QtAV/examples/simpleplayer/images/graf_section/graph_gray_bg.png); }");
    botW5->setMaximumHeight(130);
    botW5->setLayout(bot5);

    bottom_buttons->addWidget(botW1);
    bottom_buttons->addWidget(botW2);
    bottom_buttons->addWidget(botW3);
    bottom_buttons->addWidget(botW4);
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




    vertRight->addWidget(m_openBtn);



    connect(m_openBtn, SIGNAL(clicked()), SLOT(openMedia()));
    connect(m_playBtn, SIGNAL(clicked()), SLOT(playPause()));
    connect(m_stopBtn, SIGNAL(clicked()), m_player, SLOT(stop()));
    connect(m_stopBtn, SIGNAL(clicked()), m_player2, SLOT(stop()));
    //connect(new_button, SIGNAL(clicked()), SLOT(azaza()));



}



void PlayerWindow::azaza(){

    QMetaObject::invokeMethod(object, "mapcenter", Qt::QueuedConnection);
}


void PlayerWindow::initMyPlayer(){
    m_player = new AVPlayer(this);
    m_player2 = new AVPlayer(this);
    mpSubtitle = new SubtitleFilter(this);
}

void PlayerWindow::onPlayListClick(const QString &key, const QString &value)
{
    cout<< value.toStdString() << " from playlist" << endl;

    play(value);
}

void PlayerWindow::play(const QString &name)
{

    cout << name.toStdString() << " from play func" << endl;
    mFile = name;
    cout << mFile.toStdString() << " from play func" << endl;
    /*
    if (!mIsReady) {
        mHasPendingPlay = true;
        return;
    }*/
    mTitle = mFile;
    cout << 1;
    if (!mFile.contains(QLatin1String("://")) || mFile.startsWith(QLatin1String("file://"))) {
        mTitle = QFileInfo(mFile).fileName();
    }
    cout << 2;
    setWindowTitle(mTitle);
    cout << 3;
    PlayListItem item;
    item.setUrl(mFile);
    item.setTitle(mTitle);
    item.setLastTime(0);
    cout << 4;
    //mpHistory->remove(mFile);
    //mpHistory->insertItemAt(item, 0);
    cout << name.toStdString() << " from play func" << endl;
    m_player->play(name);
    m_player2->play(name);
    m_player2->setVideoStream(1);




    //m_player2->play(name);
}



