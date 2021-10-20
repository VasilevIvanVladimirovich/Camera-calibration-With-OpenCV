#ifndef CALIBRATIONPROCESSOR_H
#define CALIBRATIONPROCESSOR_H

#include <QString>
#include <QPixmap>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/persistence.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include "filesystem.h"

class CalibrationProcessor
{
public:
    CalibrationProcessor();

    void accumulationVectorsImg(cv::Mat inputFrame);
    void calibrationChessboardMethod(cv::Mat inputFrame);
private:
    int CHECKERBOARD_[2]; //размер шахматной доски убрать автоэкспозицию!
    std::vector<std::vector<cv::Point3f> > objpoints_; //Вектор, для хранения векторов 3d точек для каждого изображения шахматной доски
    std::vector<std::vector<cv::Point2f> > imgpoints_; //Вектор, для хранения векторов 2d точек для каждого изображения шахматной доски
    cv::Mat cameraMatrix_;
    cv::Mat distCoeffs_;
    cv::Mat R_;
    cv::Mat T_;

};

#endif // CALIBRATIONPROCESSOR_H
