#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/persistence.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <QDebug>

#include <QObject>
#include <QDir>
#include <QPixmap>
#include <string>
#include <QFile>
#include <QCheckBox>
#include <QTableWidget>
#include <QDebug>
#include <map>

class FileSystem : public QObject
{
    Q_OBJECT

public:
    class InformationImageSaved{
    public:
        InformationImageSaved(){
            cameraPath = "NONE";
            drawPath = "NONE";
            undistortedPath = "NONE";
            state = "NONE";
            err = 0;
        }

        void write(cv::FileStorage& fs) const                        //Write serialization for this class
        {
            fs << "{" << "cameraPath" << cameraPath << "drawPath" << drawPath << "undistortedPath" << undistortedPath
                      <<"state" << state << "err" << err <<"}";
        }

        void read(const cv::FileNode& node)                          //Read serialization for this class
        {
            cameraPath = (std::string)node["cameraPath"];
            drawPath = (std::string)node["drawPath"];
            undistortedPath = (std::string)node["undistortedPath"];
            state = (std::string)node["state"];
            err = (double)node["err"];
        }
    public:
        std::string cameraPath;
        std::string drawPath;
        std::string undistortedPath;
        std::string state;
        double err;
    };
public:
    FileSystem();
    QString getFilePath();

    void createWorkDir(int countCam);
    void copyDirImgInWorkDir(QString path);
    void openFileInView(int);
    void openDrawImgInView(int);
    void openUndistImgInView(int);

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
    void getOneTableItemsinTableCompare();
    void getTableItemsinTableCompare();

    std::vector<InformationImageSaved> getInfoCamera1();
    std::vector<InformationImageSaved> getInfoCamera2();

    void saveInfoCamera1(std::vector<InformationImageSaved> imageInfo1);
    void saveInfoCamera2(std::vector<InformationImageSaved> imageInfo2);


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
                                 double checkerSize, double markerSize, QString dictionaryName,
                                 bool isWebCamera, bool isBaslerCamera);
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

    static void write(cv::FileStorage& fs, const std::string&, const InformationImageSaved& x);
    static void read(const cv::FileNode& node, InformationImageSaved& x, const InformationImageSaved& default_value = InformationImageSaved());

signals:
    void outImgDisplayFirst(QPixmap pixmap);
    void outImgDisplaySecond(QPixmap pixmap);
    void outTableItems(QTableWidgetItem *item1, QTableWidgetItem *item2);
    void outTableItemsCompare(QTableWidgetItem* itemFile,QTableWidgetItem* itemDate,
                              QTableWidgetItem* itemCount,QTableWidgetItem* itemPattern,
                              QTableWidgetItem* itemSizePattern, QTableWidgetItem* itemRmse,
                              QTableWidgetItem* itemFlags);
public slots:

    void getTableItems();
    QString openProjectSetting();
    QString openCalibSetting(int numCam);//+

private:
    QString filePath_;
};

#endif // FILESYSTEM_H
