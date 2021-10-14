#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QObject>
#include <QPixmap>

#include<opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>
#include <opencv2/core/persistence.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/calib3d/calib3d.hpp>

#include <QThread>

class ImageProcessor :public QThread
{
    Q_OBJECT
public:
   ImageProcessor(int num_web_cam);
   ImageProcessor(cv::Mat img);

   void stopedThread();
   void setOutFrame(cv::Mat frame);
   cv::Mat getOutFrame();

signals:
    void outDisplay(QPixmap pixmap);

public slots:
    void run() override;

private:
    bool end_;
    cv::VideoCapture web_cam_;
    cv::Mat outFrame_;
};

#endif // IMAGEPROCESSOR_H
