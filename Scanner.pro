QT       += core gui

LIBS += -ltesseract -llept

LIBS += -I`pkg-config opencv4 --cflags --libs`
INCLUDEPATH += /usr/include/opencv4

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    camera.cpp \
    docwork.cpp \
    imageproc.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    camera.h \
    docwork.h \
    imageproc.h \
    mainwindow.h

FORMS += \
    camera.ui \
    docwork.ui \
    imageproc.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
