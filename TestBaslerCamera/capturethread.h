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


public slots:
    void run() override;

signals:
    void outputImg(QImage);

public slots:
    void inputImage(QImage);
private:
    MyBaslerCamera* camera;

};

#endif // CAPTURETHREAD_H
