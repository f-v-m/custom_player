/******************************************************************************
    QtAV Player Demo:  this file is part of QtAV examples
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
#include <QLayout>
#include <QMessageBox>
#include <iostream>
#include <sstream>
#include <QWidget>

//#include <QtGui>
#include <QtCore/QStringList>
#include <QtCore/QTextStream>
#include <QtGui/QGuiApplication>
#include <QtQuick/QQuickView>
#include <QtQuick/QQuickItem>
#include <QPalette>
#include <QWidgetAction>

#include <QStackedWidget>
#include "PlayList.h"
#include "PlayListModel.h"
#include "PlayListDelegate.h"
#include "common/common.h"
#include <QFileDialog>
#include <QListView>
#include <QLayout>
#include <QToolButton>
#include <QtCore/QFile>
#include <QtCore/QDataStream>
#include <QLabel>

#include <sys/stat.h>
#include <time.h>

using namespace std;

PlayList::PlayList(QWidget *parent) :
    QWidget(parent)
{
    mFirstShow = true;
    mMaxRows = mMaxRowsEvent = mMaxRowsParking = mMaxRowsNormal = mMaxRowsManual = -1;
    mpModel = new PlayListModel(this);
    mpModelEvent = new PlayListModel(this);
    mpModelParking = new PlayListModel(this);
    mpModelNormal = new PlayListModel(this);
    mpModelManual = new PlayListModel(this);

    mpDelegate = new PlayListDelegate(this);
    mpDelegateEvent = new PlayListDelegate(this);
    mpDelegateParking = new PlayListDelegate(this);
    mpDelegateNormal = new PlayListDelegate(this);
    mpDelegateManual = new PlayListDelegate(this);
    mpListView = new QListView;
    mpListViewEvent = new QListView;
    mpListViewParking = new QListView;
    mpListViewNormal = new QListView;
    mpListViewManual = new QListView;

    //mpListView->setResizeMode(QListView::Adjust);
    mpListView->setModel(mpModel);
    mpListView->setItemDelegate(mpDelegate);
    mpListView->setSelectionMode(QAbstractItemView::ExtendedSelection); //ctrl,shift
    mpListView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    mpListViewEvent->setModel(mpModelEvent);
    mpListViewEvent->setItemDelegate(mpDelegateEvent);
    mpListViewEvent->setSelectionMode(QAbstractItemView::ExtendedSelection); //ctrl,shift
    mpListViewEvent->setEditTriggers(QAbstractItemView::NoEditTriggers);

    mpListViewParking->setModel(mpModelParking);
    mpListViewParking->setItemDelegate(mpDelegateParking);
    mpListViewParking->setSelectionMode(QAbstractItemView::ExtendedSelection); //ctrl,shift
    mpListViewParking->setEditTriggers(QAbstractItemView::NoEditTriggers);

    mpListViewNormal->setModel(mpModelNormal);
    mpListViewNormal->setItemDelegate(mpDelegateNormal);
    mpListViewNormal->setSelectionMode(QAbstractItemView::ExtendedSelection); //ctrl,shift
    mpListViewNormal->setEditTriggers(QAbstractItemView::NoEditTriggers);

    mpListViewManual->setModel(mpModelManual);
    mpListViewManual->setItemDelegate(mpDelegateManual);
    mpListViewManual->setSelectionMode(QAbstractItemView::ExtendedSelection); //ctrl,shift
    mpListViewManual->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mpListView->setToolTip(QString::fromLatin1("Ctrl/Shift + ") + tr("Click to select multiple"));
    QVBoxLayout *vbl = new QVBoxLayout;
    setLayout(vbl);
    vbl->setSpacing(0);
    vbl->setContentsMargins(0,0,0,0);
    playlistMode = 1;

    mpListViewEvent->setStyleSheet("QListView { background-image: url(:/images/images/file_list/file_list_bg.png);}");
    mpListViewParking->setStyleSheet("QListView { background-image: url(:/images/images/file_list/file_list_bg.png);}");
    mpListViewNormal->setStyleSheet("QListView { background-image: url(:/images/images/file_list/file_list_bg.png);}");
    mpListViewManual->setStyleSheet("QListView { background-image: url(:/images/images/file_list/file_list_bg.png);}");
    mpListView->setStyleSheet("QListView { background-image: url(:/images/images/file_list/file_list_bg.png);}");

    QHBoxLayout *hbl = new QHBoxLayout();
    QWidget *hw1 = new QWidget();
    hw1->setLayout(hbl);
    hw1->setStyleSheet("QWidget { border-image: url(:/images/images/google_map/google_bottom_bar_bg.png);}");
    vbl->addWidget(hw1);

    QHBoxLayout *hb2 = new QHBoxLayout();
    hb2->setSpacing(0);
    QWidget *hw2 = new QWidget();
    hw2->setLayout(hb2);
    hw2->setStyleSheet("QWidget { border-image: url(:/images/images/google_map/google_bottom_bar_bg.png);}");


    all = new QPushButton("All");
    all->setStyleSheet("QPushButton {border-image: url(:/images/images/file_list/active_heading_bg.png);"
                       "border: 0;"
                       "color: white;}"
                       "QPushButton:hover {color: #ff9666};");
    event = new QPushButton("Event");
    event->setStyleSheet("QPushButton {border-image: url(:/images/images/file_list/normal_heading_bg.png);"
                         "border: 0;"
                         "color: white;}"
                         "QPushButton:hover {color: #ff9666};");
    parking = new QPushButton("Parking");
    parking->setStyleSheet("QPushButton {border-image: url(:/images/images/file_list/normal_heading_bg.png);"
                           "border: 0;"
                           "color: white;}"
                           "QPushButton:hover {color: #ff9666};");
    normal = new QPushButton("Normal");
    normal->setStyleSheet("QPushButton {border-image: url(:/images/images/file_list/normal_heading_bg.png);"
                          "border: 0;"
                          "color: white;}"
                          "QPushButton:hover {color: #ff9666};");
    manual = new QPushButton("Manual");
    manual->setStyleSheet("QPushButton {border-image: url(:/images/images/file_list/normal_heading_bg.png);"
                          "border: 0;"
                          "color: white;}"
                          "QPushButton:hover {color: #ff9666};");
    all->setFixedSize(76, 25);
    event->setFixedSize(76, 25);
    parking->setFixedSize(76, 25);
    normal->setFixedSize(76, 25);
    manual->setFixedSize(76, 25);
    hb2->setContentsMargins(0,0,0,0);
    hb2->setSpacing(1);
    hb2->setAlignment(Qt::AlignLeft);
    hw2->setContentsMargins(0,0,0,0);
    hb2->addWidget(all);
    hb2->addWidget(event);
    hb2->addWidget(parking);
    hb2->addWidget(normal);
    hb2->addWidget(manual);

    vbl->addWidget(hw2);
    QPushButton *numb = new QPushButton("№");
    numb->setStyleSheet("QPushButton {border-image: url(:/images/images/file_list/active_list_bg.png);"
                          "border: 0;"
                          "color: white;}");
    numb->setFixedSize(30,25);
    QPushButton *date = new QPushButton("Date");
    date->setStyleSheet("QPushButton {border-image: url(:/images/images/file_list/active_list_bg.png);"
                          "border: 0;"
                          "color: white;}");
    date->setFixedSize(100,25);
    QPushButton *size = new QPushButton("Size");
    size->setStyleSheet("QPushButton {border-image: url(:/images/images/file_list/active_list_bg.png);"
                          "border: 0;"
                          "color: white;}");
    size->setFixedSize(86,25);
    QPushButton *name = new QPushButton("Name");

    name->setStyleSheet("QPushButton {border-image: url(:/images/images/file_list/active_list_bg.png);"
                          "border: 0;"
                          "color: white;}");
    name->setFixedHeight(25);
    //name->setMinimumWidth(162);
    //name->setMaximumWidth(485);
    name->setFixedWidth(485);
    QHBoxLayout *hb3 = new QHBoxLayout();
    hb3->setAlignment(Qt::AlignLeft);
    hb3->setSpacing(0);
    QWidget *hw3 = new QWidget();
    hw3->setLayout(hb3);
    hw3->setStyleSheet("QWidget { border-image: url(:/images/images/file_list/active_list_bg.png);}");
    hb3->addWidget(numb);
    hb3->addSpacing(1);
    hb3->addWidget(date);
    hb3->addSpacing(1);
    hb3->addWidget(size);
    hb3->addSpacing(1);
    hb3->addWidget(name);

    hb3->setContentsMargins(0,0,0,0);
    hw3->setContentsMargins(0,0,0,0);
    vbl->addWidget(hw3);

    mpListView->setContentsMargins(0,0,0,0);
    mpListView->setMaximumWidth(700);
    mpListViewEvent->setContentsMargins(0,0,0,0);
    mpListViewEvent->setMaximumWidth(700);
    mpListViewParking->setContentsMargins(0,0,0,0);
    mpListViewParking->setMaximumWidth(700);
    mpListViewNormal->setContentsMargins(0,0,0,0);
    mpListViewNormal->setMaximumWidth(700);
    mpListViewManual->setContentsMargins(0,0,0,0);
    mpListViewManual->setMaximumWidth(700);

    plStack = new QStackedWidget();
    plStack->setContentsMargins(0,0,0,0);
    plStack->setMaximumWidth(700);

    plStack->addWidget(mpListView);
    plStack->addWidget(mpListViewEvent);
    plStack->addWidget(mpListViewParking);
    plStack->addWidget(mpListViewNormal);
    plStack->addWidget(mpListViewManual);
    plStack->setCurrentIndex(0);
    vbl->addWidget(plStack);


    QLabel *nameLabel = new QLabel("File List");
    nameLabel->setStyleSheet("QLabel {background: transparent; "
                             "color: white;}"
                            "QLabel {font-size: 15pt;}");


    QToolButton *flipButton = new QToolButton(0);
    flipButton->setStyleSheet("QToolButton { border-image: url(:/images/images/file_list/flip_btn_normal.png);} "
                          "QToolButton:hover { border-image: url(:/images/images/file_list/flip_btn_hover_active.png)}");
    flipButton->setFixedSize(26, 17);

    QToolButton *downloadButton = new QToolButton(0);
    downloadButton->setStyleSheet("QToolButton { border-image: url(:/images/images/file_list/download_btn_normal.png);} "
                          "QToolButton:hover { border-image: url(:/images/images/file_list/download_btnhover_active.png)}");
    downloadButton->setFixedSize(20, 17);

    mpRemove = new QToolButton(0);
    //mpRemove->setText(QString::fromLatin1("-"));
    mpRemove->setToolTip(tr("Remove selected items"));
    mpRemove->setFixedSize(13, 17);
    mpAdd = new QToolButton(0);
    mpAdd->setStyleSheet("QToolButton { border-image: url(:/images/images/file_list/folder_btn_normal.png);} "
                         "QToolButton:hover { border-image: url(:/images/images/file_list/folder_btn_hover_active.png)}");
    mpAdd->setFixedSize(18, 15);
    mpRemove->setStyleSheet("QToolButton { border-image: url(:/images/images/file_list/del_btn_normal.png);} "
                            "QToolButton:hover { border-image: url(:/images/images/file_list/del_btn_hover_active.png)}");



    hbl->setAlignment(Qt::AlignLeft);
    //hbl->addSpacing(width());
    hbl->addWidget(nameLabel);
    hbl->addSpacing(55);
    hbl->addWidget(flipButton);
    hbl->addSpacing(30);
    hbl->addWidget(downloadButton);
    hbl->addSpacing(30);
    hbl->addWidget(mpRemove);
    hbl->addSpacing(30);
    hbl->addWidget(mpAdd);

    //connect(mpClear, SIGNAL(clicked()), SLOT(clearItems()));
    connect(mpRemove, SIGNAL(clicked()), SLOT(removeSelectedItems()));
    connect(mpAdd, SIGNAL(clicked()), SLOT(addItems()));
    connect(mpListView, SIGNAL(doubleClicked(QModelIndex)), SLOT(onAboutToPlay(QModelIndex)));
    connect(mpListViewEvent, SIGNAL(doubleClicked(QModelIndex)), SLOT(onAboutToPlay(QModelIndex)));
    connect(mpListViewParking, SIGNAL(doubleClicked(QModelIndex)), SLOT(onAboutToPlay(QModelIndex)));
    connect(mpListViewNormal, SIGNAL(doubleClicked(QModelIndex)), SLOT(onAboutToPlay(QModelIndex)));
    connect(mpListViewManual, SIGNAL(doubleClicked(QModelIndex)), SLOT(onAboutToPlay(QModelIndex)));

    connect(all, SIGNAL(clicked()), SLOT(changePlaylistMode1()));
    connect(event, SIGNAL(clicked()), SLOT(changePlaylistMode2()));
    connect(parking, SIGNAL(clicked()), SLOT(changePlaylistMode3()));
    connect(normal, SIGNAL(clicked()), SLOT(changePlaylistMode4()));
    connect(manual, SIGNAL(clicked()), SLOT(changePlaylistMode5()));
}

PlayList::~PlayList()
{
    qDebug("+++++++++++++~PlayList()");
    save();
}

void PlayList::setSaveFile(const QString &file)
{
    mFile = file;
}

QString PlayList::saveFile() const
{
    return mFile;
}

void PlayList::load()
{
    QFile f(mFile);
    if (!f.exists())
        return;
    if (!f.open(QIODevice::ReadOnly))
        return;
    QDataStream ds(&f);
    QList<PlayListItem> list;
    ds >> list;
    for (int i = 0; i < list.size(); ++i) {
        insertItemAt(list.at(i), i);
    }
}

void PlayList::save()
{
    QFile f(mFile);
    if (!f.open(QIODevice::WriteOnly)) {
        qWarning("File open error: %s", qPrintable(f.errorString()));
        return;
    }
    QDataStream ds(&f);
    if (playlistMode == 2){
        ds << mpModelEvent->items();
    }else if (playlistMode ==3 ){
        ds << mpModelParking->items();
    }else if (playlistMode == 4){
        ds << mpModelNormal->items();
    }else if (playlistMode == 5){
        ds << mpModelManual->items();
    }
    ds << mpModel->items();
}

PlayListItem PlayList::itemAt(int row)
{
    if (playlistMode == 1){
        if (mpModel->rowCount() < 0) {
            qWarning("Invalid rowCount");
            return PlayListItem();
        }
        return mpModel->data(mpModel->index(row), Qt::DisplayRole).value<PlayListItem>();
    }else if (playlistMode == 2){
        if (mpModelEvent->rowCount() < 0) {
            qWarning("Invalid rowCount");
            return PlayListItem();
        }
        return mpModelEvent->data(mpModelEvent->index(row), Qt::DisplayRole).value<PlayListItem>();
    }else if (playlistMode ==3 ){
        if (mpModelParking->rowCount() < 0) {
            qWarning("Invalid rowCount");
            return PlayListItem();
        }
        return mpModelParking->data(mpModelParking->index(row), Qt::DisplayRole).value<PlayListItem>();
    }else if (playlistMode == 4){
        if (mpModelNormal->rowCount() < 0) {
            qWarning("Invalid rowCount");
            return PlayListItem();
        }
        return mpModelNormal->data(mpModelNormal->index(row), Qt::DisplayRole).value<PlayListItem>();
    }else if (playlistMode == 5){
        if (mpModelManual->rowCount() < 0) {
            qWarning("Invalid rowCount");
            return PlayListItem();
        }
        return mpModelManual->data(mpModelManual->index(row), Qt::DisplayRole).value<PlayListItem>();
    }

}

void PlayList::insertItemAt(const PlayListItem &item, int row)
{
    if (playlistMode == 2){
        if (mMaxRowsEvent > 0 && mpModelEvent->rowCount() >= mMaxRowsEvent) {
            // +1 because new row is to be inserted
            mpModelEvent->removeRows(mMaxRowsEvent, mpModelEvent->rowCount() - mMaxRowsEvent + 1);
        }
        int i = mpModelEvent->items().indexOf(item, row+1);
        if (i > 0) {
            mpModelEvent->removeRow(i);
        }
        if (!mpModelEvent->insertRow(row))
            return;
        if (row > 0) {
            i = mpModelEvent->items().lastIndexOf(item, row - 1);
            if (i >= 0)
                mpModelEvent->removeRow(i);
        }
        setItemAt(item, row);
    }else if (playlistMode ==3 ){

        if (mMaxRowsParking > 0 && mpModelParking->rowCount() >= mMaxRowsParking) {
            // +1 because new row is to be inserted
            mpModelParking->removeRows(mMaxRowsParking, mpModelParking->rowCount() - mMaxRowsParking + 1);
        }
        int i = mpModelParking->items().indexOf(item, row+1);
        if (i > 0) {
            mpModelParking->removeRow(i);
        }
        if (!mpModelParking->insertRow(row))
            return;
        if (row > 0) {
            i = mpModelParking->items().lastIndexOf(item, row - 1);
            if (i >= 0)
                mpModelParking->removeRow(i);
        }
        setItemAt(item, row);
    }else if (playlistMode == 4){
        if (mMaxRowsNormal > 0 && mpModelNormal->rowCount() >= mMaxRowsNormal) {
            // +1 because new row is to be inserted
            mpModelNormal->removeRows(mMaxRowsNormal, mpModelNormal->rowCount() - mMaxRowsNormal + 1);
        }
        int i = mpModelNormal->items().indexOf(item, row+1);
        if (i > 0) {
            mpModelNormal->removeRow(i);
        }
        if (!mpModelNormal->insertRow(row))
            return;
        if (row > 0) {
            i = mpModelNormal->items().lastIndexOf(item, row - 1);
            if (i >= 0)
                mpModelNormal->removeRow(i);
        }
        setItemAt(item, row);
    }else if (playlistMode == 5){
        if (mMaxRowsManual > 0 && mpModelManual->rowCount() >= mMaxRowsManual) {
            // +1 because new row is to be inserted
            mpModelManual->removeRows(mMaxRowsManual, mpModelManual->rowCount() - mMaxRowsManual + 1);
        }
        int i = mpModelManual->items().indexOf(item, row+1);
        if (i > 0) {
            mpModelManual->removeRow(i);
        }
        if (!mpModelManual->insertRow(row))
            return;
        if (row > 0) {
            i = mpModelManual->items().lastIndexOf(item, row - 1);
            if (i >= 0)
                mpModelManual->removeRow(i);
        }
        setItemAt(item, row);
    }else if (playlistMode == 1){
        if (mMaxRows > 0 && mpModel->rowCount() >= mMaxRows) {
            // +1 because new row is to be inserted
            mpModel->removeRows(mMaxRows, mpModel->rowCount() - mMaxRows + 1);
        }
        int i = mpModel->items().indexOf(item, row+1);
        if (i > 0) {
            mpModel->removeRow(i);
        }
        if (!mpModel->insertRow(row))
            return;
        if (row > 0) {
            i = mpModel->items().lastIndexOf(item, row - 1);
            if (i >= 0)
                mpModel->removeRow(i);
        }
        setItemAt(item, row);
    }

}

void PlayList::setItemAt(const PlayListItem &item, int row)
{
    if (playlistMode == 1){
        mpModel->setData(mpModel->index(row), QVariant::fromValue(item), Qt::DisplayRole);
    }else if (playlistMode == 2){
        mpModelEvent->setData(mpModelEvent->index(row), QVariant::fromValue(item), Qt::DisplayRole);
    }else if (playlistMode ==3 ){
        mpModelParking->setData(mpModelParking->index(row), QVariant::fromValue(item), Qt::DisplayRole);
    }else if (playlistMode == 4){
        mpModelNormal->setData(mpModelNormal->index(row), QVariant::fromValue(item), Qt::DisplayRole);
    }else if (playlistMode == 5){
        mpModelManual->setData(mpModelManual->index(row), QVariant::fromValue(item), Qt::DisplayRole);
    }

}

void PlayList::insert(const QString &url, int row)
{
    PlayListItem item;
    item.setUrl(url);
    item.setDuration(0);
    item.setLastTime(0);
    QString title = url;
    if (!url.contains(QLatin1String("://")) || url.startsWith(QLatin1String("file://"))) {
        title = QFileInfo(url).fileName();
    }

    if (playlistMode == 2){
        QString curNum = QString("%1").arg(rowsQuantityEvent+1, 2, 10, QChar('0'));
        struct stat buf;
        stat(item.url().toStdString().c_str(), &buf);
        char date[30];
        //date = asctime(localtime(&buf.st_atime));

        strftime(date, sizeof(date), " %m/%d/%Y", localtime(&buf.st_ctime));

        //SIZE:
        size_t size = buf.st_size;
        static const char *SIZES[] = { "B", "KB", "MB", "GB" };
        int div = 0;
        size_t rem = 0;

        while (size >= 1024 && div < (sizeof SIZES / sizeof *SIZES)) {
            rem = (size % 1024);
            div++;
            size /= 1024;
        }

        double size_d = (float)size + (float)rem / 1024.0;
        double d = size_d * 100.0;
        int i = d + 0.5;
        d = (int)i / 100.0;
        QString curSize = " " + QString::number(d)+SIZES[div];

        //----------------------

        QString formatedTitle = title.leftJustified(30, ' ');
        //item.setTitle(curNum + "     "+ date + "      " + curSize.leftJustified(12, ' ') + "         " + formatedTitle);
        item.pliNum = curNum;
        item.pliDate = date;
        item.pliSize = curSize;

        item.setTitle(title);
        insertItemAt(item, rowsQuantityEvent);
        curNum = QString("%1").arg(rowsQuantity+1, 2, 10, QChar('0'));
        item.pliNum = curNum;
        changePlaylistMode1();
        insertItemAt(item, rowsQuantity);
        changePlaylistMode2();
    }else if (playlistMode ==3 ){
        QString curNum = QString("%1").arg(rowsQuantityParking+1, 2, 10, QChar('0'));
        struct stat buf;
        stat(item.url().toStdString().c_str(), &buf);
        char date[30];
        //date = asctime(localtime(&buf.st_atime));

        strftime(date, sizeof(date), " %m/%d/%Y", localtime(&buf.st_ctime));

        //SIZE:
        size_t size = buf.st_size;
        static const char *SIZES[] = { "B", "KB", "MB", "GB" };
        int div = 0;
        size_t rem = 0;

        while (size >= 1024 && div < (sizeof SIZES / sizeof *SIZES)) {
            rem = (size % 1024);
            div++;
            size /= 1024;
        }

        double size_d = (float)size + (float)rem / 1024.0;
        double d = size_d * 100.0;
        int i = d + 0.5;
        d = (int)i / 100.0;
        QString curSize = " " + QString::number(d)+SIZES[div];

        //----------------------

        QString formatedTitle = title.leftJustified(30, ' ');
        //item.setTitle(curNum + "     "+ date + "      " + curSize.leftJustified(12, ' ') + "         " + formatedTitle);
        item.pliNum = curNum;
        item.pliDate = date;
        item.pliSize = curSize;

        item.setTitle(title);
        insertItemAt(item, rowsQuantityParking);
        curNum = QString("%1").arg(rowsQuantity+1, 2, 10, QChar('0'));
        item.pliNum = curNum;
        changePlaylistMode1();
        insertItemAt(item, rowsQuantity);
        changePlaylistMode3();
    }else if (playlistMode == 4){
        QString curNum = QString("%1").arg(rowsQuantityNormal+1, 2, 10, QChar('0'));
        struct stat buf;
        stat(item.url().toStdString().c_str(), &buf);
        char date[30];
        //date = asctime(localtime(&buf.st_atime));

        strftime(date, sizeof(date), " %m/%d/%Y", localtime(&buf.st_ctime));

        //SIZE:
        size_t size = buf.st_size;
        static const char *SIZES[] = { "B", "KB", "MB", "GB" };
        int div = 0;
        size_t rem = 0;

        while (size >= 1024 && div < (sizeof SIZES / sizeof *SIZES)) {
            rem = (size % 1024);
            div++;
            size /= 1024;
        }

        double size_d = (float)size + (float)rem / 1024.0;
        double d = size_d * 100.0;
        int i = d + 0.5;
        d = (int)i / 100.0;
        QString curSize = " " + QString::number(d)+SIZES[div];

        //----------------------

        QString formatedTitle = title.leftJustified(30, ' ');
        //item.setTitle(curNum + "     "+ date + "      " + curSize.leftJustified(12, ' ') + "         " + formatedTitle);
        item.pliNum = curNum;
        item.pliDate = date;
        item.pliSize = curSize;

        item.setTitle(title);
        insertItemAt(item, rowsQuantityNormal);
        curNum = QString("%1").arg(rowsQuantity+1, 2, 10, QChar('0'));
        item.pliNum = curNum;
        changePlaylistMode1();
        insertItemAt(item, rowsQuantity);
        changePlaylistMode4();
    }else if (playlistMode == 5){
        QString curNum = QString("%1").arg(rowsQuantityManual+1, 2, 10, QChar('0'));
        struct stat buf;
        stat(item.url().toStdString().c_str(), &buf);
        char date[30];
        //date = asctime(localtime(&buf.st_atime));

        strftime(date, sizeof(date), " %m/%d/%Y", localtime(&buf.st_ctime));

        //SIZE:
        size_t size = buf.st_size;
        static const char *SIZES[] = { "B", "KB", "MB", "GB" };
        int div = 0;
        size_t rem = 0;

        while (size >= 1024 && div < (sizeof SIZES / sizeof *SIZES)) {
            rem = (size % 1024);
            div++;
            size /= 1024;
        }

        double size_d = (float)size + (float)rem / 1024.0;
        double d = size_d * 100.0;
        int i = d + 0.5;
        d = (int)i / 100.0;
        QString curSize = " " + QString::number(d)+SIZES[div];

        //----------------------

        QString formatedTitle = title.leftJustified(30, ' ');
        //item.setTitle(curNum + "     "+ date + "      " + curSize.leftJustified(12, ' ') + "         " + formatedTitle);
        item.pliNum = curNum;
        item.pliDate = date;
        item.pliSize = curSize;

        item.setTitle(title);
        insertItemAt(item, rowsQuantityManual);
        curNum = QString("%1").arg(rowsQuantity+1, 2, 10, QChar('0'));
        item.pliNum = curNum;
        changePlaylistMode1();
        insertItemAt(item, rowsQuantity);
        changePlaylistMode5();
    }else if (playlistMode == 1){

        QString curNum = QString("%1").arg(rowsQuantity+1, 2, 10, QChar('0'));
        struct stat buf;
        stat(item.url().toStdString().c_str(), &buf);
        char date[30];
        //date = asctime(localtime(&buf.st_atime));

        strftime(date, sizeof(date), " %m/%d/%Y", localtime(&buf.st_ctime));

        //SIZE:
        size_t size = buf.st_size;
        static const char *SIZES[] = { "B", "KB", "MB", "GB" };
        int div = 0;
        size_t rem = 0;

        while (size >= 1024 && div < (sizeof SIZES / sizeof *SIZES)) {
            rem = (size % 1024);
            div++;
            size /= 1024;
        }

        double size_d = (float)size + (float)rem / 1024.0;
        double d = size_d * 100.0;
        int i = d + 0.5;
        d = (int)i / 100.0;
        QString curSize = " " + QString::number(d)+SIZES[div];

        //----------------------

        QString formatedTitle = title.leftJustified(30, ' ');
        //item.setTitle(curNum + "     "+ date + "      " + curSize.leftJustified(12, ' ') + "         " + formatedTitle);
        item.pliNum = curNum;
        item.pliDate = date;
        item.pliSize = curSize;

        item.setTitle(title);
        insertItemAt(item, rowsQuantity);
    }

}

void PlayList::remove(const QString &url)
{
    for (int i = mpModel->rowCount() - 1; i >= 0; --i) {
        PlayListItem item = mpModel->data(mpModel->index(i), Qt::DisplayRole).value<PlayListItem>();
        if (item.url() == url) {
            mpModel->removeRow(i);
        }
    }
}

void PlayList::setMaxRows(int r)
{
    if (playlistMode == 1){
        mMaxRows = r;
    }else if (playlistMode == 2){
        mMaxRowsEvent = r;
    }else if (playlistMode ==3 ){
        mMaxRowsParking = r;
    }else if (playlistMode == 4){
        mMaxRowsNormal = r;
    }else if (playlistMode == 5){
        mMaxRowsManual = r;
    }
}

int PlayList::maxRows() const
{

    if (playlistMode == 1){
        return mMaxRows;
    }else if (playlistMode == 2){
        return mMaxRowsEvent;
    }else if (playlistMode ==3 ){
        return mMaxRowsParking;
    }else if (playlistMode == 4){
        return mMaxRowsNormal;
    }else if (playlistMode == 5){
        return mMaxRowsManual;
    }
}

void PlayList::removeSelectedItems()
{
    if (playlistMode == 1){
        QItemSelectionModel *selection = mpListView->selectionModel();
        if (!selection->hasSelection())
            return;
        QModelIndexList s = selection->selectedIndexes();
        for (int i = s.size()-1; i >= 0; --i) {
            mpModel->removeRow(s.at(i).row());
            rowsQuantity = rowsQuantity - 1;
        }
    }else if (playlistMode == 2){
        QItemSelectionModel *selection = mpListViewEvent->selectionModel();
        if (!selection->hasSelection())
            return;
        QModelIndexList s = selection->selectedIndexes();
        for (int i = s.size()-1; i >= 0; --i) {
            mpModelEvent->removeRow(s.at(i).row());
            rowsQuantity = rowsQuantity - 1;
        }
    }else if (playlistMode ==3 ){
        QItemSelectionModel *selection = mpListViewParking->selectionModel();
        if (!selection->hasSelection())
            return;
        QModelIndexList s = selection->selectedIndexes();
        for (int i = s.size()-1; i >= 0; --i) {
            mpModelParking->removeRow(s.at(i).row());
            rowsQuantity = rowsQuantity - 1;
        }
    }else if (playlistMode == 4){
        QItemSelectionModel *selection = mpListViewNormal->selectionModel();
        if (!selection->hasSelection())
            return;
        QModelIndexList s = selection->selectedIndexes();
        for (int i = s.size()-1; i >= 0; --i) {
            mpModelNormal->removeRow(s.at(i).row());
            rowsQuantity = rowsQuantity - 1;
        }
    }else if (playlistMode == 5){
        QItemSelectionModel *selection = mpListViewManual->selectionModel();
        if (!selection->hasSelection())
            return;
        QModelIndexList s = selection->selectedIndexes();
        for (int i = s.size()-1; i >= 0; --i) {
            mpModelManual->removeRow(s.at(i).row());
            rowsQuantity = rowsQuantity - 1;
        }
    }

}

void PlayList::clearItems()
{
    mpModel->removeRows(0, mpModel->rowCount());
}

void PlayList::addItems()
{
    // TODO: add url;
    QStringList files = QFileDialog::getOpenFileNames(0, tr("Select files"));

    if (files.isEmpty())
        return;
    // TODO: check playlist file: m3u, pls... In another thread
    for (int i = 0; i < files.size(); ++i) {
        QString file = files.at(i);
        if (!QFileInfo(file).isFile())
            continue;

        insert(file, i);
        if (playlistMode == 2){
            rowsQuantityEvent+=1;
        }else if (playlistMode ==3 ){
            rowsQuantityParking+=1;
        }else if (playlistMode == 4){
            rowsQuantityNormal+=1;
        }else if (playlistMode == 5){
            rowsQuantityManual+=1;
        }
        rowsQuantity+=1;

    }
}

void PlayList::onAboutToPlay(const QModelIndex &index)
{
    rowIndex = index.row();
    isClicked = true;
    emit aboutToPlay(index.data(Qt::DisplayRole).value<PlayListItem>().url());

    save();
}

void PlayList::changePlaylistMode1(){
    playlistMode = 1;
    plStack->setCurrentIndex(playlistMode-1);
    cout << "playlistmode: " << playlistMode << endl;
    all->setStyleSheet("QPushButton {border-image: url(:/images/images/file_list/active_heading_bg.png);"
                       "border: 0;"
                       "color: white;}"
                       "QPushButton:hover {color: #ff9666};");

    event->setStyleSheet("QPushButton {border-image: url(:/images/images/file_list/normal_heading_bg.png);"
                         "border: 0;"
                         "color: white;}"
                         "QPushButton:hover {color: #ff9666};");

    parking->setStyleSheet("QPushButton {border-image: url(:/images/images/file_list/normal_heading_bg.png);"
                           "border: 0;"
                           "color: white;}"
                           "QPushButton:hover {color: #ff9666};");

    normal->setStyleSheet("QPushButton {border-image: url(:/images/images/file_list/normal_heading_bg.png);"
                          "border: 0;"
                          "color: white;}"
                          "QPushButton:hover {color: #ff9666};");

    manual->setStyleSheet("QPushButton {border-image: url(:/images/images/file_list/normal_heading_bg.png);"
                          "border: 0;"
                          "color: white;}"
                          "QPushButton:hover {color: #ff9666};");
}
void PlayList::changePlaylistMode2(){
    playlistMode = 2;
    plStack->setCurrentIndex(playlistMode-1);
    cout << "playlistmode: " << playlistMode << endl;
    all->setStyleSheet("QPushButton {border-image: url(:/images/images/file_list/normal_heading_bg.png);"
                       "border: 0;"
                       "color: white;}"
                       "QPushButton:hover {color: #ff9666};");

    event->setStyleSheet("QPushButton {border-image: url(:/images/images/file_list/active_heading_bg.png);"
                         "border: 0;"
                         "color: white;}"
                         "QPushButton:hover {color: #ff9666};");

    parking->setStyleSheet("QPushButton {border-image: url(:/images/images/file_list/normal_heading_bg.png);"
                           "border: 0;"
                           "color: white;}"
                           "QPushButton:hover {color: #ff9666};");

    normal->setStyleSheet("QPushButton {border-image: url(:/images/images/file_list/normal_heading_bg.png);"
                          "border: 0;"
                          "color: white;}"
                          "QPushButton:hover {color: #ff9666};");

    manual->setStyleSheet("QPushButton {border-image: url(:/images/images/file_list/normal_heading_bg.png);"
                          "border: 0;"
                          "color: white;}"
                          "QPushButton:hover {color: #ff9666};");
}
void PlayList::changePlaylistMode3(){
    playlistMode = 3;
    plStack->setCurrentIndex(playlistMode-1);
    cout << "playlistmode: " << playlistMode << endl;
    all->setStyleSheet("QPushButton {border-image: url(:/images/images/file_list/normal_heading_bg.png);"
                       "border: 0;"
                       "color: white;}"
                       "QPushButton:hover {color: #ff9666};");

    event->setStyleSheet("QPushButton {border-image: url(:/images/images/file_list/normal_heading_bg.png);"
                         "border: 0;"
                         "color: white;}"
                         "QPushButton:hover {color: #ff9666};");

    parking->setStyleSheet("QPushButton {border-image: url(:/images/images/file_list/active_heading_bg.png);"
                           "border: 0;"
                           "color: white;}"
                           "QPushButton:hover {color: #ff9666};");

    normal->setStyleSheet("QPushButton {border-image: url(:/images/images/file_list/normal_heading_bg.png);"
                          "border: 0;"
                          "color: white;}"
                          "QPushButton:hover {color: #ff9666};");

    manual->setStyleSheet("QPushButton {border-image: url(:/images/images/file_list/normal_heading_bg.png);"
                          "border: 0;"
                          "color: white;}"
                          "QPushButton:hover {color: #ff9666};");
}
void PlayList::changePlaylistMode4(){
    playlistMode = 4;
    plStack->setCurrentIndex(playlistMode-1);
    cout << "playlistmode: " << playlistMode << endl;
    all->setStyleSheet("QPushButton {border-image: url(:/images/images/file_list/normal_heading_bg.png);"
                       "border: 0;"
                       "color: white;}"
                       "QPushButton:hover {color: #ff9666};");

    event->setStyleSheet("QPushButton {border-image: url(:/images/images/file_list/normal_heading_bg.png);"
                         "border: 0;"
                         "color: white;}"
                         "QPushButton:hover {color: #ff9666};");

    parking->setStyleSheet("QPushButton {border-image: url(:/images/images/file_list/normal_heading_bg.png);"
                           "border: 0;"
                           "color: white;}"
                           "QPushButton:hover {color: #ff9666};");

    normal->setStyleSheet("QPushButton {border-image: url(:/images/images/file_list/active_heading_bg.png);"
                          "border: 0;"
                          "color: white;}"
                          "QPushButton:hover {color: #ff9666};");

    manual->setStyleSheet("QPushButton {border-image: url(:/images/images/file_list/normal_heading_bg.png);"
                          "border: 0;"
                          "color: white;}"
                          "QPushButton:hover {color: #ff9666};");
}
void PlayList::changePlaylistMode5(){
    playlistMode = 5;
    plStack->setCurrentIndex(playlistMode-1);
    cout << "playlistmode: " << playlistMode << endl;
    all->setStyleSheet("QPushButton {border-image: url(:/images/images/file_list/normal_heading_bg.png);"
                       "border: 0;"
                       "color: white;}"
                       "QPushButton:hover {color: #ff9666};");

    event->setStyleSheet("QPushButton {border-image: url(:/images/images/file_list/normal_heading_bg.png);"
                         "border: 0;"
                         "color: white;}"
                         "QPushButton:hover {color: #ff9666};");

    parking->setStyleSheet("QPushButton {border-image: url(:/images/images/file_list/normal_heading_bg.png);"
                           "border: 0;"
                           "color: white;}"
                           "QPushButton:hover {color: #ff9666};");

    normal->setStyleSheet("QPushButton {border-image: url(:/images/images/file_list/normal_heading_bg.png);"
                          "border: 0;"
                          "color: white;}"
                          "QPushButton:hover {color: #ff9666};");

    manual->setStyleSheet("QPushButton {border-image: url(:/images/images/file_list/active_heading_bg.png);"
                          "border: 0;"
                          "color: white;}"
                          "QPushButton:hover {color: #ff9666};");
}
