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
#include <QHBoxLayout>
#include <QApplication>

class FileSystem : public QObject
{
    Q_OBJECT

public:
    class SettingCalibration{
        public:
        SettingCalibration()
        {
            isUseParametr = false;
            isFixedFocal = false;
            isFixedAspectRatio = false;
            isFixedPrincipalPoint = false;
            isfixedK1 = false;
            isfixedK2 = false;
            isfixedK3 = false;
            isfixedK4 = false;
            iszeroTangent = false;
            isCaibration = false;
            isStereoCaibration = false;
            numCamera = 0;
            cameraModel = OPENCV;
            fx = 0;
            fy = 0;
            cx = 0;
            cy = 0;
            k1 = 0;
            k2 = 0;
            k3 = 0;
            k4 = 0;

            isSameFocalLengStereo = false;
            isfixAspectRatioStereo = false;
            isfixFocalLengthStereo = false;
            isfixPrincipalPointStereo = false;
            isUseSingleCalibratedInStereo = false;
            iszeroTangentS = false;
            isfixedK1S= false;
            isfixedK2S= false;
            isfixedK3S= false;
            fx1 = 0;
            fy1 = 0;
            fx2 = 0;
            fy2 = 0;
            cx1 = 0;
            cy1 = 0;
            cx2 = 0;
            cy2 = 0;
            k1S1 = 0;
            k2S1 = 0;
            k3S1 = 0;
            k1S2 = 0;
            k2S2 = 0;
            k3S2 = 0;
        }

        enum CameraModel{
            OPENCV,
            OPENCV_FISHEYE
        };

        void write(cv::FileStorage& fs) const                        //Write serialization for this class
        {
            fs << "{"
               << "isUseParametr" << isUseParametr
               << "isFixedFocal" << isFixedFocal
               << "isFixedAspectRatio" << isFixedAspectRatio
               << "isFixedPrincipalPoint" << isFixedPrincipalPoint
               << "isfixedK1" << isfixedK1
               << "isfixedK2" << isfixedK2
               << "isfixedK3" << isfixedK3
               << "isfixedK4" << isfixedK4
               << "iszeroTangent" << iszeroTangent
               << "isCaibration" << isCaibration
               << "isStereoCaibration" << isStereoCaibration
               << "numCamera" << numCamera
               << "cameraModel" << cameraModel
               << "fx" << fx
               << "fy" << fy
               << "cx" << cx
               << "cy" << cy
               << "k1" << k1
               << "k2" << k2
               << "k3" << k3
               << "k4" << k4

               << "isSameFocalLengStereo" << isSameFocalLengStereo
               << "isfixAspectRatioStereo" << isfixAspectRatioStereo
               << "isfixFocalLengthStereo" << isfixFocalLengthStereo
               << "isfixPrincipalPointStereo" << isfixPrincipalPointStereo
               << "isUseSingleCalibratedInStereo" << isUseSingleCalibratedInStereo
               << "iszeroTangentS" << iszeroTangentS
               << "isfixedK1S" << isfixedK1S
               << "isfixedK2S" << isfixedK2S
               << "isfixedK3S" << isfixedK3S
               << "fx1" << fx1
               << "fy1" << fy1
               << "fx2" << fx2
               << "fy2" << fy2
               << "cx1" << cx1
               << "cy1" << cy1
               << "cx1" << cx1
               << "cx2" << cx2
               << "cy2" << cy2
               << "k1S1" << k1S1
               << "k2S1" << k2S1
               << "k3S1" << k3S1
               << "k1S2" << k1S2
               << "k2S2" << k2S2
               << "k3S2" << k3S2
               << "}";
        }

        void read(const cv::FileNode& node)                          //Read serialization for this class
        {
            isUseParametr = (int)node["isUseParametr"];
            isFixedFocal = (int)node["isFixedFocal"];
            isFixedAspectRatio = (int)node["isFixedAspectRatio"];
            isFixedPrincipalPoint = (int)node["isFixedPrincipalPoint"];
            isfixedK1 = (int)node["isfixedK1"];
            isfixedK2 = (int)node["isfixedK2"];
            isfixedK3 = (int)node["isfixedK3"];
            isfixedK4 = (int)node["isfixedK4"];
            iszeroTangent = (int)node["iszeroTangent"];
            isCaibration = (int)node["isCaibration"];
            isStereoCaibration = (int)node["isStereoCaibration"];

            numCamera = (int)node["numCamera"];

            int intCameraModel;
            intCameraModel = (int)node["cameraModel"];
            switch (intCameraModel)
            {
            case 0:
            {
                cameraModel = OPENCV;
                break;
            }
            case 1:
            {
                cameraModel = OPENCV_FISHEYE;
                break;
            }
            }

            fx = (double)node["fx"];
            fy = (double)node["fy"];
            cx = (double)node["cx"];
            cy = (double)node["cy"];
            k1 = (double)node["k1"];
            k2 = (double)node["k2"];
            k3 = (double)node["k3"];
            k4 = (double)node["k4"];

            isUseSingleCalibratedInStereo = (int)node["isUseSingleCalibratedInStereo"];
            isSameFocalLengStereo = (int)node["isSameFocalLengStereo"];
            isfixFocalLengthStereo = (int)node["isfixFocalLengthStereo"];
            isfixAspectRatioStereo = (int)node["isfixAspectRatioStereo"];
            isfixPrincipalPointStereo = (int)node["isfixPrincipalPointStereo"];
            iszeroTangentS = (int)node["iszeroTangentS"];
            isfixedK1S = (int)node["isfixedK1S"];
            isfixedK2S = (int)node["isfixedK2S"];
            isfixedK3S = (int)node["isfixedK3S"];

            fx1 = (double)node["fx1"];
            fy1 = (double)node["fy1"];
            fx2 = (double)node["fx2"];
            fy2 = (double)node["fy2"];
            cx1 = (double)node["cx1"];
            cy1 = (double)node["cy1"];
            cx2 = (double)node["cx2"];
            cy2 = (double)node["cy2"];

            k1S1 = (double)node["k1S1"];
            k2S1 = (double)node["k2S1"];
            k3S1 = (double)node["k3S1"];
            k1S2 = (double)node["k1S2"];
            k2S2 = (double)node["k2S2"];
            k3S2 = (double)node["k3S2"];
        }
        public:
            bool isUseParametr;
            bool isFixedFocal;
            bool isFixedAspectRatio;
            bool isFixedPrincipalPoint;
            bool isfixedK1;
            bool isfixedK2;
            bool isfixedK3;
            bool isfixedK4;
            bool iszeroTangent;
            bool isCaibration;
            bool isStereoCaibration;
            unsigned short int numCamera;
            CameraModel cameraModel;
            double fx;
            double fy;
            double cx;
            double cy;
            double k1;
            double k2;
            double k3;
            double k4;

            bool isSameFocalLengStereo;
            bool isfixAspectRatioStereo;
            bool isfixFocalLengthStereo;
            bool isfixPrincipalPointStereo;
            bool isUseSingleCalibratedInStereo;
            bool iszeroTangentS;
            bool isfixedK1S;
            bool isfixedK2S;
            bool isfixedK3S;
            double fx1;
            double fy1;
            double fx2;
            double fy2;
            double cx1;
            double cy1;
            double cx2;
            double cy2;
            double k1S1;
            double k2S1;
            double k3S1;
            double k1S2;
            double k2S2;
            double k3S2;
    };

    class InformationImageSaved{
    public:
        InformationImageSaved(){
            cameraPath = "NONE";
            drawPath = "NONE";
            undistortedPath = "NONE";
            state = "NONE";
            err = 0;
            isActive = 1;
            isCalib = 0;
            cv::Point2f point;
            point.x = 0;
            point.y = 0;
            imgpoint.push_back(point);
            reprojectPoint.push_back(point);
        }

        void write(cv::FileStorage& fs) const                        //Write serialization for this class
        {
            fs << "{" << "isActive" << isActive
                      << "isCalib" << isCalib
                      << "cameraPath" << cameraPath << "drawPath" << drawPath
                      << "undistortedPath" << undistortedPath
                      <<"state" << state << "err" << err
                      << "imgpoint" <<"[";
                      for(unsigned short int i = 0; i < imgpoint.size(); ++i)
                      {
                        fs<<"[:" <<(float)imgpoint[i].x << (float)imgpoint[i].y << "]";
                      }
                      fs << "]";
                      fs << "reprojectPoint" <<"[";
                      for(unsigned short int i = 0; i < reprojectPoint.size(); ++i)
                      {
                        fs<<"[:" <<(float)reprojectPoint[i].x << (float)reprojectPoint[i].y << "]";
                      }
                      fs << "]" ;

                      fs << "charucoIds" << charucoIds;
                      fs << "}";
        }

        void read(const cv::FileNode& node)                          //Read serialization for this class
        {
            isActive = (int)node["isActive"];
            isCalib = (int)node["isCalib"];
            cameraPath = (std::string)node["cameraPath"];
            drawPath = (std::string)node["drawPath"];
            undistortedPath = (std::string)node["undistortedPath"];
            state = (std::string)node["state"];
            err = (double)node["err"];


            std::vector<cv::Point2f> temp;
            for(cv::FileNodeIterator itPts = node["imgpoint"].begin();itPts!=node["imgpoint"].end();++itPts)
            {
                cv::FileNode pt = *itPts;
                cv::Point2f point;
                cv::FileNodeIterator itPt = pt.begin();
                point.x = *itPt; ++itPt;
                point.y = *itPt; ++itPt;
                temp.push_back(point);
            }
            imgpoint = temp;

            std::vector<cv::Point2f> temp2;
            for(cv::FileNodeIterator itPts = node["reprojectPoint"].begin();itPts!=node["reprojectPoint"].end();++itPts)
            {
                cv::FileNode pt = *itPts;
                cv::Point2f point;
                cv::FileNodeIterator itPt = pt.begin();
                point.x = *itPt; ++itPt;
                point.y = *itPt; ++itPt;
                temp2.push_back(point);
            }
            reprojectPoint = temp2;

            std::vector<int> temp3;
            for(cv::FileNodeIterator itPts = node["charucoIds"].begin();itPts!=node["charucoIds"].end();++itPts)
            {
                cv::FileNode pt = *itPts;
                int id;
                cv::FileNodeIterator itPt = pt.begin();
                id = *itPt;
                temp3.push_back(id);
            }
            charucoIds = temp3;
        }
    public:
        int isActive;
        int isCalib;
        std::string cameraPath;
        std::string drawPath;
        std::string undistortedPath;
        std::string state;
        double err;
        std::vector<cv::Point2f> imgpoint;
        std::vector<cv::Point2f> reprojectPoint;
        std::vector<int> charucoIds;
    };

    class TempatesPattern{
    public:
        TempatesPattern(){
            pattern = "NONE";
            dictionaryName = "NONE";
            row = -1;
            col = -1;
            checker = -1;
            marker = -1;
        }

        void write(cv::FileStorage& fs) const                        //Write serialization for this class
        {
            fs << "{" <<"pattern" << pattern << "dictionaryName" << dictionaryName
               << "row" << row << "col" << col <<"checker" << checker << "marker" << marker << "}";
        }

        void read(const cv::FileNode& node)                          //Read serialization for this class
        {
            pattern = (std::string)node["pattern"];
            dictionaryName = (std::string)node["dictionaryName"];
            row = (int)node["row"];
            col = (int)node["col"];
            checker = (double)node["checker"];
            marker = (double)node["marker"];
        }
    public:
        std::string pattern;
        std::string dictionaryName;
        int row;
        int col;
        double checker;
        double marker;
    };
public:
    FileSystem();
    QString getFilePath();

    void createWorkDir(int countCam);
    void copyDirImgInWorkDir(QString path1,QString path2);
    void openFileInView(int, bool isChecked);
    void openDrawImgInView(int);
    void openUndistImgInView(int);

    void readYamlMatrixFirst(cv::Mat* cameraMatrix);
    void readYamlMatrixSecond(cv::Mat* cameraMatrix);

    void readYamldistCoefFirst(cv::Mat* distCoeffs);
    void readYamldistCoefSecond(cv::Mat* distCoeffs);

    void readYamldistCoef(QString path, cv::Mat* distCoeffs);
    void saveFileInYaml(std::vector<std::vector<cv::Point3f>> objpoints, std::vector<std::vector<cv::Point2f>> imgpoints,
                        cv::Mat cameraMatrix,cv::Mat distCoeffs, std::vector<cv::Mat> R, std::vector<cv::Mat> T,
                        int countImg, QString date, double rmse, double meanErr,QString pattern, std::string sizePatern, int numCam, cv::Size size);

    void saveFileInYamlStereo(cv::Mat cameraMatrix0,cv::Mat cameraMatrix1,cv::Mat distCoeffs0, cv::Mat distCoeffs1,
                            cv::Mat R, cv::Mat T, cv::Mat E, cv::Mat F,
                            cv::Mat R1, cv::Mat R2, cv::Mat P1,cv::Mat P2, cv::Mat Q,
                            cv::Size imageSize,double rms,double ReprojErr,
                            QString date, QString pattern,std::string sizePatern, int countPair);

    void writeSettingCalibInYaml(int subIter, int flagsIntFirst, QString lagsNameFirst,
                                 int flagsIntSecond, QString flagsNameSecond, bool isCalibration,bool isStereo);
    void createSettingDefault();
    void saveInImg(QPixmap qpixmap, QString name);
    void saveInImgDrawing(QPixmap qpixmap, QString fileName,int numCam);
    void getOneTableItemsinTableCompare(QString localPath);
    void getTableItemsinTableCompare();

    std::vector<InformationImageSaved> getInfoCamera1();
    std::vector<InformationImageSaved> getInfoCamera2();
    std::vector<TempatesPattern> getTempatesPattern();

    double getExpose();
    double getGain();
    double getRmse(int numCam);
    double getStereoRmse();
    std::vector<cv::Mat>getRvec(int numCam);
    std::vector<cv::Mat>getTvec(int numCam);
    cv::Mat getR1();
    cv::Mat getR2();
    cv::Mat getP1();
    cv::Mat getP2();



    double getMeanErr(int numCam);
    double getCountImg(int numCam);
    //double getRmseStereo();

    bool isWebCamera();
    bool isBaslerCamera();

    void saveInfoCamera1(std::vector<InformationImageSaved> imageInfo1);
    void saveInfoCamera2(std::vector<InformationImageSaved> imageInfo2);

    void zeroingCalibInfoCamera1();
    void zeroingCalibInfoCamera2();

    void saveTempatesPattern(std::vector<TempatesPattern>);
    void saveCalibrationSetting(SettingCalibration setting);
    SettingCalibration getCalibrationSetting();

    void removeTemplates(int);

    void parseResultCalibr(int);

    QString getPattern();
    int getRow();
    int getCol();
    std::vector<std::vector<cv::Point3f>> getObjPoints();
    std::vector<std::vector<cv::Point2f>> getImgPoints(int numCam);
    cv::Mat getCameraMatrix(int numCam);
    bool isCalibResult(int numCam);
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
    bool isValidOpenDir(QString);
    void setPath(QString path);//+
    void writeSettingCalibInYaml(int numCamFirst, QString nameCumFirst, int numCamSecond,QString nameCumSecond,
                                 QString pattern, int row, int col,
                                 double checkerSize, double markerSize, QString dictionaryName,
                                 bool isWebCamera, bool isBaslerCamera);
    void cameraSettingSave(QString nameCumFirst, int numCamFirst,
                           QString nameCumSecond,int numCamSecond,
                           bool isWebCamera, bool isBaslerCamera,double,double);
    void patternSettingSave(QString, int,int,double,double,QString);
    void addTemplates(QString, int,int,double,double,QString);
    int getIndexCameraFirst();
    int getIndexCameraSecond();
    std::string getNameCameraFirst();
    std::string getNameCameraSecond();

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

    static void write(cv::FileStorage& fs, const std::string&, const TempatesPattern& x);
    static void read(const cv::FileNode& node, TempatesPattern& x, const TempatesPattern& default_value = TempatesPattern());

    static void write(cv::FileStorage& fs, const std::string&, const SettingCalibration& x);
    static void read(const cv::FileNode& node, SettingCalibration& x, const SettingCalibration& default_value = SettingCalibration());

signals:
    void outImgDisplayFirst(QPixmap pixmap);
    void outImgDisplaySecond(QPixmap pixmap);
    void outImgDisplayFirst(QPixmap pixmap, std::vector<cv::Point2f> imgpoint,bool isActive);
    void outImgDisplaySecond(QPixmap pixmap, std::vector<cv::Point2f> imgpoint,bool isActive);
    void outImgDisplayFirst(QPixmap pixmap, std::vector<cv::Point2f> imgpoint,
                            std::vector<cv::Point2f> reprojpoint,double err, bool isActive);
    void outImgDisplaySecond(QPixmap pixmap, std::vector<cv::Point2f> imgpoint,
                             std::vector<cv::Point2f> reprojpoint,double err, bool isActive);
    void outTableItems(QTableWidgetItem *item0,QTableWidgetItem *item1, QTableWidgetItem *item2);
    void outTableItemsCompare(QTableWidgetItem* itemFile,
                              QTableWidgetItem* SettingCam1,QTableWidgetItem* itemCamera1Rmse,
                              QTableWidgetItem* SettingCam2, QTableWidgetItem* itemCamera2Rmse,
                              QTableWidgetItem* itemStereoRmse);
    void sendTerminalStr(QString);
    void updateCantrolUi();
public slots:
    void getTableItems();
    QString openProjectSetting();
    QString openCalibSetting(int numCam);//+
    QString openStereoSetting();

private:
    QString filePath_;
};

#endif // FILESYSTEM_H
