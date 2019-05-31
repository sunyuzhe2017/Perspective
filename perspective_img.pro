#-------------------------------------------------
#
# Project created by QtCreator 2019-03-04T09:22:39
# Author :Yuzhe SUN
# Version 1.0 :2019.03.10 21:40 finished
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = perspective_img
TEMPLATE = app
#include opencv files
INCLUDEPATH += /usr/include \
                /usr/include/opencv \
                /usr/include/opencv2

LIBS += /usr/lib/x86_64-linux-gnu/libopencv_highgui.so \
        /usr/lib/x86_64-linux-gnu/libopencv_core.so    \
        /usr/lib/x86_64-linux-gnu/libopencv_imgproc.so
#---------------------
#作者：Shawn-HT
#来源：CSDN
#原文：https://blog.csdn.net/shawn_ht/article/details/40795039
#版权声明：本文为博主原创文章，转载请附上博文链接！

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS += \
        mainwindow.ui
