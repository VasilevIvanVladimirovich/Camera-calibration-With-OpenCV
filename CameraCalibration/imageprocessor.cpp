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
    int countImg = 1;
    isEnd_ = false;
    QString namepath;
    if(isTransformImg_){
        filesystem.readYamlMatrix(path_,&cameraMatrix_);
        filesystem.readYamldistCoef(path_,&distCoeffs_);
    }
    cv::Mat drawFrame;
    while(web_cam_.isOpened() && !isEnd_){
        if(isTransformImg_){
            web_cam_ >> inputFrame_;
            cv::undistort(inputFrame_,outFrame_,cameraMatrix_,distCoeffs_);
            QPixmap img = toMatQpixmap(inputFrame_);;
           emit outDisplay(img);
        }else{
           web_cam_ >> outFrame_;
           outFrame_.copyTo(drawFrame);
           count++;
           if(count % frameRate_ == 0){
               if(countImg == countFrame_)
                   stopedThread();
               //saved frame
               if(isPattern_)
               {
                   if (calibProcessor_.isFramePattern(&drawFrame,pattern_,CHECKERBOARD_[0],CHECKERBOARD_[1]))
                   {
                       QPixmap imgInDisplay = toMatQpixmap(drawFrame);
                       emit outDisplay(imgInDisplay);

                       QPixmap imgInSave = toMatQpixmap(outFrame_);
                       namepath = path_.chopped(1) + "Аccumulated/" + QString::number(countImg) + ".png";
                       countImg++;
                       filesystem.saveInImg(imgInSave,namepath);
                       QTableWidgetItem *item = new QTableWidgetItem();
                       item->setCheckState(Qt::Unchecked);
                       QTableWidgetItem *item1 = new QTableWidgetItem(namepath);
                       emit setItem(item,item1);
                   }
               }else{
                   QPixmap img= toMatQpixmap(outFrame_);
                   emit outDisplay(img);
                   namepath = path_.chopped(1) + "Аccumulated/" + QString::number(countImg) + ".png";
                   countImg++;
                   filesystem.saveInImg(img,namepath);
                   QTableWidgetItem *item = new QTableWidgetItem();
                   item->setCheckState(Qt::Unchecked);
                   QTableWidgetItem *item1 = new QTableWidgetItem(namepath);
                   emit setItem(item,item1);
               }
           }else{
               if(isPattern_)
               {
                   if(calibProcessor_.isFramePattern(&drawFrame,pattern_,CHECKERBOARD_[0],CHECKERBOARD_[1])){
                       QPixmap imgInDisplay = toMatQpixmap(drawFrame);
                       emit outDisplay(imgInDisplay);
                   }else{
                       QPixmap img = toMatQpixmap(outFrame_);
                       emit outDisplay(img);
                   }

               }else{
                   QPixmap img= toMatQpixmap(outFrame_);
                   emit outDisplay(img);
               }
           }
        }
    }
}

void ImageProcessor::setTransformImg(bool newTransformImg)
{
    isTransformImg_ = newTransformImg;
}

void ImageProcessor::setIsPattern(bool isPattern)
{
    isPattern_ = isPattern;
}

void ImageProcessor::setPattern(QString pattern)
{
    pattern_ = pattern;
}

void ImageProcessor::setCheckboardstate(int row, int col)
{
    CHECKERBOARD_[0] = row;
    CHECKERBOARD_[1] = col;
}

QPixmap ImageProcessor::toMatQpixmap(cv::Mat mat)
{
    return QPixmap::fromImage(
           QImage(mat.data,
                  mat.cols,
                  mat.rows,
                  mat.step,
           QImage::Format_RGB888).rgbSwapped());
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
    countFrame_ = countFrame;
}

void ImageProcessor::setFrameRate(int frameRate)
{
    frameRate_ = frameRate;
}

void ImageProcessor::stopedThread()
{
    isEnd_=true;
}
