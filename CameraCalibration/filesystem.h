#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/persistence.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <QObject>
#include <QDir>
#include <QPixmap>
#include <string>
#include <QFile>
#include <QCheckBox>
#include <QTableWidget>
#include <QDebug>

class FileSystem : public QObject
{
    Q_OBJECT
public:
    FileSystem();
    QString getFilePath();

    void createWorkDir(int countCam);
    void copyDirImgInWorkDir(QString path);
    void openFileInView(QString filePath);
    void openDrawImgInView(QString name);
    void openUndistImgInView(QString name);

    void readYamlMatrixFirst(cv::Mat* cameraMatrix);
    void readYamlMatrixSecond(cv::Mat* cameraMatrix);

    void readYamldistCoefFirst(cv::Mat* distCoeffs);
    void readYamldistCoefSecond(cv::Mat* distCoeffs);

    void readYamldistCoef(QString path, cv::Mat* distCoeffs);
    void saveFileInYaml(std::vector<std::vector<cv::Point3f>> objpoints, std::vector<std::vector<cv::Point2f>> imgpoints,
                        cv::Mat cameraMatrix,cv::Mat distCoeffs, std::vector<cv::Mat> R, std::vector<cv::Mat> T,
                        int countImg, QString date, double rmse, QString pattern, std::string sizePatern, int numCam, cv::Size size);

    void saveFileInYamlStereo(cv::Mat cameraMatrix0,cv::Mat cameraMatrix1,cv::Mat distCoeffs0, cv::Mat distCoeffs1,
                            cv::Mat R, cv::Mat T, cv::Mat E, cv::Mat F,
                            cv::Mat R1, cv::Mat R2, cv::Mat P1,cv::Mat P2, cv::Mat Q,
                            cv::Size imageSize, cv::Rect validRoi0,cv::Rect validRoi1,
                            cv::Mat rmap00,cv::Mat rmap01,cv::Mat rmap10,cv::Mat rmap11, double rms,double ReprojErr,
                            QString date, QString pattern,std::string sizePatern, int countPair);

    void writeSettingCalibInYaml(int subIter, int flagsIntFirst, QString lagsNameFirst,
                                 int flagsIntSecond, QString flagsNameSecond, bool isCalibration,bool isStereo);
    void saveInImg(QPixmap qpixmap, QString name);
    void saveInImgDrawing(QPixmap qpixmap, QString fileName,int numCam);
    void createImgUndistorted(cv::Mat cameraMatrix, cv::Mat distCoeffs,int numCam);
    void getOneTableItemsinTableCompare();//+
    void getTableItemsinTableCompare();

    QString getPattern();
    int getRow();
    int getCol();
    std::vector<std::vector<cv::Point3f>> getObjPoints();
    std::vector<std::vector<cv::Point2f>> getImgPoints(int numCam);
    cv::Mat getCameraMatrix(int numCam);
    cv::Mat getDistMatrix(int numCam);
    cv::Size getSizeImg();
    double getCheckerSize();
    double getMarkerSize();
    QString getDictionaryName();
    int getSubPixIter();
    int getCalibFlags();
    int  countImgInDir(QString path);
    bool isSelectedDir();
    bool isValidOpenDir();//+
    void setPath(QString path);//+
    void writeSettingCalibInYaml(int numCamFirst, QString nameCumFirst, int numCamSecond,QString nameCumSecond,
                                 QString pattern, int row, int col,
                                 double checkerSize, double markerSize, QString dictionaryName);
    int getIndexCameraFirst();
    int getIndexCameraSecond();
    int isCalibration();
    int isStereoCalibration();

    cv::Mat getRmap00();
    cv::Mat getRmap01();
    cv::Mat getRmap10();
    cv::Mat getRmap11();
    QVector<QString> getVectorPath(int numCamera);

    bool isFirstCameraResul();
    bool isSecondCameraResul();
    bool isStereoCameraResul();
signals:
    void outImgDisplayFirst(QPixmap pixmap);
    void outImgDisplaySecond(QPixmap pixmap);
    void outTextDisplayYamlCalib(QString qstring);
    void outTableItems(QTableWidgetItem *item, QTableWidgetItem *item1, QTableWidgetItem *item2);
    void outTableItemsCompare(QTableWidgetItem* itemFile,QTableWidgetItem* itemDate,
                              QTableWidgetItem* itemCount,QTableWidgetItem* itemPattern,
                              QTableWidgetItem* itemSizePattern, QTableWidgetItem* itemRmse,
                              QTableWidgetItem* itemFlags);
public slots:

    void getTableItems();
    void openFileInViewYamlCalib(QString path);//+

private:
    QString filePath_;
};

#endif // FILESYSTEM_H
