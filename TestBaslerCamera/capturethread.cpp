#include "capturethread.h"

CaptureThread::CaptureThread()
{
    camera = new MyBaslerCamera();
    connect(camera,&MyBaslerCamera::sendGrabb,this,&CaptureThread::inputImage);
    camera->init(idDevice);
}

double CaptureThread::getExposureTime()
{
    return camera->getExposureTime();
}

double CaptureThread::getGain()
{
    return camera->getGain();
}

void CaptureThread::setSetting(double ex, double ga)
{
    camera->setSetting(ex, ga);
}

void CaptureThread::setIdDevice(int id)
{
    idDevice = id;
}


void CaptureThread::run()
{
    camera->run();
}

void CaptureThread::inputImage(QImage img)
{
    emit outputImg(img);
}
