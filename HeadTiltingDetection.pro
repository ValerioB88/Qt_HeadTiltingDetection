#-------------------------------------------------
#
# Project created by QtCreator 2016-02-27T15:52:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled5
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    helpingFun.h

FORMS    += mainwindow.ui




INCLUDEPATH += C:/opencv_mingw/install/include
LIBS += "C:/opencv_mingw/bin/*.dll"


LIBS += -LC:/opencv_mingw/bin/ \
    -lopencv_core310 \
    -lopencv_highgui310 \
    -lopencv_imgproc310  \
    -lopencv_features2d310 \
    -lopencv_calib3d310 \

RESOURCES += \
    resources.qrc

DESTDIR = $$PWD

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia
