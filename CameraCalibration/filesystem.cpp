#include "filesystem.h"
FileSystem::FileSystem()
{

}

QString FileSystem::getFilePath()
{
    return filePath_;
}

void FileSystem::createWorkDir(int countCam)
{
    QDir dir;
    if(countCam == 1)
    {
        dir.mkpath(filePath_+"Camera1");
        dir.mkpath(filePath_+"Camera1/" + "Accumulated");
        dir.mkpath(filePath_+"Camera1/" +"Drawnable");
        dir.mkpath(filePath_+"Camera1/" +"Undistorted");
        dir.mkpath(filePath_+"Camera1/" +"Result");

    }else{
        dir.mkpath(filePath_+"Camera1");
        dir.mkpath(filePath_+"Camera1/" + "Accumulated");
        dir.mkpath(filePath_+"Camera1/" +"Drawnable");
        dir.mkpath(filePath_+"Camera1/" +"Undistorted");
        dir.mkpath(filePath_+"Camera1/" +"Result");

        dir.mkpath(filePath_+"Camera2");
        dir.mkpath(filePath_+"Camera2/" + "Accumulated");
        dir.mkpath(filePath_+"Camera2/" + "Drawnable");
        dir.mkpath(filePath_+"Camera2/" + "Undistorted");
        dir.mkpath(filePath_+"Camera2/" + "Result");
        dir.mkpath(filePath_+"Stereo");
        dir.mkpath(filePath_+"Stereo/" + "Undistorted");
        dir.mkpath(filePath_+"Stereo/" + "Result");
    }
        dir.mkpath(filePath_+"Setting");
}

void FileSystem::copyDirImgInWorkDir(QString path)
{
    QDir dir(path);

    foreach (QString f, dir.entryList(QDir::Files)) {
    QFile::copy(path + QDir::separator()+ f,
                filePath_ + "Accumulated" + QDir::separator() + f);
    }
}

QString FileSystem::openCalibSetting(int numCam)
{
    QString output;
    std::string pattern;
    cv::Mat cameraMatrix,distCoeffs,R,T;
    std::string date;
    int countImg;
    double Rmse;

    QString fileResultPath = QString(filePath_ + "Camera%1/" + "Result" + QDir::separator() + "Result.YAML").arg(numCam);
    QFileInfo check_file(fileResultPath);
    if(check_file.exists())
    {
        cv::FileStorage fs(fileResultPath.toStdString(), cv::FileStorage::READ);
        fs["Date"] >> date;
        fs["CountImg"] >> countImg;
        fs["RMSE"] >> Rmse;
        fs["CameraMatrix"] >> cameraMatrix;
        fs["DistCoeffs"] >> distCoeffs;

        output.push_back("Date: " + QString::fromStdString(date) + "\n");
        output.push_back("CountImg: " + QString::number(countImg) + "\n");
        output.push_back("RMSE:" + QString::number(Rmse) + "\n");
        output.push_back("CameraMatrix: \n");
        output.push_back("[");
        for(int i=0; i<3; i++){
            for(int j=0; j<3; j++){
                output.push_back(QString::number(cameraMatrix.at<double>(i,j)) + ", \t");
            }
            output.push_back("\n");
        }
        output.push_back("]");
        output.push_back("\n DistCoeffs: \n");
        output.push_back("[");
        for(int i=0; i<5; i++){
            output.push_back(QString::number(distCoeffs.at<double>(i)) + ", ");
        }
        output.push_back("]");
        fs.release();
    }
    return output;
}

void FileSystem::openFileInView(int numImg)
{
    std::vector<InformationImageSaved> infoCamera1 = getInfoCamera1();
    std::vector<InformationImageSaved> infoCamera2 = getInfoCamera2();
    if(infoCamera1.size() > 0)
    {
        QString pathFirst = QString::fromStdString(infoCamera1[numImg].cameraPath);
        QFile filesCameraOne(pathFirst);
        if(filesCameraOne.open(QIODevice::ReadOnly))
        {
            QImage CameraOne(pathFirst);
            emit outImgDisplayFirst(QPixmap::fromImage(CameraOne));
        }
    }

    if(infoCamera2.size() > 0)
    {
        QString pathSecond = QString::fromStdString(infoCamera2[numImg].cameraPath);
        QFile filesCameraSecond(pathSecond);
        if(filesCameraSecond.open(QIODevice::ReadOnly))
        {
            QImage CameraSecond(pathSecond);
            emit outImgDisplaySecond(QPixmap::fromImage(CameraSecond));
        }
    }

}

void FileSystem::openDrawImgInView(int numImg)
{
    std::vector<InformationImageSaved> infoCamera1 = getInfoCamera1();
    std::vector<InformationImageSaved> infoCamera2 = getInfoCamera2();

    if(infoCamera1.size() > 0)
    {
        QString pathFirst = QString::fromStdString(infoCamera1[numImg].drawPath);
        QFile filesCameraOne(pathFirst);
        if(filesCameraOne.open(QIODevice::ReadOnly))
        {
            QImage CameraOne(pathFirst);
            emit outImgDisplayFirst(QPixmap::fromImage(CameraOne));
        }
    }

    if(infoCamera2.size() > 0)
    {
        QString pathSecond = QString::fromStdString(infoCamera2[numImg].drawPath);
        QFile filesCameraSecond(pathSecond);
        if(filesCameraSecond.open(QIODevice::ReadOnly))
        {
            QImage CameraSecond(pathSecond);
            emit outImgDisplaySecond(QPixmap::fromImage(CameraSecond));
        }
    }
}

void FileSystem::openUndistImgInView(int numImg)
{    
    std::vector<InformationImageSaved> infoCamera1 = getInfoCamera1();
    std::vector<InformationImageSaved> infoCamera2 = getInfoCamera2();

    if(infoCamera1.size() > 0)
    {
        QString pathFirst = QString::fromStdString(infoCamera1[numImg].undistortedPath);
        QFile filesCameraOne(pathFirst);
        if(filesCameraOne.open(QIODevice::ReadOnly))
        {
            QImage CameraOne(pathFirst);
            emit outImgDisplayFirst(QPixmap::fromImage(CameraOne));
        }
    }

    if(infoCamera2.size() > 0)
    {
        QString pathSecond = QString::fromStdString(infoCamera2[numImg].undistortedPath);
        QFile filesCameraSecond(pathSecond);
        if(filesCameraSecond.open(QIODevice::ReadOnly))
        {
            QImage CameraSecond(pathSecond);
            emit outImgDisplaySecond(QPixmap::fromImage(CameraSecond));
        }
    }
}

void FileSystem::readYamldistCoefFirst(cv::Mat* distCoeffs)
{
    QString name = filePath_ + "Camera1/Result/Result.YAML";
    cv::FileStorage fs(name.toStdString(),cv::FileStorage::READ);
    fs["DistCoeffs"] >> *distCoeffs;
    fs.release();
}

void FileSystem::readYamldistCoefSecond(cv::Mat* distCoeffs)
{
    QString name = filePath_ + "Camera2/Result/Result.YAML";
    cv::FileStorage fs(name.toStdString(),cv::FileStorage::READ);
    fs["DistCoeffs"] >> *distCoeffs;
    fs.release();
}

void FileSystem::readYamlMatrixFirst(cv::Mat* cameraMatrix)
{
    QString name = filePath_ + "Camera1/Result/Result.YAML";
    cv::FileStorage fs(name.toStdString(),cv::FileStorage::READ);
    fs["CameraMatrix"] >> *cameraMatrix;
    fs.release();
}

void FileSystem::readYamlMatrixSecond(cv::Mat* cameraMatrix)
{
    QString name = filePath_ + "Camera2/Result/Result.YAML";
    cv::FileStorage fs(name.toStdString(),cv::FileStorage::READ);
    fs["CameraMatrix"] >> *cameraMatrix;
    fs.release();
}


void FileSystem::saveFileInYaml(std::vector<std::vector<cv::Point3f>> objpoints, std::vector<std::vector<cv::Point2f>> imgpoints,
                                cv::Mat cameraMatrix,cv::Mat distCoeffs, std::vector<cv::Mat> R, std::vector<cv::Mat> T,
                                int countImg, QString date, double rmse, QString pattern, std::string sizePatern, int numCam,
                                cv::Size size)
{
    QString name;
    if(numCam == 1)
        name = filePath_ + "Camera1/" + "Result/" + "Result" + ".YAML";
    else if(numCam == 2) name = filePath_ + "Camera2/" + "Result/" + "Result" + ".YAML";

    std::string fileResultPath = name.toStdString();
    cv::FileStorage fs(fileResultPath,cv::FileStorage::WRITE);
    fs << "Date" << date.toStdString()
       << "CountImg" << countImg
       << "RMSE" << rmse
       << "CameraMatrix" << cameraMatrix
       << "DistCoeffs"   << distCoeffs
       << "R"   << R
       << "T"   << T
       << "Size" << size
       << "objectPoints" << objpoints
       << "imgesPoints" << imgpoints;
    fs.release();
}

void FileSystem::saveFileInYamlStereo(cv::Mat cameraMatrix0,cv::Mat cameraMatrix1,cv::Mat distCoeffs0, cv::Mat distCoeffs1,
                                      cv::Mat R, cv::Mat T, cv::Mat E, cv::Mat F,
                                      cv::Mat R1, cv::Mat R2, cv::Mat P1,cv::Mat P2, cv::Mat Q,
                                      cv::Size imageSize, cv::Rect validRoi0,cv::Rect validRoi1,
                                      cv::Mat rmap00,cv::Mat rmap01,cv::Mat rmap10,cv::Mat rmap11, double rms,double ReprojErr,
                                      QString date, QString pattern,std::string sizePatern, int countPair)
{
    QString name;

    name = filePath_ + "Stereo/" + "Result/" + "Result" + ".YAML";

    std::string fileResultPath = name.toStdString();
    cv::FileStorage fs(fileResultPath,cv::FileStorage::WRITE);
    fs << "Date" << date.toStdString()
       << "Pattern"<< pattern.toStdString()
       << "SizePattern"<< sizePatern
       << "CountPair" << countPair
       << "RMSE"<< rms
       << "AverageReprojectionErr" << ReprojErr
       << "cameraMatrixFirst" << cameraMatrix0
       << "cameraMatrixSecond" << cameraMatrix1
       << "DistCoeffsFirst" << distCoeffs0
       << "DistCoeffsSecond" << distCoeffs1
       << "R"   << R
       << "T"   << T
       << "E"   << E
       << "F"   << F
       << "R1"   << R1
       << "R2"   << R2
       << "P1"   << P1
       << "P2"   << P2
       << "Q"   << Q
       << "ImgSize" << imageSize
       << "validRoiFirst" << validRoi0
       << "validRoiSecond" << validRoi1
       << "rmap00" << rmap00
       << "rmap01" << rmap01
       << "rmap10" << rmap10
       << "rmap11" << rmap11;
    fs.release();
}

void FileSystem::saveInImg(QPixmap qpixmap, QString name)
{
    qDebug()<<"QFile file(name);";
    QFile file(name);
    if(file.open(QIODevice::WriteOnly))
        qpixmap.save(name, "png");
    file.close();
    qDebug()<<"ImageSaved";
}

void FileSystem::saveInImgDrawing(QPixmap qpixmap, QString fileName,int numCam)
{
    QString path;
    if(numCam == 1)
        path = filePath_ + "Camera1/" + "Drawnable/" + fileName;
    else if(numCam == 2)
        path = filePath_ + "Camera2/" + "Drawnable/" + fileName;
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

bool FileSystem::isSelectedDir()
{
    if(filePath_.isEmpty())
        return false;
    else
        return true;
}

bool FileSystem::isValidOpenDir()
{
    if(!QDir(filePath_ + "Camera1").exists())
        return false;
    if(!QDir(filePath_ + "Camera1/Accumulated").exists())
        return false;
    if(!QDir(filePath_ + "Camera1/Drawnable").exists())
        return false;
    if(!QDir(filePath_ + "Camera1/Result").exists())
        return false;
    if(!QDir(filePath_ + "Camera1/Undistorted").exists())
        return false;

    if(!QDir(filePath_ + "Setting").exists())
        return false;

    return true;
}

void FileSystem::writeSettingCalibInYaml(int numCamFirst, QString nameCumFirst, int numCamSecond,QString nameCumSecond,
                                         QString pattern, int row, int col,
                                         double checkerSize, double markerSize, QString dictionaryName,
                                         bool isWebCamera, bool isBaslerCamera)
{

    checkerSize = checkerSize * 0.001;
    markerSize = markerSize * 0.001;
    QString name;
    name = filePath_ + "Setting/" + "Log" + ".YAML";
    std::string fileResultPath = name.toStdString();
    std::vector<InformationImageSaved> imageInfo1;
    std::vector<InformationImageSaved> imageInfo2;
    cv::FileStorage fs(fileResultPath,cv::FileStorage::WRITE);
    fs <<"FirstCameraName"<<nameCumFirst.toStdString()
       <<"FirstCameraIndex"<<numCamFirst
       <<"SecondCameraName"<<nameCumSecond.toStdString()
       <<"SecondCameraIndex"<<numCamSecond
       <<"Pattern" <<pattern.toStdString()
       <<"Row"<<row
       <<"Col"<<col
       <<"CheckerSize"<<checkerSize
       <<"MarkerSize"<<markerSize
       <<"DictionaryName"<<dictionaryName.toStdString()
       <<"isWebCamera"<< isWebCamera
       <<"isBaslerCamera"<<isBaslerCamera
       <<"isCalibration" << false
       <<"isStereoCalibration" << false
       <<"SubPixIter" << 0
       <<"intFlagsFirst" << 0
       <<"FlagsNameFirst" << "none"
       <<"intFlagsSecond" << 0
       <<"FlagsNameSecond" << "none"
       <<"ImageInfoCamera1" << imageInfo1
       <<"ImageInfoCamera2" << imageInfo2;
    fs.release();
}

int FileSystem::getIndexCameraFirst()
{
    int index;
    QString fileResultPath = filePath_+"Setting/" +  + "Log" + ".YAML";
    cv::FileStorage fs(fileResultPath.toStdString(), cv::FileStorage::READ);
    fs["FirstCameraIndex"] >> index;
    fs.release();
    return index;
}

int FileSystem::getIndexCameraSecond()
{
    int index;
    QString fileResultPath = filePath_+"Setting/" +  + "Log" + ".YAML";
    cv::FileStorage fs(fileResultPath.toStdString(), cv::FileStorage::READ);
    fs["SecondCameraIndex"] >> index;
    fs.release();
    return index;
}

int FileSystem::isCalibration()
{
    int index;
    QString fileResultPath = filePath_+"Setting/" + "Log" + ".YAML";
    cv::FileStorage fs(fileResultPath.toStdString(), cv::FileStorage::READ);
    fs["isCalibration"] >> index;
    fs.release();
    return index;
}

int FileSystem::isStereoCalibration()
{
    int index;
    QString fileResultPath = filePath_+"Setting/" + "Log" + ".YAML";
    cv::FileStorage fs(fileResultPath.toStdString(), cv::FileStorage::READ);
    fs["isStereoCalibration"] >> index;
    fs.release();
    return index;
}

cv::Mat FileSystem::getRmap00()
{
    cv::Mat mat;
    QString fileResultPath = filePath_+"Stereo/Result/Result.YAML";
    cv::FileStorage fs(fileResultPath.toStdString(), cv::FileStorage::READ);
    fs["rmap00"] >> mat;
    fs.release();
    return mat;
}

cv::Mat FileSystem::getRmap01()
{
    cv::Mat mat;
    QString fileResultPath = filePath_+"Stereo/Result/Result.YAML";
    cv::FileStorage fs(fileResultPath.toStdString(), cv::FileStorage::READ);
    fs["rmap01"] >> mat;
    fs.release();
    return mat;
}

cv::Mat FileSystem::getRmap10()
{
    cv::Mat mat;
    QString fileResultPath = filePath_+"Stereo/Result/Result.YAML";
    cv::FileStorage fs(fileResultPath.toStdString(), cv::FileStorage::READ);
    fs["rmap10"] >> mat;
    fs.release();
    return mat;
}

cv::Mat FileSystem::getRmap11()
{
    cv::Mat mat;
    QString fileResultPath = filePath_+"Stereo/Result/Result.YAML";
    cv::FileStorage fs(fileResultPath.toStdString(), cv::FileStorage::READ);
    fs["rmap11"] >> mat;
    fs.release();
    return mat;
}

QVector<QString> FileSystem::getVectorPath(int numCamera)
{
    QVector<QString> vectorPathImg;
    if(numCamera == 1)
    {
        QDir folder(filePath_ + "Camera1/" + "Accumulated/");
        folder.setFilter(QDir::Files);
        QFileInfoList  folderitems(folder.entryInfoList());
        foreach (QFileInfo i_file, folderitems)
        {
            QString i_filename(i_file.absoluteFilePath());
            vectorPathImg.push_back(i_filename);
        }
    }
    if(numCamera == 2)
    {
        QDir folder(filePath_ + "Camera2/" + "Accumulated/");
        folder.setFilter(QDir::Files);
        QFileInfoList  folderitems(folder.entryInfoList());
        foreach (QFileInfo i_file, folderitems)
        {
            QString i_filename(i_file.absoluteFilePath());
            vectorPathImg.push_back(i_filename);
        }
    }
    return vectorPathImg;
}

bool FileSystem::isFirstCameraResul()
{
    QString name = filePath_ + "Camera1/Result/Result.YAML";
    cv::FileStorage fs(name.toStdString(), cv::FileStorage::READ);
    return fs.isOpened();
}

bool FileSystem::isSecondCameraResul()
{
    QString name = filePath_ + "Camera2/Result/Result.YAML";
    cv::FileStorage fs(name.toStdString(), cv::FileStorage::READ);
    return fs.isOpened();
}

bool FileSystem::isStereoCameraResul()
{
    QString name = filePath_ + "Stereo/Result/Result.YAML";
    cv::FileStorage fs(name.toStdString(), cv::FileStorage::READ);
    return fs.isOpened();
}

static void write(cv::FileStorage& fs, const std::string&, const FileSystem::InformationImageSaved& x)
{
    x.write(fs);
}
static void read(const cv::FileNode& node, FileSystem::InformationImageSaved& x, const FileSystem::InformationImageSaved& default_value = FileSystem::InformationImageSaved()){
    if(node.empty())
        x = default_value;
    else
        x.read(node);
}

void FileSystem::writeSettingCalibInYaml(int subIter, int flagsIntFirst, QString flagsNameFirst,
                                         int flagsIntSecond, QString flagsNameSecond, bool isCalibration,bool isStereo)
{
    QString name;
    std::vector<InformationImageSaved> imageInfo1;
    std::vector<InformationImageSaved> imageInfo2;
    std::string FirstCameraName;
    int FirstCameraIndex;
    std::string SecondCameraName;
    int SecondCameraIndex;
    std::string pattern;
    int row, col;
    double checkerSize, markerSize;
    std::string dictionaryName;
    name = filePath_ + "Setting/" + "Log" + ".YAML";
    std::string fileResultPath = name.toStdString();
    cv::FileStorage fs(fileResultPath,cv::FileStorage::READ);
    bool isWebCamera, isBaslerCamera;
    fs["FirstCameraName"] >>FirstCameraName;
    fs["FirstCameraIndex"] >> FirstCameraIndex;
    fs["SecondCameraName"] >> SecondCameraName;
    fs["SecondCameraIndex"] >>SecondCameraIndex;
    fs["Pattern"] >> pattern;
    fs["Row"] >> row;
    fs["Col"] >> col;
    fs["CheckerSize"] >> checkerSize;
    fs["MarkerSize"] >> markerSize;
    fs["isWebCamera"] >> isWebCamera;
    fs["isBaslerCamera"]>> isBaslerCamera;
    fs["DictionaryName"] >> dictionaryName;
    fs["ImageInfoCamera1"] >> imageInfo1;
    fs["ImageInfoCamera2"] >> imageInfo2;
    fs.release();

    cv::FileStorage fss(fileResultPath,cv::FileStorage::WRITE);
    fss
       <<"FirstCameraName" << FirstCameraName
       <<"FirstCameraIndex" << FirstCameraIndex
       <<"SecondCameraName" << SecondCameraName
       <<"SecondCameraIndex" << SecondCameraIndex
       <<"Pattern" << pattern
       <<"Row" << row
       <<"Col" << col
       <<"CheckerSize" << checkerSize
       <<"MarkerSize" << markerSize
       <<"DictionaryName" << dictionaryName
       <<"isWebCamera" << isWebCamera
       <<"isBaslerCamera" << isBaslerCamera
       <<"isCalibration" << isCalibration
       <<"isStereoCalibration" << isStereo
       <<"SubPixIter" << subIter
       <<"intFlagsFirst" << flagsIntFirst
       <<"FlagsNameFirst" << flagsNameFirst.toStdString()
       <<"intFlagsSecond" << flagsIntSecond
       <<"FlagsNameSecond" << flagsNameSecond.toStdString()
       <<"ImageInfoCamera1"<<imageInfo1
       <<"ImageInfoCamera2"<<imageInfo2;
    fss.release();
}

QString FileSystem::getPattern()
{
    std::string pattern;
    QString fileResultPath = filePath_ + "Setting" + "/" + "Log.YAML";
    cv::FileStorage fs(fileResultPath.toStdString(), cv::FileStorage::READ);
    fs["Pattern"] >> pattern;
    fs.release();
    return QString::fromStdString(pattern);
}

double FileSystem::getMarkerSize()
{
    double markerSize;
    QString fileResultPath = filePath_ + "Setting" + "/" + "Log.YAML";
    QString nameFile = filePath_;
    nameFile.chop(1);
    QStringList list = nameFile.split("/");
    nameFile = list.back();
    cv::FileStorage fs(fileResultPath.toStdString(), cv::FileStorage::READ);
    fs["MarkerSize"] >> markerSize;
    fs.release();
    return markerSize;
}

double FileSystem::getCheckerSize()
{
    double checkerSize;
    QString fileResultPath = filePath_ + "Setting" + "/" + "Log.YAML";
    QString nameFile = filePath_;
    nameFile.chop(1);
    QStringList list = nameFile.split("/");
    nameFile = list.back();
    cv::FileStorage fs(fileResultPath.toStdString(), cv::FileStorage::READ);
    fs["CheckerSize"] >> checkerSize;
    fs.release();
    return checkerSize;
}

int FileSystem::getCol()
{
    int col;
    QString fileResultPath = filePath_ + "Setting" + "/" + "Log.YAML";
    QString nameFile = filePath_;
    nameFile.chop(1);
    QStringList list = nameFile.split("/");
    nameFile = list.back();
    cv::FileStorage fs(fileResultPath.toStdString(), cv::FileStorage::READ);
    fs["Col"] >> col;
    fs.release();
    return col;
}

std::vector<std::vector<cv::Point3f>> FileSystem::getObjPoints()
{
    std::vector<std::vector<cv::Point3f>> vector;
    std::string pattern;
    QString fileResultPath = filePath_ + "Camera1/" + "Result/" + "Result.YAML";
    cv::FileStorage fs(fileResultPath.toStdString(), cv::FileStorage::READ);
    fs["objectPoints"] >> vector;
    fs.release();
    return vector;
}

std::vector<std::vector<cv::Point2f>> FileSystem::getImgPoints(int numCam)
{
    std::vector<std::vector<cv::Point2f>> vector;
    QString fileResultPath;
    if(numCam == 1)
        fileResultPath = filePath_ + "Camera1/" + "Result/" + "Result.YAML";
    else if(numCam == 2)
        fileResultPath = filePath_ + "Camera2/" + "Result/" + "Result.YAML";
    cv::FileStorage fs(fileResultPath.toStdString(), cv::FileStorage::READ);
    fs["imgesPoints"] >> vector;
    fs.release();
    return vector;
}

cv::Mat FileSystem::getCameraMatrix(int numCam)
{
    cv::Mat matrix;
    QString fileResultPath;
    if(numCam == 1)
        fileResultPath = filePath_ + "Camera1/" + "Result/" + "Result.YAML";
    else if(numCam == 2)
        fileResultPath = filePath_ + "Camera2/" + "Result/" + "Result.YAML";
    cv::FileStorage fs(fileResultPath.toStdString(), cv::FileStorage::READ);
    fs["CameraMatrix"] >> matrix;
    fs.release();
    return matrix;
}

cv::Mat FileSystem::getDistMatrix(int numCam)
{
    cv::Mat matrix;
    QString fileResultPath;
    if(numCam == 1)
        fileResultPath = filePath_ + "Camera1/" + "Result/" + "Result.YAML";
    else if(numCam == 2)
        fileResultPath = filePath_ + "Camera2/" + "Result/" + "Result.YAML";
    cv::FileStorage fs(fileResultPath.toStdString(), cv::FileStorage::READ);
    fs["DistCoeffs"] >> matrix;
    fs.release();
    return matrix;
}

cv::Size FileSystem::getSizeImg()
{
    cv::Size size;
    QString fileResultPath;
    fileResultPath = filePath_ + "Camera1/" + "Result/" + "Result.YAML";
    cv::FileStorage fs(fileResultPath.toStdString(), cv::FileStorage::READ);
    fs["Size"] >> size;
    fs.release();
    return size;
}

int FileSystem::getRow()
{
    int row;
    QString fileResultPath = filePath_ + "Setting" + "/" + "Log.YAML";
    QString nameFile = filePath_;
    nameFile.chop(1);
    QStringList list = nameFile.split("/");
    nameFile = list.back();
    cv::FileStorage fs(fileResultPath.toStdString(), cv::FileStorage::READ);
    fs["Row"] >> row;
    fs.release();
    return row;
}

QString FileSystem::getDictionaryName()
{
    std::string dictionaryName;
    QString fileResultPath = filePath_ + "Setting" + "/" + "Log.YAML";
    QString nameFile = filePath_;
    nameFile.chop(1);
    QStringList list = nameFile.split("/");
    nameFile = list.back();
    cv::FileStorage fs(fileResultPath.toStdString(), cv::FileStorage::READ);
    fs["DictionaryName"] >> dictionaryName;
    fs.release();
    return QString::fromStdString(dictionaryName);
}

int FileSystem::getSubPixIter()
{
    int subPixIter;
    QString fileResultPath = filePath_ + "Setting" + "/" + "Log.YAML";
    QString nameFile = filePath_;
    nameFile.chop(1);
    QStringList list = nameFile.split("/");
    nameFile = list.back();
    cv::FileStorage fs(fileResultPath.toStdString(), cv::FileStorage::READ);
    fs["SubPixIter"] >> subPixIter;
    fs.release();
    return subPixIter;
}

int FileSystem::getCalibFlags()
{
    int flags;
    QString fileResultPath = filePath_ + "Setting" + "/" + "Log.YAML";
    QString nameFile = filePath_;
    nameFile.chop(1);
    QStringList list = nameFile.split("/");
    nameFile = list.back();
    cv::FileStorage fs(fileResultPath.toStdString(), cv::FileStorage::READ);
    fs["intFlags"] >> flags;
    fs.release();
    return flags;
}

void FileSystem::setPath(QString path)
{
    filePath_ = path;
}

void FileSystem::getTableItems()
{
    std::vector<InformationImageSaved> infoCamera1 = getInfoCamera1();
    std::vector<InformationImageSaved> infoCamera2 = getInfoCamera2();

    QTableWidgetItem *item1;
    QTableWidgetItem *item2;
    for(int i = 0; i < infoCamera1.size(); i++){
        if(infoCamera2.size()>i)
        {
            item1 = new QTableWidgetItem("Camera1/Camera2");
            item2 = new QTableWidgetItem(QString::fromStdString(infoCamera1[i].state + "/" + infoCamera2[i].state));
        }
        else
        {
            item1 = new QTableWidgetItem("Camera1");
            item2 = new QTableWidgetItem(QString::fromStdString(infoCamera1[i].state));
        }

        emit outTableItems(item1, item2);
    }
}

QString FileSystem::openProjectSetting()
{
    QString output;
    QString name;
    std::vector<InformationImageSaved> imageInfo1;
    std::vector<InformationImageSaved> imageInfo2;
    std::string FirstCameraName;
    int FirstCameraIndex;
    std::string SecondCameraName;
    int SecondCameraIndex;
    std::string pattern;
    int row, col;
    double checkerSize, markerSize;
    bool isWebCamera, isBaslerCamera;
    std::string dictionaryName;
    name = filePath_ + "Setting/" + "Log" + ".YAML";
    std::string fileResultPath = name.toStdString();
    cv::FileStorage fs(fileResultPath,cv::FileStorage::READ);
    fs["FirstCameraName"] >> FirstCameraName;
    fs["FirstCameraIndex"] >> FirstCameraIndex;
    fs["SecondCameraName"] >> SecondCameraName;
    fs["SecondCameraIndex"] >>SecondCameraIndex;
    fs["Pattern"] >> pattern;
    fs["Row"] >> row;
    fs["Col"] >> col;
    fs["CheckerSize"] >> checkerSize;
    fs["MarkerSize"] >> markerSize;
    fs["isWebCamera"] >> isWebCamera;
    fs["isBaslerCamera"]>> isBaslerCamera;
    fs["DictionaryName"] >> dictionaryName;
    fs["ImageInfoCamera1"] >> imageInfo1;
    fs["ImageInfoCamera2"] >> imageInfo2;
    fs.release();

    output.append("FirstCameraName: " + QString::fromStdString(FirstCameraName) + '\n');
    output.append("FirstCameraIndex: " + QString::number(FirstCameraIndex) + '\n');
    output.append("SecondCameraName: " + QString::fromStdString(SecondCameraName) + '\n');
    output.append("SecondCameraIndex: " + QString::number(SecondCameraIndex) + '\n');
    output.append("Pattern: " + QString::fromStdString(pattern) + '\n');
    output.append("Row: " + QString::number(row) + '\n');
    output.append("Col: " + QString::number(col) + '\n');
    output.append("isWebCamera: " + QString::number(isWebCamera) + '\n');
    output.append("isBaslerCamera: " + QString::number(isBaslerCamera) + '\n');
    output.append("CheckerSize: " + QString::number(checkerSize) + '\n');
    output.append("MarkerSize: " + QString::number(markerSize) + '\n');
    output.append("ImageInfoCamera1: \n");
    for(int i = 0; i < imageInfo1.size() ;i++)
    {
         output.append(" - ");
         output.append("cameraPath: " + QString::fromStdString(imageInfo1[i].cameraPath) + '\n');
         output.append(" - ");
         output.append("drawPath: " + QString::fromStdString(imageInfo1[i].drawPath) + '\n');
         output.append(" - ");
         output.append("undistortedPath: " + QString::fromStdString(imageInfo1[i].undistortedPath) + '\n');
         output.append(" - ");
         output.append("state: " + QString::fromStdString(imageInfo1[i].state) + '\n');
         output.append(" - ");
         output.append("err: " + QString::number(imageInfo1[i].err) + '\n');
    }
    output += "ImageInfoCamera2: \n";
    for(int i = 0; i < imageInfo2.size() ;i++)
    {
         output.append(" - ");
         output.append("cameraPath: " + QString::fromStdString(imageInfo2[i].cameraPath) + '\n');
         output.append(" - ");
         output.append("drawPath: " + QString::fromStdString(imageInfo2[i].drawPath) + '\n');
         output.append(" - ");
         output.append("undistortedPath: " + QString::fromStdString(imageInfo2[i].undistortedPath) + '\n');
         output.append(" - ");
         output.append("state: " + QString::fromStdString(imageInfo2[i].state) + '\n');
         output.append(" - ");
         output.append("err: " + QString::number(imageInfo2[i].err) + '\n');

    }
    return output;
}

void FileSystem::getOneTableItemsinTableCompare()
{
    std::string date;
    std::string pattern;
    std::string sizePattern;
    int countImg;
    double Rmse;
    std::string dictionaryName;
    std::string flags;
    QString fileResultPath = filePath_ + "Result" + "/" + "Result.YAML";
    QString nameFile = filePath_;
    nameFile.chop(1);
    QStringList list = nameFile.split("/");
    nameFile = list.back();
    cv::FileStorage fs(fileResultPath.toStdString(), cv::FileStorage::READ);
    fs["Date"] >> date;
    fs["CountImg"] >> countImg;
    fs["Pattern"] >> pattern;
    fs["SizePattern"] >> sizePattern;
    fs["RMSE"] >> Rmse;
    //fs["FlagsName"] >> flags;
    fs.release();

    std::string flagss;
    fileResultPath = filePath_ + "Setting" + "/" + "Log.YAML";
    nameFile = filePath_;
    nameFile.chop(1);
    list = nameFile.split("/");
    nameFile = list.back();
    cv::FileStorage fss(fileResultPath.toStdString(), cv::FileStorage::READ);
    fss["FlagsName"] >> flagss;
    fss.release();

    QTableWidgetItem *itemFile = new QTableWidgetItem(nameFile);
    QTableWidgetItem *itemDate = new QTableWidgetItem(QString::fromStdString(date));
    QTableWidgetItem *itemCount = new QTableWidgetItem(QString::number(countImg));
    QTableWidgetItem *itemPattern = new QTableWidgetItem(QString::fromStdString(pattern));
    QTableWidgetItem *itemSizePattern = new QTableWidgetItem(QString::fromStdString(sizePattern));
    QTableWidgetItem *itemRmse = new QTableWidgetItem(QString::number(Rmse));
    QTableWidgetItem *itemFlags = new QTableWidgetItem(QString::fromStdString(flagss));

    emit outTableItemsCompare(itemFile, itemDate, itemCount, itemPattern,itemSizePattern, itemRmse, itemFlags);
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

std::vector<FileSystem::InformationImageSaved> FileSystem::getInfoCamera1()
{
    std::vector<InformationImageSaved> infoCamera;
    QString fileResultPath = filePath_ + "Setting" + "/" + "Log.YAML";
    cv::FileStorage fs(fileResultPath.toStdString(), cv::FileStorage::READ);
    fs["ImageInfoCamera1"] >> infoCamera;
    fs.release();
    return infoCamera;
}

std::vector<FileSystem::InformationImageSaved> FileSystem::getInfoCamera2()
{
    std::vector<InformationImageSaved> infoCamera;
    QString fileResultPath = filePath_ + "Setting" + "/" + "Log.YAML";
    cv::FileStorage fs(fileResultPath.toStdString(), cv::FileStorage::READ);
    fs["ImageInfoCamera2"] >> infoCamera;
    fs.release();
    return infoCamera;
}

void FileSystem::saveInfoCamera1(std::vector<InformationImageSaved> imageInfo1)
{
    int subIter;
    int flagsIntFirst ;
    std::string flagsNameFirst;
    int flagsIntSecond;
    std::string flagsNameSecond;
    bool isCalibration;
    bool isStereo;
    QString name;
    std::vector<InformationImageSaved> imageInfo2;
    std::string FirstCameraName;
    int FirstCameraIndex;
    std::string SecondCameraName;
    int SecondCameraIndex;
    std::string pattern;
    int row, col;
    double checkerSize, markerSize;
    bool isWebCamera, isBaslerCamera;
    std::string dictionaryName;
    name = filePath_ + "Setting/" + "Log" + ".YAML";
    std::string fileResultPath = name.toStdString();
    cv::FileStorage fs(fileResultPath,cv::FileStorage::READ);
    fs["FirstCameraName"] >>FirstCameraName;
    fs["FirstCameraIndex"] >> FirstCameraIndex;
    fs["SecondCameraName"] >> SecondCameraName;
    fs["SecondCameraIndex"] >>SecondCameraIndex;
    fs["Pattern"] >> pattern;
    fs["Row"] >> row;
    fs["Col"] >> col;
    fs["CheckerSize"] >> checkerSize;
    fs["MarkerSize"] >> markerSize;
    fs["DictionaryName"] >> dictionaryName;
    fs["isWebCamera"] >> isWebCamera;
    fs["isBaslerCamera"]>> isBaslerCamera;
    fs["isCalibration"] >> isCalibration;
    fs["isStereoCalibration"] >> isStereo;
    fs["SubPixIter"] >> subIter;
    fs["intFlagsFirst"] >> flagsIntFirst;
    fs["FlagsNameFirst"] >> flagsNameFirst;
    fs["intFlagsSecond"] >> flagsIntSecond;
    fs["FlagsNameSecond"] >> flagsNameSecond;
    fs["ImageInfoCamera2"]>>imageInfo2;
    fs.release();

    cv::FileStorage fss(fileResultPath,cv::FileStorage::WRITE);
    fss
       <<"FirstCameraName" << FirstCameraName
       <<"FirstCameraIndex" << FirstCameraIndex
       <<"SecondCameraName" << SecondCameraName
       <<"SecondCameraIndex" << SecondCameraIndex
       <<"Pattern" << pattern
       <<"Row" << row
       <<"Col" << col
       <<"CheckerSize" << checkerSize
       <<"MarkerSize" << markerSize
       <<"DictionaryName" << dictionaryName
       <<"isWebCamera" << isWebCamera
       <<"isBaslerCamera" << isBaslerCamera
       <<"isCalibration" << isCalibration
       <<"isStereoCalibration" << isStereo
       <<"SubPixIter" << subIter
       <<"intFlagsFirst" << flagsIntFirst
       <<"FlagsNameFirst" << flagsNameFirst
       <<"intFlagsSecond" << flagsIntSecond
       <<"FlagsNameSecond" << flagsNameSecond

       <<"ImageInfoCamera1"<<imageInfo1
       <<"ImageInfoCamera2"<<imageInfo2;
    fss.release();
}
void FileSystem::saveInfoCamera2(std::vector<InformationImageSaved> imageInfo2)
{
    int subIter;
    int flagsIntFirst ;
    std::string flagsNameFirst;
    int flagsIntSecond;
    std::string flagsNameSecond;
    bool isCalibration;
    bool isStereo;
    QString name;
    std::vector<InformationImageSaved> imageInfo1;
    std::string FirstCameraName;
    int FirstCameraIndex;
    std::string SecondCameraName;
    int SecondCameraIndex;
    std::string pattern;
    int row, col;
    double checkerSize, markerSize;
    bool isWebCamera, isBaslerCamera;
    std::string dictionaryName;
    name = filePath_ + "Setting/" + "Log" + ".YAML";
    std::string fileResultPath = name.toStdString();
    cv::FileStorage fs(fileResultPath,cv::FileStorage::READ);
    fs["FirstCameraName"] >>FirstCameraName;
    fs["FirstCameraIndex"] >> FirstCameraIndex;
    fs["SecondCameraName"] >> SecondCameraName;
    fs["SecondCameraIndex"] >>SecondCameraIndex;
    fs["Pattern"] >> pattern;
    fs["Row"] >> row;
    fs["Col"] >> col;
    fs["CheckerSize"] >> checkerSize;
    fs["MarkerSize"] >> markerSize;
    fs["DictionaryName"] >> dictionaryName;
    fs["ImageInfoCamera1"]>>imageInfo1;
    fs["isWebCamera"] >> isWebCamera;
    fs["isBaslerCamera"]>> isBaslerCamera;
    fs["isCalibration"] >> isCalibration;
    fs["isStereoCalibration"] >> isStereo;
    fs["SubPixIter"] >> subIter;
    fs["intFlagsFirst"] >> flagsIntFirst;
    fs["FlagsNameFirst"] >> flagsNameFirst;
    fs["intFlagsSecond"] >> flagsIntSecond;
    fs["FlagsNameSecond"] >> flagsNameSecond;
    fs.release();

    cv::FileStorage fss(fileResultPath,cv::FileStorage::WRITE);
    fss
       <<"FirstCameraName" << FirstCameraName
       <<"FirstCameraIndex" << FirstCameraIndex
       <<"SecondCameraName" << SecondCameraName
       <<"SecondCameraIndex" << SecondCameraIndex
       <<"Pattern" << pattern
       <<"Row" << row
       <<"Col" << col
       <<"CheckerSize" << checkerSize
       <<"MarkerSize" << markerSize
       <<"DictionaryName" << dictionaryName
       <<"isWebCamera" << isWebCamera
       <<"isBaslerCamera" << isBaslerCamera
       <<"isCalibration" << isCalibration
       <<"isStereoCalibration" << isStereo
       <<"SubPixIter" << subIter
       <<"intFlagsFirst" << flagsIntFirst
       <<"FlagsNameFirst" << flagsNameFirst
       <<"intFlagsSecond" << flagsIntSecond
       <<"FlagsNameSecond" << flagsNameSecond

       <<"ImageInfoCamera1"<<imageInfo1
       <<"ImageInfoCamera2"<<imageInfo2;
    fss.release();
}
