QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#GENICAM_LOG_CONFIG_V3_1 = 'D:/PRoG/CLib/baslerCamera/log/config/DefaultLogging.properties'

SOURCES += \
        baslercamera.cpp \
        capturethread.cpp \
        main.cpp \
        mainwindow.cpp \
        mybaslercamera.cpp

#include(../TestBaslerCamera/opencv_library_path.pri)
include(../TestBaslerCamera/balserCamera_library_path.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    baslercamera.h \
    capturethread.h \
    mainwindow.h \
    mybaslercamera.h

QT += widgets

FORMS += \
    mainwindow.ui
