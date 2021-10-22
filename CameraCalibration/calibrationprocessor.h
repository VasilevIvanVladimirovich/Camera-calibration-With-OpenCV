#ifndef CALIBRATIONPROCESSOR_H
#define CALIBRATIONPROCESSOR_H

#include <QString>
#include <QPixmap>
#include <QObject>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/persistence.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include "filesystem.h"

class CalibrationProcessor :public QObject
{
    Q_OBJECT
public:
    CalibrationProcessor(QObject *parent = nullptr);

    void accumulationVectorsImg();
    bool getFrameFromTable(int row);
    //void calibrationChessboardMethod(cv::Mat inputFrame);
    void setMaxCountInTable(int count);

signals:
    void requestFromTable(int row);
    void sendStatusImg(QString status, int row);
    //void request
public slots:
    void setTargetType(QString qstring);
    void setTargetSize(int row,int col);
    void setSubPixIter(int count);
    void setInputFrame(QString);
private:
    int maxCountInTable_;
    QString targetType_;
    int subPixelIter_;
    int CHECKERBOARD_[2]; //размер шахматной доски убрать автоэкспозицию!
    std::vector<std::vector<cv::Point3f> > objpoints_; //Вектор, для хранения векторов 3d точек для каждого изображения шахматной доски
    std::vector<std::vector<cv::Point2f> > imgpoints_; //Вектор, для хранения векторов 2d точек для каждого изображения шахматной доски
    cv::Mat inputFrame_;
    cv::Mat cameraMatrix_;
    cv::Mat distCoeffs_;
    cv::Mat R_;
    cv::Mat T_;

};

#endif // CALIBRATIONPROCESSOR_H
