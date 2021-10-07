#include "videoprocessor.h"

VideoProcessor::VideoProcessor()
{
    web_cam.open(1);
}

void VideoProcessor::startVideo()
{
    stopped = false;
    end = false;
    while(web_cam.isOpened() && !end) {
       if(!stopped)
        {
            web_cam >> outFrame;
            emit outDisplay(QPixmap::fromImage(
                            QImage(outFrame.data,
                                   outFrame.cols,
                                   outFrame.rows,
                                   outFrame.step,
                                   QImage::Format_RGB888).rgbSwapped()));
       }
    }
}

void VideoProcessor::stopVideo()
{
    end = true;
}

void VideoProcessor::setpause(bool pause)
{
    stopped = pause;
}


bool VideoProcessor::isStoped()
{
    return stopped;
}

cv::Mat VideoProcessor::getOutFrame()
{
    return outFrame;
}
void VideoProcessor::setOutFrame(cv::Mat frame)
{
    this->outFrame = frame;
}
