
#ifndef PLAYERWINDOW_H
#define PLAYERWINDOW_H

#include <QWidget>
#include <QtAV>
#include <QUrl>
#include <QComboBox>
#include <QtQuick/QQuickItem>
#include <QLabel>
#include <QProgressBar>
#include <QLayout>
#include <QQuickView>
#include <qwt_plot.h>
#include <qwt_plot_grid.h>

#include <qwt_legend.h>

#include <qwt_plot_curve.h>
#include <qwt_symbol.h>

#include <qwt_plot_magnifier.h>

#include <qwt_plot_panner.h>

#include <qwt_plot_picker.h>
#include <qwt_picker_machine.h>
#include <QStackedWidget>

#include <QMouseEvent>
namespace QtAV {
class SubtitleFilter;
}
class AVFilterSubtitle;
class QSlider;
class QPushButton;
class PlayList;



class PlayerWindow : public QWidget
{
    Q_OBJECT
public:
    explicit PlayerWindow(QWidget *parent = 0);
    void mousePressEvent(QMouseEvent *event)
        {
            // Запоминаем позицию при нажатии кнопки мыши
            mpos = event->pos();
        }

        void mouseMoveEvent(QMouseEvent *event)
        {
            if (mpos.x() >= 0 && event->buttons() && Qt::LeftButton)
            {
                QPoint diff = event->pos() - mpos;
                QPoint newpos = this->pos() + diff;

                this->move(newpos);
            }
        }

        void mouseReleaseEvent(QMouseEvent *)
        {
            // Очищаем старое значение позиции нажатия мыши
            mpos = QPoint(-1, -1);
        }
public Q_SLOTS:
    void openMedia();
    void seek(int);
    void playPause();
    void azaza();
private Q_SLOTS:
    void updateSlider();
//new
private slots:
    void toggoleSubtitleEnabled(bool value);
    void toggleSubtitleAutoLoad(bool value);
    void openSubtitle();
    void setSubtitleCharset(const QString& charSet);
    void setSubtitleEngine(const QString& value);
    void onPlayListClick(const QString& key, const QString& value);
    void setWidgetsSize();
    void initButtons();
    void changeButtonSize(QPushButton *button);
    void changeButtonSize(QPushButton *button, int w, int h);
    void setButtonsStyle();
    void play(const QString &name);

    void parseSub();
    void rearCamera();
    void frontCamera();
    void bothCamera();

    void forwardSeek();
    void backSeek();

    //WIDGETS:

    void initSliderButtonsWidgets();
    void initGraphSectionWidgets();
    void initBottomButtonsWidget();
    void initPlaylistWidget();

    void setSliderStyleSheet();
    void setSmallSliders();
    void initGraph();
    void changeBrightness(int br);
    void changeSpeed(int sp);
    void changeVolume(int vol);
    void fulscreen();
    void exitFullScreen();
    void nextFile();
    void prevFile();


    void setGridLay();
    void mirrorVert();
    void mirrorHor();
    void zoomVideo();

    void setTopPanel();
    void hideMainWindow();
    void maxNormalView();
    void initBottomButtons2();


signals:
    void ready();

private:
    QPoint mpos;
    bool mIsReady, mHasPendingPlay;
    QtAV::VideoOutput *m_vo;
    QtAV::AVPlayer *m_player;
    QtAV::VideoOutput *m_vo2;
    QtAV::AVPlayer *m_player2;
    const QtAV::Packet pkt;
    QSlider *m_slider;
    QSlider *play_speed_slider;
    QSlider *brightness_slider;
    QSlider *volume_slider;
    QWidget *container;
    //_______________BUTTONS:

    QPushButton *m_playBtn;
    QPushButton *m_stopBtn;

    QPushButton *next_button;
    QPushButton *prev_button;
    QPushButton *back_button;
    QPushButton *forward_button;

    QPushButton *b1;
    QPushButton *b2;
    QPushButton *b3;
    QPushButton *b4;
    QPushButton *b5;
    QPushButton *b6;
    QPushButton *b7;

    QPushButton *obdi;
    QPushButton *left_signal;
    QPushButton *wheel_angle;
    QPushButton *brake;
    QPushButton *temperature;
    QPushButton *accelerate;
    QPushButton *voltage;
    QPushButton *right_signal;
    QPushButton *patrol;
    QPushButton *obdi2;
    QPushButton *left_signal2;
    QPushButton *wheel_angle2;
    QPushButton *brake2;
    QPushButton *temperature2;
    QPushButton *accelerate2;
    QPushButton *voltage2;
    QPushButton *right_signal2;
    QPushButton *patrol2;
    QLabel *speedButton;
    QPushButton *speedSliderLabel;
    QPushButton *brightnessSliderLabel;
    QPushButton *volumeSliderLabel;

    QStackedWidget *mapStack;
    QLabel *gSensorX;
    QLabel *gSensorY;
    QLabel *gSensorZ;

    QLabel *pLabel;
    QLabel *rLabel;
    QLabel *nLabel;
    QLabel *dLabel;
    QLabel *mLabel;

    QQuickView *view;
    QLabel *currentTime;
    QLabel *totalTime;

    QProgressBar *speedProgress;

    QTextItem *txt1;
    QtAV::SubtitleFilter *mpSubtitle;
    QComboBox *box = new QComboBox();
    PlayList *mpPlayList, *mpHistory, *mpPlayListEvent;

    void initMyPlayer();
    void initMySubtitles();

    QQuickItem *object;

    QString mFile;
    QString mTitle;

    QString tmpX = "";
    QString tmpY = "";
    bool isLoadRoute = true;
    bool isFirstSubt = true;
    int counter = 4;
    QString sub;
    bool rearCamIsEnabled = true;

    //WIDGETS:

    QStackedWidget *bottomStack;
    QWidget *widg;
    QHBoxLayout *hb;
    QWidget *sliderWidg;
    QHBoxLayout *sliderLayout;
    QWidget *sliderButtonsWidg;
    QVBoxLayout *sliderButtonsLayout;
    QWidget *graphSectionWidg;
    QHBoxLayout *graphSectionLayer;
    QWidget *lineWithSpeedWidg;
    QVBoxLayout *layerWithSpeed;
    QHBoxLayout *prndm;
    QWidget *prndmWidg;
    QVBoxLayout *smallSlidersLayout;
    QHBoxLayout *speedLayout;
    QHBoxLayout *brightnessLayout;
    QHBoxLayout *soundLayout;
    QHBoxLayout *gSensorsAllLayout;
    QWidget *gSensorsWidg;
    QVBoxLayout *gSensorsLabels;
    QHBoxLayout *strangeButtons;
    QWidget *strangeButtonsWidg;
    QWidget *bot_buttons;
    QHBoxLayout *bottom_buttons;
    QVBoxLayout *bot1;
    QVBoxLayout *bot2;
    QVBoxLayout *bot3;
    QVBoxLayout *bot4;
    QVBoxLayout *bot5;
    QWidget *botW1;
    QWidget *botW2;
    QWidget *botW3;
    QWidget *botW4;
    QWidget *botW5;
    QGridLayout *firstLayout;
    QWidget *firstLaoutWidg;
    QGridLayout *secondLayout;
    QWidget *secondLaoutWidg;
    QGridLayout *thirdLayout;
    QWidget *thirdLaoutWidg;

    QWidget *graph;
    QwtPlot *plot;
    QwtPlotCurve *curveX, *curveY, *curveZ;
    QPolygonF *pointsX, *pointsY, *pointsZ;
    int x,y,z;
    bool front, rear, both, isFullscreen;
    int rowForPlay;
    bool isMirrorVert = false;
    bool isZoomed = false;
    bool isMirrorHor = false;
    QWidget *topPanel;
    bool isMaxSize = false;

    QWidget *logo;
    QLabel *title;
    QPushButton *minimize;
    QPushButton *maximize;
    QPushButton *close;

    QWidget *bottomButtonsWidg2;
    QHBoxLayout *bottomButtonsLayout2;


};

#endif // PLAYERWINDOW_H
