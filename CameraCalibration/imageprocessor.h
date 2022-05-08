#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QObject>
#include <QPixmap>
#include <string>
#include <QFile>
#include <QString>

#include<opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/core/persistence.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/calib3d/calib3d.hpp>

#include <QThread>
#include <QMutex>
#include <QApplication>

#include "filesystem.h"
#include "calibrationprocessor.h"

class ImageProcessor :public QThread
{
    Q_OBJECT
public:

    enum StateVideoStream{
        FIND_FIRST_STREAM,
        FIND_SECOND_STREAM,

        FIRST_STREAM,
        SECOND_STREAM,
        FIRST_SECOND_STREAM,
        FIRST_SECOND_STREAM_WHITHLINE,
        FIRST_CALIBRATED_STREAM,
        SECOND_CALIBRATED_STREAM,
        FIRST_SECOND_CALIBRATED_STREAM,
        STEREO_STREAM,
        STEREO_DEPTH_STREAM
    };

    Q_ENUM(StateVideoStream)

   ImageProcessor(int indexCam,int numCam);
   ImageProcessor(FileSystem *fs, QString current,QMutex *lock);
   ImageProcessor(cv::Mat img);

   void setOutFrame(cv::Mat frame);
   void setPath(QString qstring);
   void setCountFrame(int countFrame);
   void setFrameRate(int frameRate);
   void setTransformImg(bool newTransformImg);
   void setIsPattern(bool isPattern);
   void setPattern(QString pattern);
   void setCheckboardstate(int row,int col);
   void setIsSnapShoot(bool isSnapShoot);
   void setCheckerSize(double checkerSize);
   void setMarkerSize(double markerSize);
   void setDictionaryName(QString dictionaryName);
   void setIsPressSnap();
   void setFileSystem(FileSystem *fs);
   QPixmap toMatQpixmap(cv::Mat mat);
   cv::Mat getOutFrame();
   void undistort(cv::Mat input,cv::Mat output,cv::Mat cameraMatrix,cv::Mat distCoeffs);
   void stopedThread();
   void initCamera();

signals:
    void outDisplay(QPixmap pixmap);
    void outDisplayFirst(QPixmap pixmap);
    void outDisplaySecond(QPixmap pixmap);
    void setItem(QTableWidgetItem* item1, QTableWidgetItem* item2);
    void andStream();

public slots:
    void run() override;
private:
    bool isPressSnap_ = false;
    bool isSnapShoot_ = false;
    bool isTransformImg_ = false;
    bool isEnd_;
    bool isPattern_ = true;
    int numCam_;
    int frameRate_;
    int countFrame_;
    int CHECKERBOARD_[2];
    int dictionary_;
    double checkerSize_;
    double markerSize_;
    QString path_;
    QString pattern_;
    FileSystem *filesystem;
    cv::Mat cameraMatrix_;
    cv::Mat newCameraMatrix_;
    cv::Mat distCoeffs_;
    cv::Mat inputFrame_;
    cv::Mat outFrame_;
    cv::VideoCapture web_cam_;
    cv::VideoCapture web_camFirst_;
    cv::VideoCapture web_camSecond_;
    CalibrationProcessor calibProcessor_;

    StateVideoStream state_video_stream;
    QMutex* lock;
};
#endif // IMAGEPROCESSOR_H
