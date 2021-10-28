#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QObject>
#include <QPixmap>
#include <string>
#include <QFile>
#include <QString>

#include<opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>
#include <opencv2/core/persistence.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/calib3d/calib3d.hpp>

#include <QThread>

#include "filesystem.h"

class ImageProcessor :public QThread
{
    Q_OBJECT
public:
   ImageProcessor(int num_web_cam);
   ImageProcessor(cv::Mat img);

   void stopedThread();
   void setOutFrame(cv::Mat frame);
   cv::Mat getOutFrame();

   void setPath(QString qstring);
   void setCountFrame(int countFrame);

   void setTransformImg(bool newTransformImg);

   void undistort(cv::Mat input,cv::Mat output,cv::Mat cameraMatrix,cv::Mat distCoeffs);

signals:
    void outDisplay(QPixmap pixmap);
    void setItem(QTableWidgetItem* item,QTableWidgetItem* item1);

public slots:
    void run() override;

private:
    bool end_;
    cv::VideoCapture web_cam_;
    cv::Mat cameraMatrix_;
    cv::Mat newCameraMatrix_;
    cv::Mat distCoeffs_;
    cv::Mat inputFrame_;
    cv::Mat outFrame_;
    FileSystem filesystem;
    QString path_;
    int countFrame_;
    bool transformImg;

};

#endif // IMAGEPROCESSOR_H
