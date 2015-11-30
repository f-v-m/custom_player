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
#include <QLabel>

#include <qwt_plot.h>
#include <qwt_plot_grid.h>

#include <qwt_legend.h>

#include <qwt_plot_curve.h>
#include <qwt_symbol.h>

#include <qwt_plot_magnifier.h>

#include <qwt_plot_panner.h>

#include <qwt_plot_picker.h>
#include <qwt_picker_machine.h>


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
/*
    QPalette palette;
    QColor oldBackColor = palette.color( QPalette::Background );
    palette.setColor(QPalette::Background, QColor::fromRgb(8,8,8));
    this->setPalette(palette);
*/
    this->setStyleSheet("QWidget {background-image: url(:/images/images/bg/bg.png))}");
    //this->layout()->setMargin(0);

    setWindowTitle(QString::fromLatin1("ADAS ONE Viewer"));
    setWindowIcon(QIcon(":/images/images/top/logo.png"));

    initMyPlayer();
    initMySubtitles();
    initButtons();
    setButtonsStyle();


    QHBoxLayout *mainLayout = new QHBoxLayout();
    setLayout (mainLayout);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);



    initMainVertWidgets();

    initSliderButtonsWidgets();
    m_vo = new VideoOutput(this);
    m_vo2 = new VideoOutput(this);

    if (!m_vo->widget()) {
        QMessageBox::warning(0, QString::fromLatin1("QtAV error"), tr("Can not create video renderer"));
        return;
    }
    m_player->setRenderer(m_vo);
    m_player2->setRenderer(m_vo2);

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


    connect(m_player, SIGNAL(positionChanged(qint64)), SLOT(parseSub()));
    //______

    //_______PLAYLIST_____
    initPlaylistWidget();

    connect(mpPlayList, SIGNAL(aboutToPlay(QString)), SLOT(play(QString)));




    mainLayout->addWidget(leftVertWidg);
    mainLayout->addWidget(rightVertWidg);
    vertLeft->addWidget(m_vo->widget());
    vertRight->addWidget(m_vo2->widget());
    //m_vo->widget()->setStyleSheet("QWidget {background: transparent}");
    m_vo->widget()->setContentsMargins(0,0,0,0);
    m_vo->widget()->setStyleSheet("QWidget { background-image: url(:/images/images/main_camera/main_camera_box.png); }");
    //hor->addWidget(container);

    setWidgetsSize();
    vertLeft->addWidget(sliderButtonsWidg);

    vertRight->addWidget(container);

    vertRight->addWidget(mpPlayList);


    //m_slider->setStyleSheet("QSlider { background-color: rgb(18, 18, 18); }");
    connect(m_slider, SIGNAL(sliderMoved(int)), SLOT(seek(int)));
    connect(mpPlayList, SIGNAL(clicked(QString,QString)), SLOT(onPlayListClick(QString,QString)));

    connect(m_player, SIGNAL(positionChanged(qint64)), SLOT(updateSlider()));
    connect(m_player, SIGNAL(started()), SLOT(updateSlider()));





    //speed/sliders/coords:


    initGraphSectionWidgets();
    vertLeft->addWidget(graphSectionWidg);

    //DESIGN OF BOTTOM BUTTONS:
    initBottomButtonsWidget();

    vertLeft->addWidget(bot_buttons);


    connect(m_playBtn, SIGNAL(clicked()), SLOT(playPause()));
    connect(m_stopBtn, SIGNAL(clicked()), m_player, SLOT(stop()));
    connect(m_stopBtn, SIGNAL(clicked()), m_player2, SLOT(stop()));
    connect(b2, SIGNAL(clicked()), SLOT(rearCamera()));
    connect(forward_button, SIGNAL(clicked()), SLOT(forwardSeek()));
    connect(back_button, SIGNAL(clicked()), SLOT(backSeek()));
    connect(play_speed_slider, SIGNAL(sliderMoved(int)), SLOT(changeSpeed(int)));
    connect(brightness_slider, SIGNAL(sliderMoved(int)), SLOT(changeBrightness(int)));
    connect(volume_slider, SIGNAL(sliderMoved(int)), SLOT(changeVolume(int)));
    //connect(new_button, SIGNAL(clicked()), SLOT(azaza()));



    //mpPlayList->set
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

    mFile = name;

    mTitle = mFile;

    if (!mFile.contains(QLatin1String("://")) || mFile.startsWith(QLatin1String("file://"))) {
        mTitle = QFileInfo(mFile).fileName();
    }

    setWindowTitle(mTitle);

    PlayListItem item;
    item.setUrl(mFile);
    item.setTitle(mTitle);
    item.setLastTime(0);

    //m_vo->setOrientation(180);

    m_player->play(name);
    m_player2->play(name);
    m_player2->setVideoStream(1);
    m_player2->audio()->isMute();



    //totalTime->setText(QString::number(m_player->duration()));
    m_playBtn->setStyleSheet("QPushButton { border-image: url(:/images/images/music_player/pause_hover_and_active.png);}");

    currentTime->setText("00:00:00");
    totalTime->setText("00:00:00");



}
