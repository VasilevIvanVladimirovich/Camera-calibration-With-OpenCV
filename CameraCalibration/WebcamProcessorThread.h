#ifndef WEBCAMPROCESSORTHREAD_H
#define WEBCAMPROCESSORTHREAD_H

#include <QObject>
#include <QThread>
#include <QPixmap>


#include<opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>
#include <opencv2/core/persistence.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/calib3d/calib3d.hpp>

class WebcamProcessorThread : public QThread
{
Q_OBJECT
public:
     WebcamProcessorThread();
private:
    void run() override;
signals:
    void outDisplay(QPixmap pixmap);

private:
    cv::VideoCapture web_cam;
};

#endif // WEBCAMPROCESSORTHREAD_H
