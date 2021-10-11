#include "imageprocessor.h"

ImageProcessor::ImageProcessor(int num_web_cam)
{
    web_cam.open(num_web_cam);
}

ImageProcessor::ImageProcessor(cv::Mat img)
{
        setOutFrame(img);
}


void ImageProcessor::run()
{
    end = false;
    while(web_cam.isOpened() && !end) {
            web_cam >> outFrame;
            emit outDisplay(QPixmap::fromImage(
                            QImage(outFrame.data,
                                   outFrame.cols,
                                   outFrame.rows,
                                   outFrame.step,
                                   QImage::Format_RGB888).rgbSwapped()));
       }
}

void ImageProcessor::setOutFrame(cv::Mat frame)
{
    outFrame = frame;

    emit outDisplay(QPixmap::fromImage(
                    QImage(outFrame.data,
                           outFrame.cols,
                           outFrame.rows,
                           outFrame.step,
                           QImage::Format_RGB888).rgbSwapped()));
}

cv::Mat ImageProcessor::getOutFrame()
{
    return outFrame;
}

void ImageProcessor::stopedThread()
{
    end=true;
}
