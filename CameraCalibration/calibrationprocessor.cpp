#include "calibrationprocessor.h"

CalibrationProcessor::CalibrationProcessor(QObject *parent) : QObject(parent)
{

}

void CalibrationProcessor::accumulationVectorsImg()
{
    QString filename;
    FileSystem fileSystem;
    QDir dir(TEMP_PATH);
    dir.removeRecursively();
    dir.mkpath(TEMP_PATH);

    reloadVectors();

    std::vector<cv::Point3f> objp;
    for(int i{0};i<CHECKERBOARD_[1];i++)
    {
        for(int j{0}; j<CHECKERBOARD_[0]; j++)
        objp.push_back(cv::Point3f(j,i,0));
    }
    std::vector<cv::Point2f> corner_pts; // Вектор для хранения пикселей координат углов шахматной доски
    bool success;
    for(int i = 0;i<vectorPathImg_.length();i++)
    {
        inputFrame_= cv::imread(vectorPathImg_[i].toStdString());
        cv::cvtColor(inputFrame_,gray,cv::COLOR_BGR2GRAY);

    //Поиск углов шахматной доски
    //Если на изображении найдено нужное количество углов, то успех = истина
        success = cv::findChessboardCorners(gray,cv::Size(CHECKERBOARD_[0], CHECKERBOARD_[1]),corner_pts);
    if(success)
    {
         cv::TermCriteria criteria(cv::TermCriteria::EPS | cv::TermCriteria::MAX_ITER, subPixelIter_, 0.001);
         cv::cornerSubPix(gray,corner_pts,cv::Size(11,11), cv::Size(-1,-1),criteria);
         cv::drawChessboardCorners(inputFrame_, cv::Size(CHECKERBOARD_[0], CHECKERBOARD_[1]), corner_pts, success);
         emit sendStatusImg("Success", i);

         objpoints_.push_back(objp);
         imgpoints_.push_back(corner_pts);

         QPixmap saveImg = QPixmap::fromImage(
                                   QImage(inputFrame_.data,
                                   inputFrame_.cols,
                                   inputFrame_.rows,
                                   inputFrame_.step,
                                   QImage::Format_RGB888).rgbSwapped());
        filename = TEMP_PATH + QString::number(i) + ".png";
        fileSystem.saveInImg(saveImg,filename);

    }else emit sendStatusImg("No find corners", i);

    cameraCalibrationChessboardMethod();
}
}

void CalibrationProcessor::reloadVectors()
{
    objpoints_.clear();
    imgpoints_.clear();
    imgpoints_.clear();

}

void CalibrationProcessor::setVectorPathImg(QVector<QString> vector)
{
    vectorPathImg_=vector;
}

double CalibrationProcessor::computeReprojectionErrors()
{

    std::vector<cv::Point2f> imagePoints2;
    double totalErr=0,err;
    size_t totalPoints=0;
    for(size_t i = 0; i < objpoints_.size(); ++i )
    {
         projectPoints(objpoints_[i], R_[i], T_[i], cameraMatrix_, distCoeffs_, imagePoints2);
         err = norm(imgpoints_[i], imagePoints2, cv::NORM_L2);
         size_t n = objpoints_[i].size();
         totalErr        += err*err;
         totalPoints     += n;
    }
    return std::sqrt(totalErr/totalPoints);
}


void CalibrationProcessor::cameraCalibrationChessboardMethod()
{
    cv::calibrateCamera(objpoints_, imgpoints_, cv::Size(gray.rows, gray.cols), cameraMatrix_, distCoeffs_, R_, T_);

    FileSystem fs;
    fs.saveFileInYaml(cameraMatrix_,distCoeffs_,R_,T_,"Result");  

}

void CalibrationProcessor::setTargetType(QString qstring)
{
    targetType_ = qstring;
}

void CalibrationProcessor::setTargetSize(int row, int col)
{
    CHECKERBOARD_[0]=row;
    CHECKERBOARD_[1]=col;
}

void CalibrationProcessor::setSubPixIter(int count)
{
    subPixelIter_ = count;
}

