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
    $$PWD/controls.cpp

HEADERS += \
    playerwindow.h \
    filters/AVFilterSubtitle.h \
    playlist/PlayListModel.h \
    playlist/PlayListItem.h \
    playlist/PlayListDelegate.h \
    playlist/PlayList.h \

RESOURCES += \
    mapviewerwrapper.qrc

qmlcontent.files += \
    mapviewer.qml \
    demo.ogv
OTHER_FILES += $$qmlcontent.files

qmlcontentmap.files += \
    content/map/MapComponent.qml \
    content/map/Marker.qml \
    content/map/CircleItem.qml \
    content/map/RectangleItem.qml \
    content/map/PolylineItem.qml \
    content/map/PolygonItem.qml \
    content/map/ImageItem.qml \
    content/map/VideoItem.qml \
    content/map/3dItem.qml \
    content/map/MiniMap.qml
OTHER_FILES += $$qmlcontentmap.files

qmlcontentdialogs.files += \
    content/dialogs/Message.qml \
    content/dialogs/RouteDialog.qml
OTHER_FILES += $$qmlcontentdialogs.files

include(c:/Qt/Examples/Qt-5.4/location/common/common.pri)

DISTFILES +=
