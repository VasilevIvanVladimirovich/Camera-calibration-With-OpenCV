#ifndef CAPTURETHREAD_H
#define CAPTURETHREAD_H

#include <QObject>
#include <QThread>
#include "mybaslercamera.h"

class CaptureThread : public QThread
{
    Q_OBJECT
public:
    CaptureThread();

    double getExposureTime();

    double getGain();

    void setSetting(double,double);
    void setIdDevice(int id);



public slots:
    void run() override;

signals:
    void outputImg1(QImage);
    void outputImg2(QImage);

public slots:
    void inputImage1(QImage);
    void inputImage2(QImage);

private:
    MyBaslerCamera* camera;

    int idDevice=0;

};

#endif // CAPTURETHREAD_H
