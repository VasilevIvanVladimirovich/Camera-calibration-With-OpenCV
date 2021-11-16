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
#include "calibrationprocessor.h"

class ImageProcessor :public QThread
{
    Q_OBJECT
public:
   ImageProcessor(int num_web_cam);
   ImageProcessor(cv::Mat img);


   void setOutFrame(cv::Mat frame);
   void setPath(QString qstring);
   void setCountFrame(int countFrame);
   void setFrameRate(int frameRate);
   void setTransformImg(bool newTransformImg);
   void setIsPattern(bool isPattern);
   void setPattern(QString pattern);
   void setCheckboardstate(int row,int col);

   QPixmap toMatQpixmap(cv::Mat mat);

   cv::Mat getOutFrame();

   void undistort(cv::Mat input,cv::Mat output,cv::Mat cameraMatrix,cv::Mat distCoeffs);

   void stopedThread();

signals:
    void outDisplay(QPixmap pixmap);
    void setItem(QTableWidgetItem* item,QTableWidgetItem* item1);

public slots:
    void run() override;

private:
    bool isTransformImg_ = false;
    bool isEnd_;
    bool isPattern_ = false;
    int frameRate_;
    int countFrame_;
    int CHECKERBOARD_[2];
    QString path_;
    QString pattern_;
    FileSystem filesystem;
    cv::Mat cameraMatrix_;
    cv::Mat newCameraMatrix_;
    cv::Mat distCoeffs_;
    cv::Mat inputFrame_;
    cv::Mat outFrame_;
    cv::VideoCapture web_cam_;
    CalibrationProcessor calibProcessor_;
};

#endif // IMAGEPROCESSOR_H
