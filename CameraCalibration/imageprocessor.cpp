#include "imageprocessor.h"

ImageProcessor::ImageProcessor(int num_web_cam)
{
    web_cam_.open(num_web_cam);
    web_cam_.set(cv::CAP_PROP_AUTOFOCUS, 0);
}

ImageProcessor::ImageProcessor(cv::Mat img)
{
        setOutFrame(img);
}


void ImageProcessor::run()
{
    end_ = false;
    while(web_cam_.isOpened() && !end_) {
            web_cam_ >> outFrame_;
            emit outDisplay(QPixmap::fromImage(
                            QImage(outFrame_.data,
                                   outFrame_.cols,
                                   outFrame_.rows,
                                   outFrame_.step,
                                   QImage::Format_RGB888).rgbSwapped()));
       }
}

void ImageProcessor::setOutFrame(cv::Mat frame)
{
    outFrame_ = frame;

    emit outDisplay(QPixmap::fromImage(
                    QImage(outFrame_.data,
                           outFrame_.cols,
                           outFrame_.rows,
                           outFrame_.step,
                           QImage::Format_RGB888).rgbSwapped()));
}

cv::Mat ImageProcessor::getOutFrame()
{
    return outFrame_;
}

void ImageProcessor::stopedThread()
{
    end_=true;
}
