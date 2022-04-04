QT       += core gui multimedia widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    calibrationprocessor.cpp \
    controlwindow.cpp \
    dialogwindowdetectcalibration.cpp \
    dialogwindowimportimage.cpp \
    filesystem.cpp \
    imageprocessor.cpp \
    main.cpp \
    mainwindow.cpp \
    menuwindownewfile.cpp \
    streamsetting.cpp \
    tablecompare.cpp

HEADERS += \
    calibrationprocessor.h \
    controlwindow.h \
    dialogwindowdetectcalibration.h \
    dialogwindowimportimage.h \
    filesystem.h \
    imageprocessor.h \
    mainwindow.h \
    menuwindownewfile.h \
    streamsetting.h \
    tablecompare.h

FORMS += \
    mainwindow.ui

include(../CameraCalibration/opencv_library_path.pri)


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target




