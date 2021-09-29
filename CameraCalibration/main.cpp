#include "mainwindow.h"

#include <QApplication>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
int main()
{
    VideoCapture cap(0);
        Mat edges;
        namedWindow("webcam", 1);
        while (true)
        {
            Mat frame;
            cap >> frame;
            imshow("webcam", frame);
            if (waitKey(30) >= 0) break;
        }
        return 0;

}
