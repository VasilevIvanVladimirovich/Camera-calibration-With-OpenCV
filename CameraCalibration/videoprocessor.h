#ifndef VIDEOPROCESSOR_H
#define VIDEOPROCESSOR_H

#include <QObject>
#include <QPixmap>


#include<opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>
#include <opencv2/core/persistence.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/calib3d/calib3d.hpp>

class VideoProcessor :public QObject
{
Q_OBJECT
public:
    VideoProcessor();
   cv::Mat getOutFrame();
   void setOutFrame(cv::Mat frame);
   bool isStoped();
   void setpause(bool pause);
signals:
    void outDisplay(QPixmap pixmap);
public slots:
    void startVideo();
    void stopVideo();

private:
    bool end;
    bool stopped;
    cv::VideoCapture web_cam;
    cv::Mat outFrame;
};

#endif // VIDEOPROCESSOR_H
