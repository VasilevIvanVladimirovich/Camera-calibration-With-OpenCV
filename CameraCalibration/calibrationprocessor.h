#ifndef CALIBRATIONPROCESSOR_H
#define CALIBRATIONPROCESSOR_H

#include <QString>
#include <QPixmap>
#include <QObject>
#include <QDate>
#include <string>
#include <QMessageBox>
#include <string>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/core/persistence.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/aruco/charuco.hpp>
#include <opencv2/core/types.hpp>

#include <QThread>
#include "filesystem.h"

class CalibrationProcessor :public QThread
{
    Q_OBJECT
public:
    CalibrationProcessor(FileSystem* fs);
    CalibrationProcessor();

    enum State{
        ACCUMULATION,
        CALIBRATION
    };

    void setState(State state);

    void setPattern(QString Pattern);
    void setRowCol(int row, int col);
    void setMarkerSize(double markerSize);
    void setCheckerSize(double checkerSize);
    void setDictionaryName(QString dictionaryName);
    void setCalibrationFlags(int calibrationFlags);
    bool getFrameFromTable(int row);
    void reloadVectors();
    int translateFlagsOpencv(QString textFlag);
    int translateFlagsFisheye(QString textFlag);
    void cameraCalibration(std::vector<FileSystem::InformationImageSaved>& imageInfo, FileSystem::SettingCalibration&);
    void stereoCalibration();
    void accumulation(std::vector<FileSystem::InformationImageSaved>& imageInfo);
    void charucoAccumulation(int i,std::vector<FileSystem::InformationImageSaved>& imageInfo);
    void chessboardAccumulation(int i,cv::Mat gray,
                                std::vector<cv::Point2f> corner_pts,
                                std::vector<FileSystem::InformationImageSaved>& imageInfo);
    void circleAccumulation(int i,cv::Mat gray,
                            std::vector<cv::Point2f> corner_pts,
                            std::vector<FileSystem::InformationImageSaved>& imageInfo);
    void aCircleAccumulation(int i,cv::Mat gray,
                             std::vector<cv::Point2f> corner_pts,
                             std::vector<FileSystem::InformationImageSaved>& imageInfo);
    bool isFramePattern(cv::Mat* frame,QString pattern,int row, int col,
                        double icheckerSize, double imarkerSize, int idictionary);
    void createImgUndistorted(std::vector<FileSystem::InformationImageSaved>& imageInfo,
                              cv::Mat cameraMatrix,cv::Mat disCoeffs,
                              int numCam);

    void saveInImgDrawing(QPixmap qpixmap, QString fileName,int i,
                          std::vector<FileSystem::InformationImageSaved>& imageInfo);

    double Rmse(std::vector<FileSystem::InformationImageSaved>& imageInfo,
                std::vector<std::vector<cv::Point2f>> imgpoints,
                std::vector<std::vector<cv::Point3f>> objpoints,
                cv::Mat cameraMatrix,cv::Mat disCoeffs,
                std::vector<cv::Mat> R,std::vector<cv::Mat> T,
                std::vector<int> indexImages);
signals:
    void sendStatusImg(QString status, int row);
    void sendCalibBrowser();
    void updateCantrolUi();
public slots:
    void run() override;
    void setTargetType(QString qstring);
    void setTargetSize(int row,int col, double markerSize, double checkerSize, QString dictionaryName);
private:
    bool isRefindStrategy_;
    int calibrationFlags_;
    int intDictionary_;
    int CHECKERBOARD_[2]; //размер шахматной доски убрать автоэкспозицию!
    double markerSize_;
    double checkerSize_;
    double rmse_;
    QString targetType_;
    QString dictionaryName_;
    cv::Size imgSizeCharuco_;
    cv::Mat inputFrame_;

    cv::Ptr<cv::aruco::CharucoBoard> charucoboard_;
    cv::Ptr<cv::aruco::Dictionary> dictionary_;
    cv::Ptr<cv::aruco::DetectorParameters> params_;
    std::vector<std::vector<cv::Point2f>>  allCharucoCorners;
    std::vector<std::vector<int>> allCharucoIds;
    std::vector< cv::Mat > allImgs;
    FileSystem* fs_;

    State state_;
};

#endif // CALIBRATIONPROCESSOR_H
