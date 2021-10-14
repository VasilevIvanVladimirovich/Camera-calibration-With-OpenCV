#include "calibrationprocessor.h"

CalibrationProcessor::CalibrationProcessor()
{

}

void CalibrationProcessor::calibrationChessboardMethod(cv::Mat inputFrame)
{
    int CHECKERBOARD[2]{6,9}; //размер шахматной доски

    std::vector<std::vector<cv::Point3f> > objpoints; //Вектор, для хранения векторов 3d точек для каждого изображения шахматной доски
    std::vector<std::vector<cv::Point2f> > imgpoints; //Вектор, для хранения векторов 2d точек для каждого изображения шахматной доски

    //Определение мировых координат для 3d точке
    std::vector<cv::Point3f> objp;
    for(int i{0};i<CHECKERBOARD[1];i++)
    {
        for(int j{0}; j<CHECKERBOARD[0]; j++)
        objp.push_back(cv::Point3f(j,i,0));
    }

    std::vector<cv::Point2f> corner_pts; // Вектор для хранения пикселей координат углов шахматной доски
    bool success;

    //Поиск углов шахматной доски
    //Если на изображении найдено нужное количество углов, то успех = истина
    success = cv::findChessboardCorners(inputFrame,cv::Size(CHECKERBOARD[0], CHECKERBOARD[1]),corner_pts);

    if(success)
    {
        cv::drawChessboardCorners(inputFrame, cv::Size(CHECKERBOARD[0], CHECKERBOARD[1]), corner_pts, success);
        objpoints.push_back(objp);
        imgpoints.push_back(corner_pts);
        cv::Mat cameraMatrix, distCoeffs, R, T;

        cv::calibrateCamera(objpoints, imgpoints, cv::Size(inputFrame.rows, inputFrame.cols), cameraMatrix, distCoeffs, R, T);

        std::cout << "cameraMatrix : " << cameraMatrix << std::endl;
        std::cout << "distCoeffs : " << distCoeffs << std::endl;
        std::cout << "Rotation vector : " << R << std::endl;
        std::cout << "Translation vector : " << T << std::endl;

        FileSystem fileSystem;
        QPixmap saveImg = QPixmap::fromImage(
                                    QImage(inputFrame.data,
                                    inputFrame.cols,
                                    inputFrame.rows,
                                    inputFrame.step,
                                    QImage::Format_RGB888).rgbSwapped());
        fileSystem.saveResult(saveImg, cameraMatrix, distCoeffs, R, T);

    }
}
