QT       += core gui multimedia widgets charts


CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    calibrationprocessor.cpp \
    controlwindow.cpp \
    customgraphicsview.cpp \
    dialogwindowdetectcalibration.cpp \
    dialogwindowimportimage.cpp \
    filesystem.cpp \
    imageprocessor.cpp \
    main.cpp \
    menuwindownewfile.cpp \
    settingcamerawindow.cpp \
    settingpatternwindow.cpp \
    streamsetting.cpp \
    tablecompare.cpp

HEADERS += \
    calibrationprocessor.h \
    controlwindow.h \
    customgraphicsview.h \
    dialogwindowdetectcalibration.h \
    dialogwindowimportimage.h \
    filesystem.h \
    imageprocessor.h \
    menuwindownewfile.h \
    settingcamerawindow.h \
    settingpatternwindow.h \
    streamsetting.h \
    tablecompare.h

include(../CameraCalibration/opencv_library_path.pri)
include(../CameraCalibration/balserCamera_library_path.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target




