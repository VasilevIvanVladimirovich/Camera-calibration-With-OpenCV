#include "imageprocessor.h"

ImageProcessor::ImageProcessor(int indexCam,int numCam)
{
    numCam_= numCam;
    web_cam_.open(indexCam);
    web_cam_.set(cv::CAP_PROP_AUTOFOCUS, 0);
    web_cam_.set(cv::CAP_PROP_AUTO_EXPOSURE, 0.75); //where X is a camera-dependent value such as 0.25 or 0.75.
}

ImageProcessor::ImageProcessor(FileSystem *fs, QString current,QMutex *data_lock)
{
    filesystem = fs;
    lock = data_lock;

    if(current == "FindFirstCameraStream") state_video_stream = FIND_FIRST_STREAM;
    if(current == "FindSecondCameraStream") state_video_stream = FIND_SECOND_STREAM;
    if(current == "FirstCameraStream") state_video_stream = FIRST_STREAM;
    if(current == "SecondCameraStream") state_video_stream = SECOND_STREAM;
    if(current == "FirstAndSecondCameraStream") state_video_stream = FIRST_SECOND_STREAM;
    if(current == "FirstAndSecondCameraStreamWhithLine") state_video_stream = FIRST_SECOND_STREAM_WHITHLINE;
    if(current == "FirstCameraCalibratedStream") state_video_stream = FIRST_CALIBRATED_STREAM;
    if(current == "SecondCameraCalibratedStream") state_video_stream = SECOND_CALIBRATED_STREAM;
    if(current == "FirstAndSecondCameraCalibratedStream") state_video_stream = FIRST_SECOND_CALIBRATED_STREAM;
    if(current == "StereoCalibratedStream") state_video_stream = STEREO_STREAM;
    if(current == "DepthMapStream") state_video_stream = STEREO_DEPTH_STREAM;
}

ImageProcessor::ImageProcessor(cv::Mat img)
{
        setOutFrame(img);
}

void ImageProcessor::run()
{
    initCamera();
    std::vector<FileSystem::InformationImageSaved> imageInfo;
    if(state_video_stream == FIND_FIRST_STREAM || state_video_stream == FIND_SECOND_STREAM)
    {
    if(state_video_stream == FIND_FIRST_STREAM )
        imageInfo = filesystem->getInfoCamera1();
    else if(state_video_stream == FIND_SECOND_STREAM)
        imageInfo = filesystem->getInfoCamera2();
    path_ = filesystem->getFilePath();
    pattern_ = filesystem->getPattern();
    CHECKERBOARD_[0] = filesystem->getRow();
    CHECKERBOARD_[1] = filesystem->getCol();
    checkerSize_ = filesystem->getCheckerSize();
    markerSize_ = filesystem->getMarkerSize();
    setDictionaryName(filesystem->getDictionaryName());
    int count = 0;
    int countImg = 1;
    isEnd_ = false;
    QString namepath;
    cv::Mat drawFrame;
    while(web_cam_.isOpened() && !isEnd_){
        web_cam_ >> outFrame_;
        outFrame_.copyTo(drawFrame);
        count++;
        if(countImg - 1 == countFrame_)
            break;
        //действие по нажатию кнопки
        if(isSnapShoot_)
        {
            if(isPressSnap_) //если кнопка нажата
            {
                //saved frame
                if(isPattern_) //есть ли шаблон на экране (если есть, показываем на экране) и сохраняем его в таблицу
                {
                    if (calibProcessor_.isFramePattern(&drawFrame, pattern_, CHECKERBOARD_[0], CHECKERBOARD_[1], checkerSize_, markerSize_, dictionary_))
                    {
                        QPixmap imgInDisplay = toMatQpixmap(drawFrame);
                        emit outDisplay(imgInDisplay);

                        QPixmap imgInSave = toMatQpixmap(outFrame_);
                        namepath = QString(path_ + "Camera%1/" + "Accumulated/" + QString::number(imageInfo.size() + 1) + ".png").arg(numCam_);

                        filesystem->saveInImg(imgInSave,namepath);
                        FileSystem::InformationImageSaved tmpInfo;
                        tmpInfo.cameraPath = namepath.toStdString();
                        imageInfo.push_back(tmpInfo);
                        QTableWidgetItem *item1;
                        QTableWidgetItem *item2;
                        if(numCam_==1)
                        {
                            item1 = new QTableWidgetItem("Camera");
                            item2 = new QTableWidgetItem("save");
                            emit setItem(item1, item2);
                        }
                        countImg++;
                        isPressSnap_ = false;
                   }else{
                        QPixmap img = toMatQpixmap(outFrame_);
                        emit outDisplay(img);
                        QApplication::beep();
                        namepath = QString(path_ + "Camera%1/" + "Accumulated/" + QString::number(imageInfo.size() + 1) + ".png").arg(numCam_);

                        filesystem->saveInImg(img,namepath);
                        FileSystem::InformationImageSaved tmpInfo;
                        tmpInfo.cameraPath = namepath.toStdString();
                        imageInfo.push_back(tmpInfo);
                        QTableWidgetItem *item1;
                        QTableWidgetItem *item2;
                        if(numCam_==1)
                        {
                            item1 = new QTableWidgetItem("Camera");
                            item2 = new QTableWidgetItem("save");
                            emit setItem(item1, item2);
                        }
                        countImg++;
                        isPressSnap_ = false;
                  }
                }
               }else{ // если кнопка не была нажата
                    if(isPattern_) //если шаблон был найден, показываем его на экране
                    {
                        if(calibProcessor_.isFramePattern(&drawFrame, pattern_, CHECKERBOARD_[0],CHECKERBOARD_[1], checkerSize_, markerSize_, dictionary_)){
                            QPixmap imgInDisplay = toMatQpixmap(drawFrame);
                            emit outDisplay(imgInDisplay);
                        }else{
                            QPixmap img = toMatQpixmap(outFrame_);
                            emit outDisplay(img);
                        }
                   }else{ //если шаблона нету, показываем просто на экране
                        QPixmap img = toMatQpixmap(outFrame_);
                        emit outDisplay(img);
                   }
               }
           }else{
           //действие по таймеру
           if(count % frameRate_ == 0){
               //saved frame
               if(isPattern_)  //есть ли шаблон на экране (если есть, показываем на экране) и сохраняем его в таблицу
               {
                    if (calibProcessor_.isFramePattern(&drawFrame, pattern_, CHECKERBOARD_[0], CHECKERBOARD_[1], checkerSize_, markerSize_, dictionary_))
                    {
                        QPixmap imgInDisplay = toMatQpixmap(drawFrame);
                        emit outDisplay(imgInDisplay);

                        QPixmap imgInSave = toMatQpixmap(outFrame_);
                        namepath = QString(path_ + "Camera%1/" + "Accumulated/" + QString::number(imageInfo.size() + 1) + ".png").arg(numCam_);

                        filesystem->saveInImg(imgInSave,namepath);
                        FileSystem::InformationImageSaved tmpInfo;
                        tmpInfo.cameraPath = namepath.toStdString();
                        imageInfo.push_back(tmpInfo);
                        QTableWidgetItem *item1;
                        QTableWidgetItem *item2;
                        if(numCam_==1)
                        {
                            item1 = new QTableWidgetItem("Camera");
                            item2 = new QTableWidgetItem("save");
                            emit setItem(item1, item2);
                        }
                        countImg++;
                   }
               }else{
                    QPixmap img= toMatQpixmap(outFrame_);
                    emit outDisplay(img);
                    QApplication::beep();
                    namepath = QString(path_ + "Camera%1/" + "Accumulated/" + QString::number(imageInfo.size() + 1) + ".png").arg(numCam_);

                    filesystem->saveInImg(img,namepath);
                    FileSystem::InformationImageSaved tmpInfo;
                    tmpInfo.cameraPath = namepath.toStdString();
                    imageInfo.push_back(tmpInfo);
                    QTableWidgetItem *item1;
                    QTableWidgetItem *item2;
                    if(numCam_==1)
                    {
                        item1 = new QTableWidgetItem("Camera");
                        item2 = new QTableWidgetItem("save");
                        emit setItem(item1, item2);
                    }
                    countImg++;
              }
           }else{
               if(isPattern_)
               {
                   if(calibProcessor_.isFramePattern(&drawFrame,pattern_,CHECKERBOARD_[0],CHECKERBOARD_[1], checkerSize_, markerSize_, dictionary_)){
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
        if(state_video_stream == FIND_FIRST_STREAM)
        {
            lock->lock();
            filesystem->saveInfoCamera1(imageInfo);
            lock->unlock();
            emit andStream();
        }else if(state_video_stream == FIND_SECOND_STREAM)
        {
            lock->lock();
            filesystem->saveInfoCamera2(imageInfo);
            lock->unlock();
            emit andStream();
        }
    }
    if(state_video_stream == FIRST_STREAM)
    {
        while(web_camFirst_.isOpened() && !isEnd_)
        {
            web_camFirst_ >> outFrame_;
            QPixmap img = toMatQpixmap(outFrame_);
            emit outDisplayFirst(img);
        }
    }
    if(state_video_stream == SECOND_STREAM)
    {
        while(web_camSecond_.isOpened() && !isEnd_)
        {
            web_camSecond_ >> outFrame_;
            QPixmap img = toMatQpixmap(outFrame_);
            emit outDisplaySecond(img);
        }
    }
    if(state_video_stream == FIRST_SECOND_STREAM)
    {
        cv::Mat outFrameFirst,outFrameSecond;
        while(web_camFirst_.isOpened() && web_camSecond_.isOpened() && !isEnd_)
        {
            web_camFirst_ >> outFrameFirst;
            web_camSecond_ >> outFrameSecond;

            QPixmap imgFirst = toMatQpixmap(outFrameFirst);
            emit outDisplayFirst(imgFirst);

            QPixmap imgSecond = toMatQpixmap(outFrameSecond);
            emit outDisplaySecond(imgSecond);
        }
    }
    if(state_video_stream == FIRST_SECOND_STREAM_WHITHLINE)
    {
        cv::Mat outFrameFirst,outFrameSecond;
        while(web_camFirst_.isOpened() && web_camSecond_.isOpened() && !isEnd_)
        {
            web_camFirst_ >> outFrameFirst;
            web_camSecond_ >> outFrameSecond;

            for(int j = 0; j < outFrameFirst.rows; j += 16)
                line(outFrameFirst, cv::Point(0, j), cv::Point(outFrameFirst.cols, j), cv::Scalar(0, 255, 0), 1, 8);

            for(int j = 0; j < outFrameFirst.cols; j += 64)
                line(outFrameFirst, cv::Point(j, 0), cv::Point(j, outFrameFirst.rows), cv::Scalar(0, 255, 0), 1, 8);

            QPixmap imgFirst = toMatQpixmap(outFrameFirst);
            emit outDisplayFirst(imgFirst);

            for(int j = 0; j < outFrameSecond.rows; j += 16 )
                line(outFrameSecond, cv::Point(0, j), cv::Point(outFrameSecond.cols, j), cv::Scalar(0, 255, 0), 1, 8);

            for(int j = 0; j < outFrameSecond.cols; j += 64 )
                line(outFrameSecond, cv::Point(j, 0), cv::Point(j,outFrameSecond.cols), cv::Scalar(0, 255, 0), 1, 8);

            QPixmap imgSecond = toMatQpixmap(outFrameSecond);
            emit outDisplaySecond(imgSecond);
        }
    }

    if(state_video_stream == FIRST_CALIBRATED_STREAM)
    {
        cv::Mat cameraMatrixFirst, distCoeffsFirst;
        filesystem->readYamlMatrixFirst(&cameraMatrixFirst);
        filesystem->readYamldistCoefSecond(&distCoeffsFirst);
        while(web_camFirst_.isOpened() && !isEnd_)
        {
            web_camFirst_ >> inputFrame_;
            cv::undistort(inputFrame_, outFrame_, cameraMatrixFirst, distCoeffsFirst);
            QPixmap img = toMatQpixmap(outFrame_);
            emit outDisplayFirst(img);
        }
    }
    if(state_video_stream == SECOND_CALIBRATED_STREAM)
    {
        cv::Mat cameraMatrixSecond, distCoeffsSecond;
        filesystem->readYamlMatrixFirst(&cameraMatrixSecond);
        filesystem->readYamldistCoefSecond(&distCoeffsSecond);
        while(web_camSecond_.isOpened() && !isEnd_)
        {
            web_camSecond_ >> inputFrame_;
            cv::undistort(inputFrame_, outFrame_, cameraMatrixSecond, distCoeffsSecond);
            QPixmap img = toMatQpixmap(outFrame_);
            emit outDisplaySecond(img);
        }
    }
    if(state_video_stream == FIRST_SECOND_CALIBRATED_STREAM)
    {
        cv::Mat cameraMatrixFirst, distCoeffsFirst;
        cv::Mat cameraMatrixSecond, distCoeffsSecond;
        cv::Mat inputFirst,outputFirst;
        cv::Mat inputSecond,outputSecond;
        filesystem->readYamlMatrixFirst(&cameraMatrixFirst);
        filesystem->readYamldistCoefSecond(&distCoeffsFirst);
        filesystem->readYamlMatrixFirst(&cameraMatrixSecond);
        filesystem->readYamldistCoefSecond(&distCoeffsSecond);
        while(web_camFirst_.isOpened() && web_camSecond_.isOpened() && !isEnd_)
        {
            web_camFirst_ >> inputFirst;
            web_camSecond_ >> inputSecond;
            cv::undistort(inputFirst, outputFirst, cameraMatrixFirst, distCoeffsFirst);
            cv::undistort(inputSecond, outputSecond, cameraMatrixSecond, distCoeffsSecond);
            QPixmap imgFirst = toMatQpixmap(outputFirst);
            QPixmap imgSecond = toMatQpixmap(outputSecond);
            emit outDisplayFirst(imgFirst);
            emit outDisplaySecond(imgSecond);
        }
    }
    if(state_video_stream == STEREO_STREAM)
    {
        cv::Mat cameraMatrixFirst, distCoeffsFirst;
        cv::Mat cameraMatrixSecond, distCoeffsSecond;
        cv::Mat inputFirst,outputFirst;
        cv::Mat inputSecond,outputSecond;
        cv::Mat rmap[2][2];
        filesystem->readYamlMatrixFirst(&cameraMatrixFirst);
        filesystem->readYamldistCoefSecond(&distCoeffsFirst);
        filesystem->readYamlMatrixFirst(&cameraMatrixSecond);
        filesystem->readYamldistCoefSecond(&distCoeffsSecond);
        rmap[0][0] = filesystem->getRmap00();
        rmap[0][1] = filesystem->getRmap01();
        rmap[1][0] = filesystem->getRmap10();
        rmap[1][1] = filesystem->getRmap11();
        while(web_camFirst_.isOpened() && web_camSecond_.isOpened() && !isEnd_)
        {
            web_camFirst_ >> inputFirst;
            web_camSecond_ >> inputSecond;

            cv::remap(inputFirst, outputFirst, rmap[0][0], rmap[0][1], cv::INTER_LINEAR, cv::BORDER_DEFAULT, cv::Scalar());
            QPixmap imgFirst = toMatQpixmap(outputFirst);
            emit outDisplayFirst(imgFirst);

            cv::remap(inputSecond, outputSecond, rmap[1][0], rmap[1][1], cv::INTER_LINEAR, cv::BORDER_DEFAULT, cv::Scalar());
            QPixmap imgSecond = toMatQpixmap(outputSecond);
            emit outDisplaySecond(imgSecond);

        }
    }
    if(state_video_stream == STEREO_DEPTH_STREAM)
    {
        cv::Mat cameraMatrixFirst, distCoeffsFirst;
        cv::Mat cameraMatrixSecond, distCoeffsSecond;
        cv::Mat inputFirst,outputFirst;
        cv::Mat inputSecond,outputSecond;
        cv::Mat grayScaleFirst,grayScaleSecond;
        cv::Mat disparity;
        cv::Mat rmap[2][2];
        filesystem->readYamlMatrixFirst(&cameraMatrixFirst);
        filesystem->readYamldistCoefSecond(&distCoeffsFirst);
        filesystem->readYamlMatrixFirst(&cameraMatrixSecond);
        filesystem->readYamldistCoefSecond(&distCoeffsSecond);
        rmap[0][0] = filesystem->getRmap00();
        rmap[0][1] = filesystem->getRmap01();
        rmap[1][0] = filesystem->getRmap10();
        rmap[1][1] = filesystem->getRmap11();

        cv::Ptr<cv::StereoBM> sbm =  cv::StereoBM::create(16,21);

        sbm->setDisp12MaxDiff(1);
        sbm->setSpeckleRange(8);
        sbm->setSpeckleWindowSize(9);
        sbm->setUniquenessRatio(0);
        sbm->setTextureThreshold(507);
        sbm->setMinDisparity(-39);
        sbm->setPreFilterCap(61);
        sbm->setPreFilterSize(5);

        while(web_camFirst_.isOpened() && web_camSecond_.isOpened() && !isEnd_)
        {
            web_camFirst_ >> inputFirst;
            web_camSecond_ >> inputSecond;
            cv::remap(inputFirst, outputFirst, rmap[0][0], rmap[0][1], cv::INTER_LINEAR, cv::BORDER_DEFAULT, cv::Scalar());
            cv::remap(inputSecond, outputSecond, rmap[1][0], rmap[1][1], cv::INTER_LINEAR, cv::BORDER_DEFAULT, cv::Scalar());

            cvtColor(outputFirst, grayScaleFirst, cv::COLOR_RGB2GRAY);
            cvtColor(outputSecond, grayScaleSecond, cv::COLOR_RGB2GRAY);

            sbm->compute(grayScaleFirst, grayScaleSecond, disparity);

            QPixmap img = toMatQpixmap(disparity);
            emit outDisplayFirst(img);
        }
    }
}


void ImageProcessor::initCamera()
{
    if(state_video_stream == FIND_FIRST_STREAM)
    {
        numCam_= 1;
        web_cam_.open(filesystem->getIndexCameraFirst(), cv::CAP_DSHOW);
        web_cam_.set(cv::CAP_PROP_AUTOFOCUS, 0);
        web_cam_.set(cv::CAP_PROP_AUTO_EXPOSURE, 0.75);
        web_cam_.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
        web_cam_.set(cv::CAP_PROP_FRAME_HEIGHT, 720);
    }
    if(state_video_stream == FIND_SECOND_STREAM)
    {
        numCam_= 2;
        web_cam_.open(filesystem->getIndexCameraSecond(), cv::CAP_DSHOW);
        web_cam_.set(cv::CAP_PROP_AUTOFOCUS, 0);
        web_cam_.set(cv::CAP_PROP_AUTO_EXPOSURE, 0.75);
        web_cam_.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
        web_cam_.set(cv::CAP_PROP_FRAME_HEIGHT, 720);
    }
    if(state_video_stream == FIRST_STREAM || state_video_stream == FIRST_SECOND_STREAM || state_video_stream == FIRST_SECOND_STREAM_WHITHLINE ||
       state_video_stream == FIRST_CALIBRATED_STREAM || state_video_stream == FIRST_SECOND_CALIBRATED_STREAM ||
       state_video_stream == STEREO_STREAM || state_video_stream == STEREO_DEPTH_STREAM)
    {
        web_camFirst_.open(filesystem->getIndexCameraFirst(), cv::CAP_DSHOW);
        web_camFirst_.set(cv::CAP_PROP_AUTOFOCUS, 0);
        web_camFirst_.set(cv::CAP_PROP_AUTO_EXPOSURE, 0.75);
        web_camFirst_.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
        web_camFirst_.set(cv::CAP_PROP_FRAME_HEIGHT, 720);
    }

    if(state_video_stream == SECOND_STREAM || state_video_stream == FIRST_SECOND_STREAM || state_video_stream == FIRST_SECOND_STREAM_WHITHLINE ||
       state_video_stream == SECOND_CALIBRATED_STREAM || state_video_stream == FIRST_SECOND_CALIBRATED_STREAM ||
       state_video_stream == STEREO_STREAM || state_video_stream == STEREO_DEPTH_STREAM)
    {
        web_camSecond_.open(filesystem->getIndexCameraSecond(), cv::CAP_DSHOW);
        web_camSecond_.set(cv::CAP_PROP_AUTOFOCUS, 0);
        web_camSecond_.set(cv::CAP_PROP_AUTO_EXPOSURE, 0.75);
        web_camSecond_.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
        web_camSecond_.set(cv::CAP_PROP_FRAME_HEIGHT, 720);
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

void ImageProcessor::setIsSnapShoot(bool isSnapShoot)
{
    isSnapShoot_ = isSnapShoot;
}

void ImageProcessor::setCheckerSize(double checkerSize)
{
    checkerSize_ = checkerSize;
}

void ImageProcessor::setMarkerSize(double markerSize)
{
    markerSize_ = markerSize;
}

void ImageProcessor::setDictionaryName(QString dictionaryName)
{
    if(dictionaryName == "DICT_4X4_50") dictionary_ = 0;
    else if(dictionaryName == "DICT_4X4_50") dictionary_ = 1;
    else if(dictionaryName == "DICT_4X4_100") dictionary_ = 2;
    else if(dictionaryName == "DICT_4X4_1000") dictionary_ = 3;
    else if(dictionaryName == "DICT_5X5_50") dictionary_ = 4;
    else if(dictionaryName == "DICT_5X5_100") dictionary_ = 5;
    else if(dictionaryName == "DICT_5X5_250") dictionary_ = 6;
    else if(dictionaryName == "DICT_5X5_1000") dictionary_ = 7;
    else if(dictionaryName == "DICT_6X6_50") dictionary_ = 8;
    else if(dictionaryName == "DICT_6X6_100") dictionary_ = 9;
    else if(dictionaryName == "DICT_6X6_250") dictionary_ = 10;
    else if(dictionaryName == "DICT_6X6_1000") dictionary_ = 11;
    else if(dictionaryName == "DICT_7X7_50") dictionary_ = 12;
    else if(dictionaryName == "DICT_7X7_100") dictionary_ = 13;
    else if(dictionaryName == "DICT_7X7_250") dictionary_ = 14;
    else if(dictionaryName == "DICT_7X7_1000") dictionary_ = 15;
    else if(dictionaryName == "DICT_ARUCO_ORIGINAL") dictionary_ = 16;
    else if(dictionaryName == "DICT_APRILTAG_16h5") dictionary_ = 17;
    else if(dictionaryName == "DICT_APRILTAG_25h9") dictionary_ = 18;
    else if(dictionaryName == "DICT_APRILTAG_36h10") dictionary_ = 19;
    else if(dictionaryName == "NULL") dictionary_ = -1;
}

void ImageProcessor::setIsPressSnap()
{
    lock->lock();
    isPressSnap_ = true;
    lock->unlock();
}

void ImageProcessor::setFileSystem(FileSystem *fs)
{
    filesystem = fs;
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
    emit andStream();
    isEnd_=true;
}


