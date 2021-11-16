#include "filesystem.h"

FileSystem::FileSystem()
{

}

QString FileSystem::getFilePath()
{
    return filePath_;
}

void FileSystem::createWorkDir()
{
    QDir dir;
    dir.mkpath(filePath_+"Accumulated");
    dir.mkpath(filePath_+"Drawnable");
    dir.mkpath(filePath_+"Result");
}

void FileSystem::copyDirImgInWorkDir(QString path)
{
    QDir dir(path);

    foreach (QString f, dir.entryList(QDir::Files)) {
    QFile::copy(path + QDir::separator()+ f,
                filePath_ + "Accumulated" + QDir::separator() + f);
    }
}

void FileSystem::openFileInViewYamlCalib()
{
    std::string output="";
    cv::Mat cameraMatrix,distCoeffs,R,T;
    std::string date;
    int countImg;
    double Rmse;
    QString fileResultPath = filePath_ + "Result" + QDir::separator() + "Result.YAML";
    cv::FileStorage fs(fileResultPath.toStdString(),cv::FileStorage::READ);
    fs["Date"] >> date;
    fs["CountImg"] >> countImg;
    fs["RMSE"] >> Rmse;
    fs["CameraMatrix"] >> cameraMatrix;
    fs["DistCoeffs"] >> distCoeffs;
    output += "Date:" + date + "\n";
    output += "CountImg:" + std::to_string(countImg) + "\n";
    output += "RMSE:" + std::to_string(Rmse) + "\n";
    output += "CameraMatrix: \n";
    output += "[";
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            output += std::to_string(cameraMatrix.at<double>(i,j)) + ", \t";
        }
        output += "\n";
    }
    output += "]";
    output += "\n DistCoeffs: \n";
    output += "[";
    for(int i=0; i<5; i++){
        output += std::to_string(distCoeffs.at<double>(i)) + ", ";
    }
    output += "]";
    emit outTextDisplayYamlCalib(QString::fromStdString(output));
    fs.release();
}

void FileSystem::openFileInView(QString pathName)
{

    QRegExp tagExp("/");
    QStringList firstList = pathName.split(tagExp);
    QStringList splitname = firstList.back().split(".");
    if(splitname.back() == "jpg" || splitname.back() == "bmp" || splitname.back() == "png"){
        QImage img(pathName);
        emit outImgDisplay(QPixmap::fromImage(img));
    }
}

void FileSystem::openDrawImgInView(int row)
{
    QString localPath;
    localPath = filePath_ +
                "Drawnable" + QDir::separator() + QString::number(row) + ".png";
    QImage img(localPath);
    emit outImgDisplay(QPixmap::fromImage(img));

}

void FileSystem::readYamldistCoef(QString path,cv::Mat* distCoeffs)
{
    cv::FileStorage fs(path.toStdString(),cv::FileStorage::READ);
    fs["distCoeffs"] >> *distCoeffs;
    fs.release();
}

void FileSystem::readYamlMatrix(QString path,cv::Mat* cameraMatrix)
{
    cv::FileStorage fs(path.toStdString(),cv::FileStorage::READ);
    fs["cameraMatrix"] >> *cameraMatrix;
    fs.release();
}


void FileSystem::saveFileInYaml(cv::Mat cameraMatrix, cv::Mat distCoeffs, std::vector<cv::Mat> R, std::vector<cv::Mat> T,
                                int countImg,QString date,double rmse)
{
    QString name;
    name = filePath_ + "Result/" + "Result" + ".YAML";
    std::string fileResultPath = name.toStdString();
    cv::FileStorage fs(fileResultPath,cv::FileStorage::WRITE);
    fs << "Date" << date.toStdString()
       << "CountImg" << countImg
       << "RMSE" << rmse
       << "CameraMatrix" << cameraMatrix
       << "DistCoeffs"   << distCoeffs
       << "R"   << R
       << "T"   << T;
    fs.release();
}

void FileSystem::saveInImg(QPixmap qpixmap, QString path)
{
    QFile file(path);
    file.open(QIODevice::WriteOnly);
    qpixmap.save(&file, "png");
    file.close();
}

void FileSystem::saveInImgDrawing(QPixmap qpixmap, QString name)
{
    QFile file(filePath_ + "Drawnable/" + name);
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

bool FileSystem::isSelectedDir()
{
    if(filePath_.isEmpty())
        return false;
    else
        return true;
}

bool FileSystem::isValidOpenDir()
{
    bool isAccum, isDraw, isRes;

    isAccum = QDir(filePath_ + "Accumulated").exists();
    isDraw = QDir(filePath_ + "Drawnable").exists();
    isRes = QDir(filePath_ + "Result").exists();
    if(isAccum && isDraw && isRes)
        return true;
    return false;
}

void FileSystem::setPath(QString path)
{
    filePath_ = path;
}

void FileSystem::getTableItems()
{
    QDir dir(filePath_ + "Accumulated" + QDir::separator());
    QFileInfoList fileList;
    fileList = dir.entryInfoList();
    fileList.count();
    QString status="";

    for(int i= 2; i < fileList.count(); i++){
        QTableWidgetItem *item = new QTableWidgetItem();
        QTableWidgetItem *item1 = new QTableWidgetItem(fileList[i].absoluteFilePath());
        item->setCheckState(Qt::Unchecked);
        emit outTableItems(item,item1);
    }
}

void FileSystem::getOneTableItemsinTableCompare()
{
    std::string date;
    QString pattern;
    int countImg;
    double Rmse;
    QString fileResultPath = filePath_ + "Result" + "/" + "Result.YAML";
    cv::FileStorage fs(fileResultPath.toStdString(),cv::FileStorage::READ);
    fs["Date"] >> date;
    fs["CountImg"] >> countImg;
//  fs["Pattern"] >> pattern;
    fs["RMSE"] >> Rmse;
    fs.release();
    QTableWidgetItem *itemFile = new QTableWidgetItem(filePath_);
    QTableWidgetItem *itemDate = new QTableWidgetItem(QString::fromStdString(date));
    QTableWidgetItem *itemCount = new QTableWidgetItem(QString::number(countImg));
    QTableWidgetItem *itemPattern = new QTableWidgetItem("Chessboard");
    QTableWidgetItem *itemRmse = new QTableWidgetItem(QString::number(Rmse));

    emit outTableItemsCompare(itemFile,itemDate,itemCount,itemPattern,itemRmse);

}

void FileSystem::getTableItemsinTableCompare()
{
    QDir dir(filePath_);
    QFileInfoList fileList;
    fileList = dir.entryInfoList(QDir::Dirs);
    QString localPathDir;
    for(int i = 2;i<fileList.count();i++)
    {
        localPathDir = fileList[i].absoluteFilePath() + "/";
        filePath_ = localPathDir;
        if(isValidOpenDir())
            getOneTableItemsinTableCompare();
    }
}
