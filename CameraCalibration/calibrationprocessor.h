#ifndef CALIBRATIONPROCESSOR_H
#define CALIBRATIONPROCESSOR_H

#include <QString>
#include <QPixmap>
#include <QObject>
#include <QDate>
#include <string>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/core/persistence.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>


#include <QThread>
#include "filesystem.h"


class CalibrationProcessor :public QThread
{
    Q_OBJECT
public:
    CalibrationProcessor();

    void setVectorPathImg(QVector<QString> vector);

    void setPath(QString path);

    bool getFrameFromTable(int row);

    void reloadVectors();

    void cameraCalibration();

    bool isFramePattern(cv::Mat* frame,QString pattern,int row, int col);

    double Rmse();
signals:
    void sendStatusImg(QString status, int row);
    void sendOpenFileInViewYamlCalib(QString filepath);
public slots:

    void run() override;

    void setTargetType(QString qstring);
    void setTargetSize(int row,int col);
    void setSubPixIter(int count);
private:
    int subPixelIter_;
    int CHECKERBOARD_[2]; //размер шахматной доски убрать автоэкспозицию!
    QString targetType_;
    cv::Mat inputFrame_;
    cv::Mat gray;
    cv::Mat cameraMatrix_;
    cv::Mat distCoeffs_;
    std::vector<std::vector<cv::Point3f> > objpoints_; //Вектор, для хранения векторов 3d точек для каждого изображения шахматной доски
    std::vector<std::vector<cv::Point2f> > imgpoints_; //Вектор, для хранения векторов 2d точек для каждого изображения шахматной доски
    std::vector<cv::Mat> R_;
    std::vector<cv::Mat> T_;
    QVector<QString> vectorPathImg_;
    FileSystem fs_;
};

#endif // CALIBRATIONPROCESSOR_H
