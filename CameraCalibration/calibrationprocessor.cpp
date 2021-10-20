 #include "calibrationprocessor.h"

CalibrationProcessor::CalibrationProcessor()
{

}

void CalibrationProcessor::accumulationVectorsImg(cv::Mat inputFrame)
{
    std::vector<cv::Point3f> objp;
    for(int i{0};i<CHECKERBOARD_[1];i++)
    {
        for(int j{0}; j<CHECKERBOARD_[0]; j++)
        objp.push_back(cv::Point3f(j,i,0));
    }

    std::vector<cv::Point2f> corner_pts; // Вектор для хранения пикселей координат углов шахматной доски
    bool success;

    //Поиск углов шахматной доски
    //Если на изображении найдено нужное количество углов, то успех = истина
    success = cv::findChessboardCorners(inputFrame,cv::Size(CHECKERBOARD_[0], CHECKERBOARD_[1]),corner_pts);

    if(success)
    {
        cv::drawChessboardCorners(inputFrame, cv::Size(CHECKERBOARD_[0], CHECKERBOARD_[1]), corner_pts, success);
        objpoints_.push_back(objp);
        imgpoints_.push_back(corner_pts);

        std::cout << "cameraMatrix : " << cameraMatrix_ << std::endl;
        std::cout << "distCoeffs : " << distCoeffs_ << std::endl;
        std::cout << "Rotation vector : " << R_ << std::endl;
        std::cout << "Translation vector : " << T_ << std::endl;

        FileSystem fileSystem;
        QPixmap saveImg = QPixmap::fromImage(
                                    QImage(inputFrame.data,
                                    inputFrame.cols,
                                    inputFrame.rows,
                                    inputFrame.step,
                                    QImage::Format_RGB888).rgbSwapped());
        fileSystem.saveResult(saveImg, cameraMatrix_, distCoeffs_, R_, T_);

    }

}

void CalibrationProcessor::calibrationChessboardMethod(cv::Mat inputFrame)
{
        cv::calibrateCamera(objpoints_, imgpoints_, cv::Size(inputFrame.rows, inputFrame.cols), cameraMatrix_, distCoeffs_, R_, T_);

}
