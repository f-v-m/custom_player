
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
    palette.setColor(QPalette::Background, QColor::fromRgb(18,18,18));
    this->setPalette(palette);

    setWindowTitle(QString::fromLatin1("Viewer"));
    setWindowIcon(QIcon("E:/workspace/QtAV/examples/simpleplayerimages/top/logo.png"));
    initMyPlayer();
    initMySubtitles();



    QHBoxLayout *mainLayout = new QHBoxLayout();
    setLayout (mainLayout);
    mainLayout->setContentsMargins(2, 2, 2, 2);

    QVBoxLayout *vertLeft = new QVBoxLayout();
    QVBoxLayout *vertRight = new QVBoxLayout();

    QWidget *widg = new QWidget();
    widg->setStyleSheet("QWidget { background-color: rgb(18, 18, 18); }");

    QHBoxLayout *hb = new QHBoxLayout();
    hb->setContentsMargins(0,0,0,0);
    widg->setMaximumHeight(20);
    widg->setMaximumWidth(1000);
    widg->setLayout(hb);
    m_vo = new VideoOutput(this);
    m_vo2 = new VideoOutput(this);

    if (!m_vo->widget()) {
        QMessageBox::warning(0, QString::fromLatin1("QtAV error"), tr("Can not create video renderer"));
        return;
    }
    m_player->setRenderer(m_vo);
    m_player2->setRenderer(m_vo2);
    //mpSubtitle->installTo(m_vo);

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
    mpPlayList = new PlayList(this);
    //mpPlayList->setSaveFile(Config::instance().defaultDir() + QString::fromLatin1("/playlist.qds"));
    mpPlayList->load();

    connect(mpPlayList, SIGNAL(aboutToPlay(QString)), SLOT(play(QString)));



    QWidget *leftVertWidg = new QWidget();
    QWidget *rightVertWidg = new QWidget();

    leftVertWidg->setStyleSheet("QWidget { background-color: rgb(18, 18, 18); }");
    rightVertWidg->setStyleSheet("QWidget { background-color: rgb(18, 18, 18); }");

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
    m_slider->setContentsMargins(0,0,0,0);
    //m_slider->setStyleSheet("QSlider { background-color: rgb(18, 18, 18); }");
    connect(m_slider, SIGNAL(sliderMoved(int)), SLOT(seek(int)));
    connect(mpPlayList, SIGNAL(clicked(QString,QString)), SLOT(onPlayListClick(QString,QString)));

    connect(m_player, SIGNAL(positionChanged(qint64)), SLOT(updateSlider()));
    connect(m_player, SIGNAL(started()), SLOT(updateSlider()));


    m_slider->setMinimumHeight(15);
    m_slider->setStyleSheet("QSlider::handle:horizontal {image: url(E:/workspace/QtAV/examples/simpleplayer/images/graph_section/small_player_handle.png);"
                            "width: 30px; "
                            "margin: -8px,0 ;"
                            "height: 30px; }");
    m_slider->setMaximumWidth(1000);


    vertLeft->addWidget(m_slider);
    vertLeft->addWidget(widg);

//LINE WITH SPEED
    QWidget *lineWithSpeedWidg = new QWidget();
    lineWithSpeedWidg->setStyleSheet("QWidget { background-color: rgb(18, 18, 18); }");
    lineWithSpeedWidg->setMaximumHeight(35);
    QHBoxLayout *layerWithSpeed = new QHBoxLayout();
    //lineWithSpeedWidg->setMinimumHeight(20);
    //lineWithSpeedWidg->setMaximumWidth(1000);
    lineWithSpeedWidg->setLayout(layerWithSpeed);
    lineWithSpeedWidg->setMinimumHeight(55);
    vertLeft->addWidget(lineWithSpeedWidg);
    layerWithSpeed->setAlignment(Qt::AlignLeft);





    initButtons();
    setButtonsStyle();

    layerWithSpeed->addWidget(speedButton);

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
    bot_buttons->setStyleSheet("QWidget { background-image: url(E:/workspace/QtAV/examples/simpleplayer/images/buttons_bg.png); }");
    bot_buttons->setMaximumHeight(140);
    QHBoxLayout *bottom_buttons = new QHBoxLayout();
    bottom_buttons->setContentsMargins(0,0,0,0);
    bot_buttons->setLayout(bottom_buttons);
    bot_buttons->setMaximumWidth(1000);




    vertLeft->addWidget(bot_buttons);

    QVBoxLayout *bot1 = new QVBoxLayout();
    QVBoxLayout *bot2 = new QVBoxLayout();
    QVBoxLayout *bot3 = new QVBoxLayout();
    QVBoxLayout *bot4 = new QVBoxLayout();
    QVBoxLayout *bot5 = new QVBoxLayout();
    bot1->setContentsMargins(0,0,0,0);
    bot2->setContentsMargins(0,0,0,0);
    bot3->setContentsMargins(0,0,0,0);
    bot4->setContentsMargins(0,0,0,0);
    bot5->setContentsMargins(0,0,0,0);

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





    connect(m_playBtn, SIGNAL(clicked()), SLOT(playPause()));
    connect(m_stopBtn, SIGNAL(clicked()), m_player, SLOT(stop()));
    connect(m_stopBtn, SIGNAL(clicked()), m_player2, SLOT(stop()));
    connect(b2, SIGNAL(clicked()), SLOT(rearCamera()));
    connect(forward_button, SIGNAL(clicked()), SLOT(forwardSeek()));
    connect(back_button, SIGNAL(clicked()), SLOT(backSeek()));
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



    m_playBtn->setStyleSheet("QPushButton { border-image: url(E:/workspace/QtAV/examples/simpleplayer/images/music_player/pause_hover_and_active.png);}");

    //m_player2->play(name);
}



