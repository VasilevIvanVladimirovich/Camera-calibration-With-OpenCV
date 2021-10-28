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

#define TEMP_PATH "D:/PRoG/Git-repos/Camera-calibration-With-OpenCV/Temp/"


class FileSystem : public QObject
{
    Q_OBJECT
public:
    FileSystem();

    void openFileInViewYamlCalib(QString pathName);
    void openFileInView(QString filePath);
    void openFileInView(int row);
    void readYamlMatrix(QString path, cv::Mat* cameraMatrix);
    void readYamldistCoef(QString path, cv::Mat* distCoeffs);
    void saveResult(QPixmap qpixmap, cv::Mat cameraMatrix,cv::Mat distCoeffs, cv::Mat R, cv::Mat T);
    void saveFileInYaml(cv::Mat cameraMatrix,cv::Mat distCoeffs, std::vector<cv::Mat> R, std::vector<cv::Mat> T, QString name);
    void saveInImg(QPixmap qpixmap, QString name);
    int  countImgInDir(QString path);
    QString getFilePath();

signals:
    void outImgDisplay(QPixmap pixmap);
    void outTextDisplayYamlCalib(QString qstring);
    void outTableItems(QTableWidgetItem *item1,QTableWidgetItem *item2);

public slots:
    void setPath(QString path);
    void getTableItems();

private:
    QString filePath;
};

#endif // FILESYSTEM_H
