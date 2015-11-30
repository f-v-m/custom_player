SOURCES += main.cpp \
    playerwindow.cpp \
    filters/AVFilterSubtitle.cpp \
    qmlmapviewerwrapper.cpp \
    playlist/PlayListModel.cpp \
    playlist/PlayListItem.cpp \
    playlist/PlayListDelegate.cpp \
    playlist/PlayList.cpp \
    $$PWD/buttons.cpp \
    $$PWD/widgets.cpp \
    $$PWD/map.cpp \
    $$PWD/subtitles.cpp \
    $$PWD/player.cpp \
    $$PWD/controls.cpp \
    $$PWD/ui_buttons.cpp \
    $$PWD/gsensorsgraph.cpp

HEADERS += \
    playerwindow.h \
    filters/AVFilterSubtitle.h \
    playlist/PlayListModel.h \
    playlist/PlayListItem.h \
    playlist/PlayListDelegate.h \
    playlist/PlayList.h

RESOURCES += \
    mapviewerwrapper.qrc \
    $$PWD/buttons.qrc



include(c:/Qt/Examples/Qt-5.4/location/common/common.pri)

#DISTFILES +=
