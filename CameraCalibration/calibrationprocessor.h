#ifndef CALIBRATIONPROCESSOR_H
#define CALIBRATIONPROCESSOR_H

#include <QString>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/persistence.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

class CalibrationProcessor
{
private:
    cv::Mat inputFrame_;
public:
    CalibrationProcessor(cv::Mat inputFrame);
    ~CalibrationProcessor();
    void calibrationChessboardMethod();


};

#endif // CALIBRATIONPROCESSOR_H
