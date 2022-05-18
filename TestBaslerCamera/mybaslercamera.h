#ifndef MYBASLERCAMERA_H
#define MYBASLERCAMERA_H

#include <QObject>

#include <pylon/PylonIncludes.h>
#include <pylon/usb/BaslerUsbInstantCameraArray.h>
#include <pylon/ImageEventHandler.h>
#include <pylon/ConfigurationEventHandler.h>
#include <pylon/PylonGUIIncludes.h>
#include <pylon/GrabResultPtr.h>
#include <pylon/PylonBase.h>

#include <QImage>
#include <QThread>
#include <QPixmap>

#include <QDebug>

class MyBaslerCamera : public QObject
{
    Q_OBJECT
public:
    MyBaslerCamera();
    ~MyBaslerCamera();

    void init();

    double getExposureTime();

    double getGain();

    void setSetting(double,double);

    void run();
signals:
    void sendGrabb1(QImage);
    void sendGrabb2(QImage);

private:

};

#endif // MYBASLERCAMERA_H
