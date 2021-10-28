#ifndef CALIBRATIONPROCESSOR_H
#define CALIBRATIONPROCESSOR_H

#include <QString>
#include <QPixmap>
#include <QObject>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/core/persistence.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include "filesystem.h"

#define TEMP_PATH "D:/PRoG/Git-repos/Camera-calibration-With-OpenCV/Temp/"

class CalibrationProcessor :public QObject
{
    Q_OBJECT
public:
    CalibrationProcessor(QObject *parent = nullptr);

    void accumulationVectorsImg();
    void reloadVectors();
    bool getFrameFromTable(int row);
    void cameraCalibrationChessboardMethod();
    void setVectorPathImg(QVector<QString> vector);
    double computeReprojectionErrors();

signals:
    void sendStatusImg(QString status, int row);
public slots:
    void setTargetType(QString qstring);
    void setTargetSize(int row,int col);
    void setSubPixIter(int count);
private:
    QVector<QString> vectorPathImg_;
    QString targetType_;
    int subPixelIter_;
    int CHECKERBOARD_[2]; //размер шахматной доски убрать автоэкспозицию!
    std::vector<std::vector<cv::Point3f> > objpoints_; //Вектор, для хранения векторов 3d точек для каждого изображения шахматной доски
    std::vector<std::vector<cv::Point2f> > imgpoints_; //Вектор, для хранения векторов 2d точек для каждого изображения шахматной доски
    cv::Mat inputFrame_;
    cv::Mat gray;
    cv::Mat cameraMatrix_;
    cv::Mat distCoeffs_;
    std::vector<cv::Mat> R_;
    std::vector<cv::Mat> T_;

};

#endif // CALIBRATIONPROCESSOR_H
