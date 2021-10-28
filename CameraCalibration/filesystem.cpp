#include "filesystem.h"

FileSystem::FileSystem()
{

}

QString FileSystem::getFilePath()
{
    return filePath;
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
            emit outImgDisplay(QPixmap::fromImage(img));
        }


        //кусок кода, который выводит значения из файла YAML
//        if(splitname.back()=="YAML")
//        {
//            std::string output="";
//            cv::Mat cameraMatrix,distCoeffs,R,T;
//            QString fileResultPath = pathName;

//                    cv::FileStorage fs(fileResultPath.toStdString(),cv::FileStorage::READ);
//                    fs["cameraMatrix"] >> cameraMatrix;
//                    fs["distCoeffs"] >> distCoeffs;
//                    output += "CameraMatrix: \n";
//                    output += "[";
//                    for(int i=0; i<3; i++)
//                        {
//                           for(int j=0; j<3; j++)
//                           {
//                               output += std::to_string(cameraMatrix.at<double>(i,j)) + ", \t";
//                           }
//                         output += "\n";
//                        }
//                    output += "]";
//                    output += "\n DistCoeffs: \n";
//                    output += "[";
//                           for(int i=0; i<5; i++)
//                           {
//                               output += std::to_string(distCoeffs.at<double>(i)) + ", ";
//                           }
//                    output += "]";
//                    emit outTextDisplay(QString::fromStdString(output));
//                    fs.release();
//        }

}

void FileSystem::openFileInView(int row)
{
        QString pathName;
        pathName = TEMP_PATH + QString::number(row) + ".png";
        QImage img(pathName);
        emit outImgDisplay(QPixmap::fromImage(img));

}

void FileSystem::saveResult(QPixmap qpixmap, cv::Mat cameraMatrix, cv::Mat distCoeffs, cv::Mat R, cv::Mat T)
{
    QDir dir(filePath);
    int experimentNumber;
    if(dir.isEmpty())  experimentNumber = 1;
    else{
        dir.count();
        experimentNumber = dir.count()/2 + 1;
    }

    QString fullNameYaml = filePath + QString::number(experimentNumber) + ".YAML";
    QString fullNameJpg = filePath + QString::number(experimentNumber) + ".jpg";

    saveFileInYaml(cameraMatrix, distCoeffs, R, T, fullNameYaml);
    saveInImg(qpixmap, fullNameJpg);
}

void FileSystem::saveFileInYaml(cv::Mat cameraMatrix, cv::Mat distCoeffs, cv::Mat R, cv::Mat T, QString name)
{
    name = TEMP_PATH + name + ".YAML";
    std::string fileResultPath = name.toStdString();
            cv::FileStorage fs(fileResultPath,cv::FileStorage::WRITE);
            fs << "cameraMatrix" << cameraMatrix
               << "distCoeffs"   << distCoeffs
               << "R"   << R
               << "T"   << T;
            fs.release();
}

void FileSystem::saveInImg(QPixmap qpixmap, QString path)
{
    //name = TEMP_PATH + name + ".png";
    QFile file(path);
    file.open(QIODevice::WriteOnly);
    qpixmap.save(&file, "png");
    file.close();
}

int FileSystem::countImgInDir(QString path)
{
    QDir dir(path);
    QStringList filters;
    filters<<"*jpg";

    return dir.entryInfoList(filters,QDir::Files).size();
}

void FileSystem::setPath(QString path)
{
    filePath = path;
}

void FileSystem::getTableItems()
{
    QDir dir(filePath);
    QFileInfoList fileList;
    fileList = dir.entryInfoList();
    fileList.count();
    QString status="";

    for(int i= 2; i < fileList.count(); i++)
    {
        QTableWidgetItem *item = new QTableWidgetItem();
        QTableWidgetItem *item1 = new QTableWidgetItem(fileList[i].absoluteFilePath());
        item->setCheckState(Qt::Unchecked);
        emit outTableItems(item,item1);
    }
}
