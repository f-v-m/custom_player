

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


void PlayerWindow::initMySubtitles(){
    //SUBTITLES:
    mpSubtitle->setPlayer(m_player);

    mpSubtitle->setEnabled(true);
    mpSubtitle->setAutoLoad(true);
}

void PlayerWindow::setSubtitleEngine(const QString &value)
{
    Q_UNUSED(value)
    QComboBox *box = qobject_cast<QComboBox*>(sender());
    if (!box)
        return;
    mpSubtitle->setEngines(QStringList() << box->itemData(box->currentIndex()).toString());
}


void PlayerWindow::toggoleSubtitleEnabled(bool value)
{
    mpSubtitle->setEnabled(value);
}

void PlayerWindow::toggleSubtitleAutoLoad(bool value)
{
    mpSubtitle->setAutoLoad(value);
}

void PlayerWindow::openSubtitle()
{
    QString file = QFileDialog::getOpenFileName(0, tr("Open a subtitle file"));
    if (file.isEmpty())
        return;
    mpSubtitle->setFile(file);
}

void PlayerWindow::setSubtitleCharset(const QString &charSet)
{
    Q_UNUSED(charSet);
    QComboBox *box = qobject_cast<QComboBox*>(sender());
    if (!box)
        return;
    mpSubtitle->setCodec(box->itemData(box->currentIndex()).toByteArray());
}

