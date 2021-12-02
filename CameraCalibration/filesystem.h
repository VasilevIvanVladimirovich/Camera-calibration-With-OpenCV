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

    void createWorkDir();

    void copyDirImgInWorkDir(QString path);

    void openFileInView(QString filePath);
    void openDrawImgInView(int row);
    void openUndistImgInView(int row);

    void readYamlMatrix(QString path, cv::Mat* cameraMatrix);
    void readYamldistCoef(QString path, cv::Mat* distCoeffs);

    void saveFileInYaml(cv::Mat cameraMatrix,cv::Mat distCoeffs, std::vector<cv::Mat> R, std::vector<cv::Mat> T,
                        int countImg, QString date, double rmse, QString pattern, int rowPattern, int colPattern);

    void saveInImg(QPixmap qpixmap, QString name);
    void saveInImgDrawing(QPixmap qpixmap, QString name);


    void createImgUndistorted(cv::Mat cameraMatrix, cv::Mat distCoeffs);

    void getOneTableItemsinTableCompare();
    void getTableItemsinTableCompare();

    int  countImgInDir(QString path);

    bool isSelectedDir();
    bool isValidOpenDir();

signals:
    void outImgDisplay(QPixmap pixmap);
    void outTextDisplayYamlCalib(QString qstring);
    void outTableItems(QTableWidgetItem *item, QTableWidgetItem *item1, QTableWidgetItem *item2);
    void outTableItemsCompare(QTableWidgetItem* itemFile,QTableWidgetItem* itemDate,
                              QTableWidgetItem* itemCount,QTableWidgetItem* itemPattern,
                              QTableWidgetItem* itemSizePattern, QTableWidgetItem* itemRmse);

public slots:
    void setPath(QString path);
    void getTableItems();
    void openFileInViewYamlCalib(QString path);

private:
    QString filePath_;
};

#endif // FILESYSTEM_H
