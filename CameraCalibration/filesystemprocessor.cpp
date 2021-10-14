#include "filesystemprocessor.h"

FileSystemProcessor::FileSystemProcessor()
{
//    QDir dir(FILE_PATH);
//    QFileInfoList list = dir.entryInfoList(QDir::Files);
//    QString  name = list.back().baseName();
//    experimentNumber_ += name.toInt();
}

//QString FileSystemProcessor::saveIn(cv::Mat frame, QVector<cv::Mat> info)
//{

//}

//Открыть файлы для просмотра и вывести
//void FileSystemProcessor::openFileInView(QString pathName)
//{
//    QRegExp tagExp("/");
//    QStringList firstList = pathName.split(tagExp);
//    QStringList splitname = firstList.back().split(".");
//    if(splitname.back()=="jpg" || splitname.back()=="bmp" || splitname.back()=="png")
//    {
//        cv::Mat mt = cv::imread(pathName.append(splitname.first()).append(".").append(splitname.back()).toStdString());
//        QImage imgcam((uchar*)mt.data,mt.cols,mt.rows,mt.step,QImage::Format_RGB888);
//        emit outImgDisplay(QPixmap::fromImage(imgcam));
//    }
//    if(splitname.back()=="YAML")
//    {

//    }


//}
