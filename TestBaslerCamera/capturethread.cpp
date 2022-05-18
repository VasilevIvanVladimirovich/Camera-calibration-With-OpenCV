#include "capturethread.h"

CaptureThread::CaptureThread()
{
    camera = new MyBaslerCamera();
    connect(camera,&MyBaslerCamera::sendGrabb1,this,&CaptureThread::inputImage1);
    connect(camera,&MyBaslerCamera::sendGrabb2,this,&CaptureThread::inputImage2);
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
    camera->init();
}

void CaptureThread::inputImage1(QImage img)
{
    emit outputImg1(img);
}

void CaptureThread::inputImage2(QImage img)
{
    emit outputImg2(img);
}
