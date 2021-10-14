#ifndef CALIBRATIONPROCESSOR_H
#define CALIBRATIONPROCESSOR_H

#include <QString>
#include <QPixmap>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/persistence.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include "filesystem.h"

class CalibrationProcessor
{
public:
    CalibrationProcessor();

    void calibrationChessboardMethod(cv::Mat inputFrame);


};

#endif // CALIBRATIONPROCESSOR_H
