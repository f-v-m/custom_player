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

using namespace std;

PlayList::PlayList(QWidget *parent) :
    QWidget(parent)
{
    mFirstShow = true;
    mMaxRows = -1;
    mpModel = new PlayListModel(this);
    mpDelegate = new PlayListDelegate(this);
    mpListView = new QListView;

    //mpListView->setResizeMode(QListView::Adjust);
    mpListView->setModel(mpModel);
    mpListView->setItemDelegate(mpDelegate);
    mpListView->setSelectionMode(QAbstractItemView::ExtendedSelection); //ctrl,shift
    mpListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mpListView->setToolTip(QString::fromLatin1("Ctrl/Shift + ") + tr("Click to select multiple"));
    QVBoxLayout *vbl = new QVBoxLayout;
    setLayout(vbl);
    vbl->setSpacing(0);
    vbl->setContentsMargins(0,0,0,0);

    mpListView->setStyleSheet("QListView { border-image: url(:/images/images/file_list/file_list_bg.png);}");

    QHBoxLayout *hbl = new QHBoxLayout();
    QWidget *hw1 = new QWidget();
    hw1->setLayout(hbl);
    hw1->setStyleSheet("QWidget { border-image: url(:/images/images/google_map/google_bottom_bar_bg.png);}");
    vbl->addWidget(hw1);

    QHBoxLayout *hb2 = new QHBoxLayout();
    hb2->setSpacing(0);
    QWidget *hw2 = new QWidget();
    hw2->setLayout(hb2);


    QPushButton *all = new QPushButton("All");
    all->setStyleSheet("QPushButton {background-image: url(:/images/images/file_list/active_heading_bg.png);"
                       "border: 0;"
                       "color: white;}"
                       "QPushButton:hover {color: #ff9666};");
    QPushButton *event = new QPushButton("Event");
    event->setStyleSheet("QPushButton {background-image: url(:/images/images/file_list/active_heading_bg.png);"
                         "border: 0;"
                         "color: white;}"
                         "QPushButton:hover {color: #ff9666};");
    QPushButton *parking = new QPushButton("Parking");
    parking->setStyleSheet("QPushButton {background-image: url(:/images/images/file_list/active_heading_bg.png);"
                           "border: 0;"
                           "color: white;}"
                           "QPushButton:hover {color: #ff9666};");
    QPushButton *normal = new QPushButton("Normal");
    normal->setStyleSheet("QPushButton {background-image: url(:/images/images/file_list/active_heading_bg.png);"
                          "border: 0;"
                          "color: white;}"
                          "QPushButton:hover {color: #ff9666};");
    QPushButton *manual = new QPushButton("Manual");
    manual->setStyleSheet("QPushButton {background-image: url(:/images/images/file_list/active_heading_bg.png);"
                          "border: 0;"
                          "color: white;}"
                          "QPushButton:hover {color: #ff9666};");
    all->setFixedSize(64, 25);
    event->setFixedSize(64, 25);
    parking->setFixedSize(64, 25);
    normal->setFixedSize(64, 25);
    manual->setFixedSize(64, 25);
    hb2->setContentsMargins(0,0,0,0);
    hw2->setContentsMargins(0,0,0,0);
    hb2->addWidget(all);
    hb2->addWidget(event);
    hb2->addWidget(parking);
    hb2->addWidget(normal);
    hb2->addWidget(manual);

    vbl->addWidget(hw2);
    mpListView->setContentsMargins(0,0,0,0);
    mpListView->setMaximumWidth(500);
    vbl->addWidget(mpListView);


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
    // enter to highight
    //connect(mpListView, SIGNAL(entered(QModelIndex)), SLOT(highlight(QModelIndex)));
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
    ds << mpModel->items();
}

PlayListItem PlayList::itemAt(int row)
{
    if (mpModel->rowCount() < 0) {
        qWarning("Invalid rowCount");
        return PlayListItem();
    }
    return mpModel->data(mpModel->index(row), Qt::DisplayRole).value<PlayListItem>();
}

void PlayList::insertItemAt(const PlayListItem &item, int row)
{

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

void PlayList::setItemAt(const PlayListItem &item, int row)
{
    mpModel->setData(mpModel->index(row), QVariant::fromValue(item), Qt::DisplayRole);
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
    item.setTitle(title);
    insertItemAt(item, row);
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
    mMaxRows = r;
}

int PlayList::maxRows() const
{
    return mMaxRows;
}

void PlayList::removeSelectedItems()
{
    QItemSelectionModel *selection = mpListView->selectionModel();
    if (!selection->hasSelection())
        return;
    QModelIndexList s = selection->selectedIndexes();
    for (int i = s.size()-1; i >= 0; --i) {
        mpModel->removeRow(s.at(i).row());
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

    }
}

void PlayList::onAboutToPlay(const QModelIndex &index)
{
    emit aboutToPlay(index.data(Qt::DisplayRole).value<PlayListItem>().url());
    save();
}


