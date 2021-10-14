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
            emit outImgDisplay(QPixmap::fromImage(img));
        }
        if(splitname.back()=="YAML")
        {
            std::string output="";
            cv::Mat cameraMatrix,distCoeffs,R,T;
            QString fileResultPath = pathName;

                    cv::FileStorage fs(fileResultPath.toStdString(),cv::FileStorage::READ);
                    fs["cameraMatrix"] >> cameraMatrix;
                    fs["distCoeffs"] >> distCoeffs;
//                    fs["R"]  >> R;
//                    fs["T"] >> T;
                    output += "CameraMatrix: \n";
                    output += "[";
                    for(int i=0; i<3; i++)
                        {
                           for(int j=0; j<3; j++)
                           {
                               output += std::to_string(cameraMatrix.at<double>(i,j)) + ", \t";
                           }
                         output += "\n";
                        }
                    output += "]";
                    output += "\n DistCoeffs: \n";
                    output += "[";
                           for(int i=0; i<5; i++)
                           {
                               output += std::to_string(distCoeffs.at<double>(i)) + ", ";
                           }
                    output += "]";
                    emit outTextDisplay(QString::fromStdString(output));
                    fs.release();
        }




}

void FileSystem::saveResult(cv::Mat img, cv::Mat cameraMatrix, cv::Mat distCoeffs, cv::Mat R, cv::Mat T)
{
    QDir dir(DIR_PATH);
    int experimentNumber;
    if(dir.entryInfoList(QDir::Files).isEmpty())  experimentNumber = 1;
    else{
        QFileInfoList list = dir.entryInfoList(QDir::Files);
        QString  name = list.back().baseName();
        experimentNumber = name.toInt() + 1;
    }

    QString fullNameYaml = DIR_PATH + QString::number(experimentNumber) + ".YAML";
    QString fullNameJpg = DIR_PATH + QString::number(experimentNumber) + ".jpg";

    saveFileInYaml(cameraMatrix, distCoeffs, R, T, fullNameYaml);
    saveInImg(img, fullNameJpg);
}

void FileSystem::saveFileInYaml(cv::Mat cameraMatrix, cv::Mat distCoeffs, cv::Mat R, cv::Mat T, QString name)
{
    std::string fileResultPath = name.toStdString();
            cv::FileStorage fs(fileResultPath,cv::FileStorage::WRITE);
            fs << "cameraMatrix" << cameraMatrix
               << "distCoeffs"   << distCoeffs
               << "R"   << R
               << "T"   << T;
            fs.release();
}

void FileSystem::saveInImg(cv::Mat img, QString name)
{
    cv::imwrite(name.toStdString(), img);
}
