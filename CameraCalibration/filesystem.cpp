#include "filesystem.h"

FileSystem::FileSystem()
{

}
//Открыть файлы для просмотра и вывести
void FileSystem::openFileInView(QString pathName)
{

        QRegExp tagExp("/");
        QStringList firstList = pathName.split(tagExp);
        QStringList splitname = firstList.back().split(".");
        if(splitname.back()=="jpg" || splitname.back()=="bmp" || splitname.back()=="png")
        {
            QImage img(pathName);
            //cv::Mat mt = cv::imread(pathName.toStdString());
            //QImage imgcam.;//((uchar*)mt.data,mt.cols,mt.rows,mt.step,QImage::Format_RGB888);

            emit outImgDisplay(QPixmap::fromImage(img));
        }
        if(splitname.back()=="YAML")
        {

        }
        emit outTextDisplay(splitname.back());



}
