
TEMPLATE = app
CONFIG -= app_bundle
CONFIG += ordered


QT += opengl qml network quick widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

PROJECTROOT = $$PWD/../..
include($$PROJECTROOT/src/libQtAV.pri)
include($$PROJECTROOT/widgets/libQtAVWidgets.pri)
STATICLINK=1
include($$PWD/../common/libcommon.pri)
preparePaths($$OUT_PWD/../../out)


INCLUDEPATH += $$PWD

genRC($$TARGET)
include(src.pri)




#RC_FILE += file.rc
#OTHER_FILES += file.rc

#RESOURCES += res/player.qrc

target.path = $$[QT_INSTALL_BINS]

include($$PROJECTROOT/deploy.pri)

