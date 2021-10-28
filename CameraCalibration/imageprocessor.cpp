#include "imageprocessor.h"

ImageProcessor::ImageProcessor(int num_web_cam)
{
    web_cam_.open(num_web_cam);
    web_cam_.set(cv::CAP_PROP_AUTOFOCUS, 0);
    web_cam_.set(cv::CAP_PROP_AUTO_EXPOSURE, 0.75); //where X is a camera-dependent value such as 0.25 or 0.75.
}

ImageProcessor::ImageProcessor(cv::Mat img)
{
        setOutFrame(img);
}


void ImageProcessor::run()
{
    int count = 0;
    int countImg=1;
    end_ = false;
    QString namepath;
    if(transformImg==true){
        filesystem.readYamlMatrix(path_,&cameraMatrix_);
        filesystem.readYamldistCoef(path_,&distCoeffs_);

    }
    while(web_cam_.isOpened() && !end_) {
        if(transformImg==true)
        {
            web_cam_ >> inputFrame_;
            cv::undistort(inputFrame_,outFrame_,cameraMatrix_,distCoeffs_);
            QPixmap img;
            img = QPixmap::fromImage(
                        QImage(outFrame_.data,
                               outFrame_.cols,
                               outFrame_.rows,
                               outFrame_.step,
                               QImage::Format_RGB888).rgbSwapped());

           emit outDisplay(img);
        }else
        {
            web_cam_ >> outFrame_;
            QPixmap img;
            img = QPixmap::fromImage(
                        QImage(outFrame_.data,
                               outFrame_.cols,
                               outFrame_.rows,
                               outFrame_.step,
                               QImage::Format_RGB888).rgbSwapped());

           emit outDisplay(img);
           count++;
           if(count % countFrame_ == 0)
           {
               namepath = path_ + QString::number(countImg) + ".png";
               countImg++;
               filesystem.saveInImg(img,namepath);
               QTableWidgetItem *item = new QTableWidgetItem();
               item->setCheckState(Qt::Unchecked);
               QTableWidgetItem *item1 = new QTableWidgetItem(namepath);
               emit setItem(item,item1);
           }
    }
    }
}

void ImageProcessor::setTransformImg(bool newTransformImg)
{
    transformImg = newTransformImg;
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

void ImageProcessor::setPath(QString qstring)
{
    path_ = qstring;
}

void ImageProcessor::setCountFrame(int countFrame)
{
    countFrame_=countFrame;
}

void ImageProcessor::stopedThread()
{
    end_=true;
}
