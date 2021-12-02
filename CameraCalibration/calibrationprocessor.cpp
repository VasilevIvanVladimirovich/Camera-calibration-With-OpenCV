#include "calibrationprocessor.h"

CalibrationProcessor::CalibrationProcessor()
{

}

void CalibrationProcessor::run()
{
    QString filename;
    reloadVectors();

    std::vector<cv::Point3f> objp;
    for(int i{0};i<CHECKERBOARD_[1];i++)
    {
        for(int j{0}; j<CHECKERBOARD_[0]; j++)
        objp.push_back(cv::Point3f(j,i,0));
    }
    std::vector<cv::Point2f> corner_pts; // Вектор для хранения пикселей координат углов шахматной доски
    bool isSuccess;

    for(int i = 0;i<vectorPathImg_.length();i++)
    {
        inputFrame_ = cv::imread(vectorPathImg_[i].toStdString());
        cv::cvtColor(inputFrame_,gray,cv::COLOR_BGR2GRAY);

        if(targetType_ == "Chessboard")
             isSuccess = cv::findChessboardCorners(gray,cv::Size(CHECKERBOARD_[0], CHECKERBOARD_[1]),corner_pts);
        else if(targetType_ == "Circle")
             isSuccess = cv::findCirclesGrid(gray, cv::Size(CHECKERBOARD_[0], CHECKERBOARD_[1]), corner_pts,
                         cv::CALIB_CB_SYMMETRIC_GRID, cv::SimpleBlobDetector::create());
        else if(targetType_ == "Assymetric Circles")
             isSuccess = cv::findCirclesGrid(gray, cv::Size(CHECKERBOARD_[0], CHECKERBOARD_[1]), corner_pts,
                         cv::CALIB_CB_ASYMMETRIC_GRID, cv::SimpleBlobDetector::create());

        if(isSuccess)
        {
           //cv::TermCriteria criteria(cv::TermCriteria::EPS | cv::TermCriteria::MAX_ITER, subPixelIter_, 0.001);
           //cv::cornerSubPix(gray,corner_pts, cv::Size(11,11), cv::Size(-1,-1),criteria);
           cv::drawChessboardCorners(inputFrame_, cv::Size(CHECKERBOARD_[0], CHECKERBOARD_[1]), corner_pts, isSuccess);
           emit sendStatusImg("Success", i);

           objpoints_.push_back(objp);
           imgpoints_.push_back(corner_pts);

           QPixmap saveImg = QPixmap::fromImage(
                             QImage(inputFrame_.data,
                                     inputFrame_.cols,
                                     inputFrame_.rows,
                                     inputFrame_.step,
                             QImage::Format_RGB888).rgbSwapped());

           filename = QString::number(i) + ".png";
           fs_.saveInImgDrawing(saveImg, filename);
       }else emit sendStatusImg("No find corners", i);
    }
    cameraCalibration();
    emit sendOpenFileInViewYamlCalib(fs_.getFilePath());
}

void CalibrationProcessor::cameraCalibration()
{
    cv::calibrateCamera(objpoints_, imgpoints_, cv::Size(gray.rows, gray.cols), cameraMatrix_, distCoeffs_, R_, T_);

    fs_.createImgUndistorted(cameraMatrix_, distCoeffs_);

    QDateTime date;
    date = QDateTime::currentDateTime();
    int countImg;
    countImg = vectorPathImg_.length();
    double rmse;
    rmse = Rmse();
    fs_.saveFileInYaml(cameraMatrix_, distCoeffs_, R_, T_, countImg, date.toString("yyyy.dd.M--HH:mm:ss"),
                       rmse, targetType_, CHECKERBOARD_[0], CHECKERBOARD_[1]);
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

void CalibrationProcessor::setPath(QString path)
{
    fs_.setPath(path);
}

double CalibrationProcessor::Rmse()
{

    std::vector<cv::Point2f> imagePoints2;
    double totalErr = 0;
    double err;
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


bool CalibrationProcessor::isFramePattern(cv::Mat* frame, QString pattern,int row, int col)
{
    cv::cvtColor(*frame, gray, cv::COLOR_BGR2GRAY);
    std::vector<cv::Point2f> corner_pts;

    if(pattern == "Chessboard")
    {
        if(cv::findChessboardCorners(gray, cv::Size(row, col), corner_pts, cv::CALIB_CB_FAST_CHECK))
        {
            cv::drawChessboardCorners(*frame, cv::Size(row, col), corner_pts, true);
            return true;
        }
    }
    if(pattern == "Circles")
    {
        if(cv::findCirclesGrid(gray, cv::Size(row, col), corner_pts, cv::CALIB_CB_SYMMETRIC_GRID, cv::SimpleBlobDetector::create()))
        {
            cv::drawChessboardCorners(*frame, cv::Size(row, col), cv::Mat(corner_pts), true);
            return true;
        }
    }
    if(pattern == "Assymetric Circles")
    {
        if(cv::findCirclesGrid(gray, cv::Size(row, col), corner_pts, cv::CALIB_CB_ASYMMETRIC_GRID, cv::SimpleBlobDetector::create()))
        {
            cv::drawChessboardCorners(*frame, cv::Size(row, col), cv::Mat(corner_pts), true);
            return true;
        }
    }
    if(pattern == "ChArUco")
    {
//        cv::detectMarkers();
    }
    return false;
}

void CalibrationProcessor::setTargetType(QString qstring)
{
    targetType_ = qstring;
}

void CalibrationProcessor::setTargetSize(int row, int col)
{
    CHECKERBOARD_[0] = row;
    CHECKERBOARD_[1] = col;
}

void CalibrationProcessor::setSubPixIter(int count)
{
    subPixelIter_ = count;
}
