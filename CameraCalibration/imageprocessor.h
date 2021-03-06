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

#include <pylon/PylonIncludes.h>
#include <pylon/usb/BaslerUsbInstantCameraArray.h>
#include <pylon/ImageEventHandler.h>
#include <pylon/ConfigurationEventHandler.h>
#include <pylon/PylonGUIIncludes.h>
#include <pylon/GrabResultPtr.h>
#include <pylon/PylonBase.h>

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
        FIND_STEREO_STREAM,

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

    enum DeviceState{
        WEB_CAMERA,
        BASLER_CAMERA
    };

    Q_ENUM(StateVideoStream)

   ImageProcessor(int indexCam,int numCam);
   ImageProcessor(FileSystem *fs, StateVideoStream stVideo,DeviceState stDevice,QMutex *data_lock);
   ImageProcessor(cv::Mat img);
   ~ImageProcessor();

   std::vector<FileSystem::InformationImageSaved> getInfoCamera();

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
   void setIsDraw(bool);
   void setFileSystem(FileSystem *fs);
   QPixmap toMatQpixmap(cv::Mat mat);
   QPixmap toMatQpixmapGray(cv::Mat mat);
   cv::Mat getOutFrame();
   void undistort(cv::Mat input,cv::Mat output,cv::Mat cameraMatrix,cv::Mat distCoeffs);
   void stopedThread();
   void initCamera();

   void setDeviceState(DeviceState state);

signals:
    void outDisplay(QPixmap pixmap);
    void outDisplayFirst(QPixmap pixmap);
    void outDisplaySecond(QPixmap pixmap);
    void outDisplayFirstSecond(QPixmap,QPixmap,QMutex*);
    void setItem(QTableWidgetItem* item0,QTableWidgetItem* item1, QTableWidgetItem* item2);
    void andStream();
    void sendTerminal(QString);

public slots:
    void run() override;
private:
    bool isPressSnap_ = false;
    bool isSnapShoot_ = false;
    bool isTransformImg_ = false;
    bool isEnd_;
    bool isPattern_ = true;
    bool isDraw = false;
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

    std::vector<FileSystem::InformationImageSaved> imageInfo;

    StateVideoStream state_video_stream;
    DeviceState state_device;
    QMutex* lock;
};
#endif // IMAGEPROCESSOR_H
