#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/persistence.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>


#include <QObject>
#include <QPixmap>

#define DIR_PATH "D:/PRoG/Git-repos/Camera-calibration-With-OpenCV/SaveResult"

class FileSystem : public QObject
{
    Q_OBJECT
public:
    FileSystem();

    void openFileInView(QString filePath);

signals:
    void outImgDisplay(QPixmap pixmap);
    void outTextDisplay(QString qstring);
};

#endif // FILESYSTEM_H
