#ifndef FILESYSTEMPROCESSOR_H
#define FILESYSTEMPROCESSOR_H

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/persistence.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <QThread>
#include <QString>
#include <QPixmap>
#include <QImage>

#define DIR_PATH "D:/PRoG/Git-repos/Camera-calibration-With-OpenCV/SaveResult"

class FileSystemProcessor : public QThread
{
   Q_OBJECT
public:
    FileSystemProcessor();
//    QString saveIn(cv::Mat frame, QVector<cv::Mat> info);
//    void openFileInView(QString filePath);

//signals:
//    void outImgDisplay(QPixmap pixmap);
//    void outTextDisplay(QString qstring);

//private:
//    int experimentNumber_;

};

#endif // FILESYSTEMPROCESSOR_H
