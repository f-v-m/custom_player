
#ifndef PLAYERWINDOW_H
#define PLAYERWINDOW_H

#include <QWidget>
#include <QtAV>
#include <QUrl>
#include <QComboBox>
#include <QtQuick/QQuickItem>

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

    void forwardSeek();
    void backSeek();



signals:
    void ready();

private:
    bool mIsReady, mHasPendingPlay;
    QtAV::VideoOutput *m_vo;
    QtAV::AVPlayer *m_player;
    QtAV::VideoOutput *m_vo2;
    QtAV::AVPlayer *m_player2;
    const QtAV::Packet pkt;
    QSlider *m_slider;
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

    QTextItem *txt1;
    QtAV::SubtitleFilter *mpSubtitle;
    QComboBox *box = new QComboBox();
    PlayList *mpPlayList, *mpHistory;

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


};

#endif // PLAYERWINDOW_H
