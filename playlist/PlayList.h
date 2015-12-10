#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QWidget>
#include <QtCore/QModelIndex>
#include "PlayListItem.h"
#include <QLabel>
#include <windows.h>



class QListView;
class QToolButton;
class PlayListDelegate;
class PlayerWindow;

class PlayListModel;
class PlayList : public QWidget
{
    Q_OBJECT
public:
    explicit PlayList(QWidget *parent = 0);
    ~PlayList();

    void setSaveFile(const QString& file);
    QString saveFile() const;
    void load();
    void save();

    PlayListItem itemAt(int row);
    void insertItemAt(const PlayListItem& item, int row = 0);
    void setItemAt(const PlayListItem& item, int row = 0);
    void remove(const QString& url);
    void insert(const QString& url, int row = 0);
    void setMaxRows(int r);
    int maxRows() const;

    int rowIndex;
    int rowsQuantity = 0;
    bool isClicked = false;
    WIN32_FIND_DATAA findData;
    std::map<std::string, FILETIME> report;

signals:
    void aboutToPlay(const QString& url);

private slots:
    void removeSelectedItems();
    void clearItems();
    //
    void addItems();

    void onAboutToPlay(const QModelIndex& index);
    //void highlight(const QModelIndex& index);
private:
    QListView *mpListView;
    QToolButton *mpClear, *mpRemove, *mpAdd;

    PlayListDelegate *mpDelegate;
    PlayListModel *mpModel;
    int mMaxRows;
    QString mFile;
    bool mFirstShow;

};

#endif // PLAYLIST_H
