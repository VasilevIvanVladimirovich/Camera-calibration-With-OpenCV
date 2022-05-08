#ifndef MYBASLERCAMERA_H
#define MYBASLERCAMERA_H

#include <QObject>

#include <pylon/PylonIncludes.h>
#include <pylon/ImageEventHandler.h>
#include <pylon/ConfigurationEventHandler.h>
#include <pylon/PylonGUIIncludes.h>
#include <pylon/GrabResultPtr.h>
#include <pylon/PylonGUI.h>
#include <pylon/PylonBase.h>
#include <pylon/BaslerUniversalInstantCamera.h>

#include <QImage>
#include <QThread>
#include <QPixmap>

#include <QDebug>

class MyBaslerCamera : public QObject, public Pylon::CImageEventHandler
{
    Q_OBJECT
public:
    MyBaslerCamera();
    ~MyBaslerCamera();

    void init();

    double getExposureTime();

    double getGain();

    void setSetting(double,double);

    virtual void OnImageGrabbed(Pylon::CInstantCamera& camera, const Pylon::CGrabResultPtr& ptrGrab);

    void run();
signals:
    void sendGrabb(QImage);

private:
    Pylon::CBaslerUniversalInstantCamera* baslerCamera_;

    Basler_UniversalCameraParams::UserSetDefaultEnums oldDefaultUserSet;
    Basler_UniversalCameraParams::UserSetDefaultSelectorEnums oldDefaultUserSetSelector;
};

#endif // MYBASLERCAMERA_H
