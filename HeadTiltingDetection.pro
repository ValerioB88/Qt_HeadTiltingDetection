#-------------------------------------------------
#
# Project created by QtCreator 2016-02-27T15:52:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
win32{
TARGET = ../QtHeadTilting_win32/HeadTiltingDetectionWin
}
linux{
TARGET = bashFileLinux
}
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    helpingFun.h \
    HelpingFun.h

FORMS    += mainwindow.ui



win32{
INCLUDEPATH += C:/opencv_mingw/install/include
LIBS += "C:/opencv_mingw/bin/*.dll"


LIBS += -LC:/opencv_mingw/bin/ \
    -lopencv_core310 \
    -lopencv_highgui310 \
    -lopencv_imgproc310  \
    -lopencv_features2d310 \
    -lopencv_calib3d310 \
}

linux{
INCLUDEPATH += .
INCLUDEPATH += /usr/local/include
INCLUDEPATH += /usr/local/include/qt4
INCLUDEPATH += /usr/local/include/opencv
INCLUDEPATH += /usr/local/include/opencv2

LIBS += `pkg-config opencv --cflags --libs`
}

RESOURCES += \
    resources.qrc

DESTDIR = $$PWD

CONFIG += qt

QTPLUGIN += qtaudio_coreaudio


QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia


#CONFIG += console  #TAKE THIS OUT LATER!

