#include "imageprocessor.h"

ImageProcessor::ImageProcessor(int indexCam,int numCam)
{
    numCam_= numCam;
    web_cam_.open(indexCam);
    web_cam_.set(cv::CAP_PROP_AUTOFOCUS, 0);
    web_cam_.set(cv::CAP_PROP_AUTO_EXPOSURE, 0.75); //where X is a camera-dependent value such as 0.25 or 0.75.
}

ImageProcessor::ImageProcessor(FileSystem *fs, StateVideoStream stVideo,DeviceState stDevice,QMutex *data_lock)
{
    filesystem = fs;
    lock = data_lock;
    state_video_stream = stVideo;
    state_device = stDevice;
}

ImageProcessor::ImageProcessor(cv::Mat img)
{
    setOutFrame(img);
}

std::vector<FileSystem::InformationImageSaved> ImageProcessor::getInfoCamera()
{
    return imageInfo;
}


void ImageProcessor::run()
{
    switch (state_device) {
    case WEB_CAMERA:
    {
        initCamera();
        if(state_video_stream == FIND_FIRST_STREAM || state_video_stream == FIND_SECOND_STREAM)
        {
        if(state_video_stream == FIND_FIRST_STREAM)
            imageInfo = filesystem->getInfoCamera1();
        if(state_video_stream == FIND_SECOND_STREAM)
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

        if(web_cam_.isOpened())
            sendTerminal(QString("Camera%1 launched").arg(numCam_));

        while(web_cam_.isOpened() && !isEnd_){
            web_cam_ >> outFrame_;
            outFrame_.copyTo(drawFrame);
            count++;
            if(countImg - 1 == countFrame_)
                break;
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
                        QTableWidgetItem *item0 = new QTableWidgetItem();
                        item0->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
                        item0->setCheckState(Qt::Checked);
                        QTableWidgetItem *item1 = new QTableWidgetItem(QString("Camera%1").arg(numCam_));
                        QTableWidgetItem *item2 = new QTableWidgetItem("save");
                        emit setItem(item0, item1, item2);
                        sendTerminal(QString("Camera%1 saved the image").arg(numCam_));
                        FileSystem::InformationImageSaved tmpInfo;
                        tmpInfo.cameraPath = namepath.toStdString();
                        imageInfo.push_back(tmpInfo);
                        countImg++;
                        isPressSnap_ = false;
                   }else{
                        QPixmap img = toMatQpixmap(outFrame_);
                        emit outDisplay(img);
                        QApplication::beep();
                        namepath = QString(path_ + "Camera%1/" + "Accumulated/" + QString::number(imageInfo.size() + 1) + ".png").arg(numCam_);

                        filesystem->saveInImg(img,namepath);
                        QTableWidgetItem *item0 = new QTableWidgetItem();
                        item0->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
                        item0->setCheckState(Qt::Checked);
                        QTableWidgetItem *item1 = new QTableWidgetItem(QString("Camera%1").arg(numCam_));
                        QTableWidgetItem *item2 = new QTableWidgetItem("save");
                        emit setItem(item0, item1, item2);
                        sendTerminal(QString("Camera%1 saved the image").arg(numCam_));
                        FileSystem::InformationImageSaved tmpInfo;
                        tmpInfo.cameraPath = namepath.toStdString();
                        imageInfo.push_back(tmpInfo);
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
            filesystem->readYamldistCoefFirst(&distCoeffsFirst);
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
            filesystem->readYamlMatrixSecond(&cameraMatrixSecond);
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
            cv::Mat R1,P1,R2,P2;
            filesystem->readYamlMatrixFirst(&cameraMatrixFirst);
            filesystem->readYamldistCoefSecond(&distCoeffsFirst);
            filesystem->readYamlMatrixSecond(&cameraMatrixSecond);
            filesystem->readYamldistCoefSecond(&distCoeffsSecond);
            R1 = filesystem->getR1();
            R2 = filesystem->getR2();
            P1 = filesystem->getP1();
            P2 = filesystem->getP2();

            std::vector<FileSystem::InformationImageSaved> imageInfo = filesystem->getInfoCamera1();
            cv::Mat img1 = cv::imread(imageInfo[0].cameraPath);

            cv::initUndistortRectifyMap(cameraMatrixFirst, distCoeffsFirst, R1, P1, img1.size(), CV_16SC2, rmap[0][0], rmap[0][1]);
            cv::initUndistortRectifyMap(cameraMatrixSecond, distCoeffsSecond, R2, P2, img1.size(), CV_16SC2, rmap[1][0], rmap[1][1]);


            while(web_camFirst_.isOpened() && web_camSecond_.isOpened() && !isEnd_)
            {
                qDebug()<<"go";
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
            cv::Mat R1,P1,R2,P2;
            filesystem->readYamlMatrixFirst(&cameraMatrixFirst);
            filesystem->readYamldistCoefSecond(&distCoeffsFirst);
            filesystem->readYamlMatrixSecond(&cameraMatrixSecond);
            filesystem->readYamldistCoefSecond(&distCoeffsSecond);
            R1 = filesystem->getR1();
            R2 = filesystem->getR2();
            P1 = filesystem->getP1();
            P2 = filesystem->getP2();

            std::vector<FileSystem::InformationImageSaved> imageInfo = filesystem->getInfoCamera1();
            cv::Mat img1 = cv::imread(imageInfo[0].cameraPath);

            cv::initUndistortRectifyMap(cameraMatrixFirst, distCoeffsFirst, R1, P1, img1.size(),CV_16SC2, rmap[0][0], rmap[0][1]);
            cv::initUndistortRectifyMap(cameraMatrixSecond, distCoeffsSecond, R2, P2, img1.size(),CV_16SC2, rmap[1][0], rmap[1][1]);

            cv::Ptr<cv::StereoBM> sbm =  cv::StereoBM::create();
            isEnd_ = false;

//            sbm->setNumDisparities(numDisparities);
//            sbm->setBlockSize(blockSize);
//            sbm->setPreFilterType(preFilterType);
//            sbm->setPreFilterSize(preFilterSize);
//            sbm->setPreFilterCap(preFilterCap);
//            sbm->setTextureThreshold(textureThreshold);
//            sbm->setUniquenessRatio(uniquenessRatio);
//            sbm->setSpeckleRange(speckleRange);
//            sbm->setSpeckleWindowSize(speckleWindowSize);
//            sbm->setDisp12MaxDiff(disp12MaxDiff);
//            sbm->setMinDisparity(minDisparity);

            cv::namedWindow("disparity",cv::WINDOW_NORMAL);

            cv::Mat disp;
            double minVal; double maxVal;
            while(web_camFirst_.isOpened() && web_camSecond_.isOpened() && !isEnd_)
            {
                web_camFirst_ >> inputFirst;
                web_camSecond_ >> inputSecond;

                cv::cvtColor(inputFirst, grayScaleFirst, cv::COLOR_BGR2GRAY);
                cv::cvtColor(inputSecond, grayScaleSecond, cv::COLOR_BGR2GRAY);
                //cv::remap(grayScaleFirst, outputFirst, rmap[0][0], rmap[0][1], cv::INTER_LANCZOS4, cv::BORDER_DEFAULT, cv::Scalar());
                //cv::remap(grayScaleSecond, outputSecond, rmap[1][0], rmap[1][1], cv::INTER_LANCZOS4, cv::BORDER_DEFAULT, cv::Scalar());

                sbm->compute(grayScaleFirst, grayScaleSecond, disp);
                //cv::normalize(disp, disp, 0, 255, cv::NORM_MINMAX, CV_8UC1);

                minMaxLoc(disp,&minVal, &maxVal);
                disp.convertTo(disparity, CV_8UC1, 255/(maxVal - minVal));

                QPixmap img = toMatQpixmapGray(disparity);
                emit outDisplayFirst(img);

            }
        }
        break;
    }
    case BASLER_CAMERA:
    {
        Pylon::PylonInitialize();
        Pylon::CTlFactory &TlFactory = Pylon::CTlFactory::GetInstance();
        Pylon::ITransportLayer * pTl = TlFactory.CreateTl("BaslerUsb");
        Pylon::DeviceInfoList_t lstDevices;
        pTl->EnumerateDevices(lstDevices);

        //mono stream
        if(state_video_stream == FIND_FIRST_STREAM ||
           state_video_stream == FIND_SECOND_STREAM ||
           state_video_stream == FIRST_STREAM ||
           state_video_stream == SECOND_STREAM ||
           state_video_stream == FIRST_CALIBRATED_STREAM ||
           state_video_stream == SECOND_CALIBRATED_STREAM)
        {

            int numCam;
            //std::vector<FileSystem::InformationImageSaved> imageInfo;
            std::string nameBasler;
            if(state_video_stream == FIND_FIRST_STREAM ||
               state_video_stream == FIRST_STREAM ||
               state_video_stream == FIRST_CALIBRATED_STREAM)
            {
                numCam = 1;
                imageInfo = filesystem->getInfoCamera1();
                nameBasler = filesystem->getNameCameraFirst();
            }else if(state_video_stream == FIND_SECOND_STREAM ||
                     state_video_stream == SECOND_STREAM ||
                     state_video_stream ==  SECOND_CALIBRATED_STREAM)
            {
                numCam = 2;
                imageInfo = filesystem->getInfoCamera2();
                nameBasler = filesystem->getNameCameraSecond();
            }
            bool isCamExist = false;
            Pylon::CBaslerUsbInstantCamera Baslercamera;
            for(int j = 0; j<lstDevices.size();++j)
            {
                if(lstDevices[j].GetFriendlyName() == nameBasler.c_str())
                {
                    Baslercamera.Attach(Pylon::CTlFactory::GetInstance().CreateDevice(lstDevices[j]));
                    Baslercamera.Open();
                    Baslercamera.ExposureAuto.SetValue(Basler_UsbCameraParams::ExposureAuto_Off);
                    Baslercamera.ExposureTime.SetValue(filesystem->getExpose());
                    Baslercamera.GainAuto.SetValue(Basler_UsbCameraParams::GainAuto_Off);
                    Baslercamera.Gain.SetValue(filesystem->getGain());
                    isCamExist= true;
                    break;
                }
            }
            if(!isCamExist) return;

            Baslercamera.TriggerSelector.SetValue(Basler_UsbCameraParams::TriggerSelector_FrameStart);
            Baslercamera.TriggerSource.SetValue(Basler_UsbCameraParams::TriggerSource_Line1);
            Baslercamera.TriggerActivation.SetValue(Basler_UsbCameraParams::TriggerActivation_RisingEdge);
            Baslercamera.AcquisitionMode.SetValue(Basler_UsbCameraParams::AcquisitionMode_Continuous);
            Baslercamera.TriggerMode.SetValue(Basler_UsbCameraParams::TriggerMode_Off);
            Baslercamera.TriggerSoftware.Execute();

            Pylon::CGrabResultPtr ptrGrabResult;


            int countImg;
            if(state_video_stream == FIND_FIRST_STREAM ||
               state_video_stream == FIND_SECOND_STREAM)
            {
                pattern_ = filesystem->getPattern();
                CHECKERBOARD_[0] = filesystem->getRow();
                CHECKERBOARD_[1] = filesystem->getCol();
                checkerSize_ = filesystem->getCheckerSize();
                markerSize_ = filesystem->getMarkerSize();
                setDictionaryName(filesystem->getDictionaryName());
                countImg = 1;
                cv::Mat drawFrame;
                isEnd_ = false;
                QString namepath;
                cv::Mat img;
                Baslercamera.StartGrabbing(Pylon::GrabStrategy_LatestImages, Pylon::GrabLoop_ProvidedByUser);
                while(Baslercamera.IsGrabbing() && !isEnd_)
                {
                    Baslercamera.RetrieveResult(5000, ptrGrabResult, Pylon::TimeoutHandling_ThrowException);
                    if (ptrGrabResult->GrabSucceeded())
                    {
                        lock->lock();
                        img = cv::Mat(ptrGrabResult->GetHeight(),ptrGrabResult->GetWidth(),CV_8UC1,ptrGrabResult->GetBuffer());
                        lock->unlock();
                        img.copyTo(drawFrame);
                        cv::cvtColor(drawFrame, drawFrame, cv::COLOR_GRAY2BGR);

                        if(state_video_stream == FIND_FIRST_STREAM ||
                           state_video_stream == FIND_SECOND_STREAM)
                        {
                            if(countImg - 1 == countFrame_)
                                isEnd_ = true;
                            if(isPressSnap_) //если кнопка нажата
                            {
                                if(isDraw)
                                    calibProcessor_.isFramePattern(&drawFrame, pattern_, CHECKERBOARD_[0], CHECKERBOARD_[1], checkerSize_, markerSize_, dictionary_);

                                QPixmap imgInDisplay = toMatQpixmap(drawFrame);

                                emit outDisplayFirst(imgInDisplay);

                                QPixmap imgInSave = toMatQpixmapGray(img);
                                namepath = QString(path_ + "Camera%1/" + "Accumulated/" + QString::number(imageInfo.size() + 1) + ".png").arg(numCam);
                                filesystem->saveInImg(imgInSave,namepath);
                                FileSystem::InformationImageSaved tmpInfo;
                                tmpInfo.cameraPath = namepath.toStdString();
                                imageInfo.push_back(tmpInfo);
                                countImg++;
                                isPressSnap_ = false;
                            }else{
                                if(isDraw)
                                    calibProcessor_.isFramePattern(&drawFrame, pattern_, CHECKERBOARD_[0], CHECKERBOARD_[1], checkerSize_, markerSize_, dictionary_);

                                QPixmap imgInDisplay = toMatQpixmap(drawFrame);

                                emit outDisplayFirst(imgInDisplay);
                            }
                        }
                    }
                }

            }else if(state_video_stream == FIRST_STREAM ||
                     state_video_stream == SECOND_STREAM)
            {
                cv::Mat img;
                Baslercamera.StartGrabbing(Pylon::GrabStrategy_LatestImages, Pylon::GrabLoop_ProvidedByUser);
                while(Baslercamera.IsGrabbing() && !isEnd_)
                {
                    Baslercamera.RetrieveResult(5000, ptrGrabResult, Pylon::TimeoutHandling_ThrowException);
                    if (ptrGrabResult->GrabSucceeded())
                    {
                        lock->lock();
                        img = cv::Mat(ptrGrabResult->GetHeight(),ptrGrabResult->GetWidth(),CV_8UC1,ptrGrabResult->GetBuffer());
                        lock->unlock();

                        QPixmap imgInDisplay = toMatQpixmapGray(img);
                        emit outDisplayFirst(imgInDisplay);
                    }

                }
            }else if(FIRST_CALIBRATED_STREAM )
            {
                cv::Mat cameraMatrix, distCoeffs;
                filesystem->readYamlMatrixFirst(&cameraMatrix);
                filesystem->readYamldistCoefFirst(&distCoeffs);
                cv::Mat img,out;
                Baslercamera.StartGrabbing(Pylon::GrabStrategy_LatestImages, Pylon::GrabLoop_ProvidedByUser);
                while(Baslercamera.IsGrabbing() && !isEnd_)
                {
                    Baslercamera.RetrieveResult(5000, ptrGrabResult, Pylon::TimeoutHandling_ThrowException);
                    if (ptrGrabResult->GrabSucceeded())
                    {
                        lock->lock();
                        img = cv::Mat(ptrGrabResult->GetHeight(),ptrGrabResult->GetWidth(),CV_8UC1,ptrGrabResult->GetBuffer());
                        lock->unlock();

                        cv::undistort(img, out, cameraMatrix, distCoeffs);
                        QPixmap imgInDisplay = toMatQpixmapGray(out);
                        emit outDisplayFirst(imgInDisplay);
                    }

                }
            }else if(SECOND_CALIBRATED_STREAM )
            {
                cv::Mat cameraMatrix, distCoeffs;
                filesystem->readYamlMatrixSecond(&cameraMatrix);
                filesystem->readYamldistCoefSecond(&distCoeffs);
                cv::Mat img,out;
                Baslercamera.StartGrabbing(Pylon::GrabStrategy_LatestImages, Pylon::GrabLoop_ProvidedByUser);
                while(Baslercamera.IsGrabbing() && !isEnd_)
                {
                    Baslercamera.RetrieveResult(5000, ptrGrabResult, Pylon::TimeoutHandling_ThrowException);
                    if (ptrGrabResult->GrabSucceeded())
                    {
                        lock->lock();
                        img = cv::Mat(ptrGrabResult->GetHeight(),ptrGrabResult->GetWidth(),CV_8UC1,ptrGrabResult->GetBuffer());
                        lock->unlock();

                        cv::undistort(img, out, cameraMatrix, distCoeffs);
                        QPixmap imgInDisplay = toMatQpixmapGray(out);
                        emit outDisplayFirst(imgInDisplay);
                    }
                }
            }
        //stereo stream
        }else if(state_video_stream == FIND_STEREO_STREAM ||
                 state_video_stream == FIRST_SECOND_STREAM ||
                 state_video_stream == FIRST_SECOND_STREAM_WHITHLINE ||
                 state_video_stream == FIRST_SECOND_CALIBRATED_STREAM ||
                 state_video_stream == STEREO_STREAM ||
                 state_video_stream == STEREO_DEPTH_STREAM)
        {
            Pylon::CBaslerUsbInstantCameraArray Baslercameras(2);
            std::string nameBaslerFirst = filesystem->getNameCameraFirst();
            std::string nameBaslerSecond = filesystem->getNameCameraSecond();
            for(int j = 0; j<lstDevices.size();++j)
            {
                if(lstDevices[j].GetFriendlyName() == nameBaslerFirst.c_str())
                {
                    Baslercameras[0].Attach( Pylon::CTlFactory::GetInstance().CreateDevice(lstDevices[j]));
                    Baslercameras[0].Open();
                    Baslercameras[0].ExposureAuto.SetValue(Basler_UsbCameraParams::ExposureAuto_Off);
                    Baslercameras[0].ExposureTime.SetValue(filesystem->getExpose());
                    Baslercameras[0].GainAuto.SetValue(Basler_UsbCameraParams::GainAuto_Off);
                    Baslercameras[0].Gain.SetValue(filesystem->getGain());
                }
                if(lstDevices[j].GetFriendlyName() == nameBaslerSecond.c_str())
                {
                    Baslercameras[1].Attach( Pylon::CTlFactory::GetInstance().CreateDevice(lstDevices[j]));
                    Baslercameras[1].Open();
                    Baslercameras[1].ExposureAuto.SetValue(Basler_UsbCameraParams::ExposureAuto_Off);
                    Baslercameras[1].ExposureTime.SetValue(filesystem->getExpose());
                    Baslercameras[1].GainAuto.SetValue(Basler_UsbCameraParams::GainAuto_Off);
                    Baslercameras[1].Gain.SetValue(filesystem->getGain());
                }
            }
            Baslercameras[0].TriggerSelector.SetValue(Basler_UsbCameraParams::TriggerSelector_FrameStart);
            Baslercameras[0].TriggerSource.SetValue(Basler_UsbCameraParams::TriggerSource_Line1);
            Baslercameras[0].TriggerActivation.SetValue(Basler_UsbCameraParams::TriggerActivation_RisingEdge);
            Baslercameras[0].AcquisitionMode.SetValue(Basler_UsbCameraParams::AcquisitionMode_Continuous);
            Baslercameras[0].TriggerMode.SetValue(Basler_UsbCameraParams::TriggerMode_Off);
            Baslercameras[0].TriggerSoftware.Execute();

            Baslercameras[1].TriggerSelector.SetValue(Basler_UsbCameraParams::TriggerSelector_FrameStart);
            Baslercameras[1].TriggerSource.SetValue(Basler_UsbCameraParams::TriggerSource_Line1);
            Baslercameras[1].TriggerActivation.SetValue(Basler_UsbCameraParams::TriggerActivation_RisingEdge);
            Baslercameras[1].AcquisitionMode.SetValue(Basler_UsbCameraParams::AcquisitionMode_Continuous);
            Baslercameras[1].TriggerMode.SetValue(Basler_UsbCameraParams::TriggerMode_Off);
            Baslercameras[1].TriggerSoftware.Execute();

            Pylon::CGrabResultPtr ptrGrabResultL;
            Pylon::CGrabResultPtr ptrGrabResultR;

            std::vector<FileSystem::InformationImageSaved> imageInfo1;
            std::vector<FileSystem::InformationImageSaved> imageInfo2;


            if(state_video_stream == FIND_STEREO_STREAM)
            {
                int countImg;
                pattern_ = filesystem->getPattern();
                CHECKERBOARD_[0] = filesystem->getRow();
                CHECKERBOARD_[1] = filesystem->getCol();
                checkerSize_ = filesystem->getCheckerSize();
                markerSize_ = filesystem->getMarkerSize();
                setDictionaryName(filesystem->getDictionaryName());
                imageInfo1 = filesystem->getInfoCamera1();
                imageInfo2 = filesystem->getInfoCamera2();
                countImg = 1;
                cv::Mat drawFrameL,drawFrameR;
                isEnd_ = false;
                QString namepathL,namepathR;
                cv::Mat imgL,imgR;
                path_ = filesystem->getFilePath();

                Baslercameras.StartGrabbing(Pylon::GrabStrategy_LatestImages, Pylon::GrabLoop_ProvidedByUser);
                while(Baslercameras.IsGrabbing() && !isEnd_)
                {
                    Baslercameras[0].RetrieveResult(5000, ptrGrabResultL, Pylon::TimeoutHandling_ThrowException);
                    Baslercameras[1].RetrieveResult(5000, ptrGrabResultR, Pylon::TimeoutHandling_ThrowException);

                    if (ptrGrabResultL->GrabSucceeded() && ptrGrabResultR->GrabSucceeded())
                    {

                        lock->lock();
                        imgL = cv::Mat(ptrGrabResultL->GetHeight(),ptrGrabResultL->GetWidth(),CV_8UC1,ptrGrabResultL->GetBuffer());
                        imgR = cv::Mat(ptrGrabResultR->GetHeight(),ptrGrabResultR->GetWidth(),CV_8UC1,ptrGrabResultR->GetBuffer());
                        lock->unlock();
                        imgL.copyTo(drawFrameL);
                        imgR.copyTo(drawFrameR);
                        cv::cvtColor(drawFrameL, drawFrameL, cv::COLOR_GRAY2BGR);
                        cv::cvtColor(drawFrameR, drawFrameR, cv::COLOR_GRAY2BGR);

                        if(state_video_stream == FIND_STEREO_STREAM)
                        {
                            if(countImg - 1 == countFrame_)
                                isEnd_ = true;
                            if(isPressSnap_) //если кнопка нажата
                            {
                                if(isDraw)
                                {
                                    calibProcessor_.isFramePattern(&drawFrameL, pattern_, CHECKERBOARD_[0], CHECKERBOARD_[1], checkerSize_, markerSize_, dictionary_);
                                    calibProcessor_.isFramePattern(&drawFrameR, pattern_, CHECKERBOARD_[0], CHECKERBOARD_[1], checkerSize_, markerSize_, dictionary_);
                                }
                                QPixmap imgInDisplayL = toMatQpixmap(drawFrameL);
                                QPixmap imgInDisplayR = toMatQpixmap(drawFrameR);

                                emit outDisplayFirstSecond(imgInDisplayL,imgInDisplayR,lock);

                                QPixmap imgInSaveL = toMatQpixmapGray(imgL);
                                QPixmap imgInSaveR = toMatQpixmapGray(imgR);
                                namepathL = QString(path_ + "Camera1/" + "Accumulated/" + QString::number(imageInfo1.size() + 1) + ".png");
                                namepathR = QString(path_ + "Camera2/" + "Accumulated/" + QString::number(imageInfo2.size() + 1) + ".png");
                                filesystem->saveInImg(imgInSaveL,namepathL);
                                filesystem->saveInImg(imgInSaveR,namepathR);


                                QTableWidgetItem *item0 = new QTableWidgetItem();
                                item0->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
                                item0->setCheckState(Qt::Checked);
                                QTableWidgetItem *item1 = new QTableWidgetItem("Camera1/Camera2");
                                QTableWidgetItem *item2 = new QTableWidgetItem("save");
                                emit setItem(item0, item1, item2);
                                FileSystem::InformationImageSaved tmpInfo1;
                                FileSystem::InformationImageSaved tmpInfo2;
                                tmpInfo1.cameraPath = namepathL.toStdString();
                                tmpInfo2.cameraPath = namepathR.toStdString();
                                imageInfo1.push_back(tmpInfo1);
                                imageInfo2.push_back(tmpInfo2);
                                filesystem->saveInfoCamera1(imageInfo1);
                                filesystem->saveInfoCamera2(imageInfo2);
                                countImg++;
                                isPressSnap_ = false;
                            }else{
                                if(isDraw)
                                {
                                    calibProcessor_.isFramePattern(&drawFrameL, pattern_, CHECKERBOARD_[0], CHECKERBOARD_[1], checkerSize_, markerSize_, dictionary_);
                                    calibProcessor_.isFramePattern(&drawFrameR, pattern_, CHECKERBOARD_[0], CHECKERBOARD_[1], checkerSize_, markerSize_, dictionary_);
                                }
                                QPixmap imgInDisplayL = toMatQpixmap(drawFrameL);
                                QPixmap imgInDisplayR = toMatQpixmap(drawFrameR);

                                emit outDisplayFirstSecond(imgInDisplayL,imgInDisplayR,lock);
                            }
                        }
                    }
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
                cv::Mat R1,P1,R2,P2;
                filesystem->readYamlMatrixFirst(&cameraMatrixFirst);
                filesystem->readYamldistCoefSecond(&distCoeffsFirst);
                filesystem->readYamlMatrixSecond(&cameraMatrixSecond);
                filesystem->readYamldistCoefSecond(&distCoeffsSecond);
                R1 = filesystem->getR1();
                R2 = filesystem->getR2();
                P1 = filesystem->getP1();
                P2 = filesystem->getP2();

                std::vector<FileSystem::InformationImageSaved> imageInfo = filesystem->getInfoCamera1();
                cv::Mat img1 = cv::imread(imageInfo[0].cameraPath);

                cv::initUndistortRectifyMap(cameraMatrixFirst, distCoeffsFirst, R1, P1, img1.size(),CV_16SC2, rmap[0][0], rmap[0][1]);
                cv::initUndistortRectifyMap(cameraMatrixSecond, distCoeffsSecond, R2, P2, img1.size(),CV_16SC2, rmap[1][0], rmap[1][1]);

                cv::Ptr<cv::StereoBM> sbm =  cv::StereoBM::create();

                isEnd_ = false;

                Baslercameras.StartGrabbing(Pylon::GrabStrategy_LatestImages, Pylon::GrabLoop_ProvidedByUser);
                while(Baslercameras.IsGrabbing() && !isEnd_)
                {
                    Baslercameras[0].RetrieveResult(5000, ptrGrabResultL, Pylon::TimeoutHandling_ThrowException);
                    Baslercameras[1].RetrieveResult(5000, ptrGrabResultR, Pylon::TimeoutHandling_ThrowException);

                    if (ptrGrabResultL->GrabSucceeded() && ptrGrabResultR->GrabSucceeded())
                    {

                        lock->lock();
                        inputFirst = cv::Mat(ptrGrabResultL->GetHeight(),ptrGrabResultL->GetWidth(),CV_8UC1,ptrGrabResultL->GetBuffer());
                        inputSecond = cv::Mat(ptrGrabResultR->GetHeight(),ptrGrabResultR->GetWidth(),CV_8UC1,ptrGrabResultR->GetBuffer());
                        lock->unlock();

                        cv::remap(inputFirst, outputFirst, rmap[0][0], rmap[0][1], cv::INTER_LINEAR, cv::BORDER_DEFAULT, cv::Scalar());
                        cv::remap(inputSecond, outputSecond, rmap[0][0], rmap[0][1], cv::INTER_LINEAR, cv::BORDER_DEFAULT, cv::Scalar());

                        sbm->compute(outputFirst, outputSecond, disparity);

                        QPixmap img = toMatQpixmap(disparity);
                        emit outDisplayFirst(img);
                    }
                }
            }
        }
        break;
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
        if(web_cam_.isOpened()==false)
        {
            emit sendTerminal("Camera 1 is not open!");
        }
    }
    if(state_video_stream == FIND_SECOND_STREAM)
    {
        numCam_= 2;
        web_cam_.open(filesystem->getIndexCameraSecond(), cv::CAP_DSHOW);
        web_cam_.set(cv::CAP_PROP_AUTOFOCUS, 0);
        web_cam_.set(cv::CAP_PROP_AUTO_EXPOSURE, 0.75);
        if(web_cam_.isOpened()==false)
        {
            emit sendTerminal("Camera 2 is not open!");
        }
    }
    if(state_video_stream == FIRST_STREAM || state_video_stream == FIRST_SECOND_STREAM || state_video_stream == FIRST_SECOND_STREAM_WHITHLINE ||
       state_video_stream == FIRST_CALIBRATED_STREAM || state_video_stream == FIRST_SECOND_CALIBRATED_STREAM ||
       state_video_stream == STEREO_STREAM || state_video_stream == STEREO_DEPTH_STREAM)
    {
        web_camFirst_.open(filesystem->getIndexCameraFirst(), cv::CAP_DSHOW);
        web_camFirst_.set(cv::CAP_PROP_AUTOFOCUS, 0);
        web_camFirst_.set(cv::CAP_PROP_AUTO_EXPOSURE, 0.75);
        if(web_camFirst_.isOpened()==false)
        {
            emit sendTerminal("Camera 1 is not open!");
        }
    }

    if(state_video_stream == SECOND_STREAM || state_video_stream == FIRST_SECOND_STREAM || state_video_stream == FIRST_SECOND_STREAM_WHITHLINE ||
       state_video_stream == SECOND_CALIBRATED_STREAM || state_video_stream == FIRST_SECOND_CALIBRATED_STREAM ||
       state_video_stream == STEREO_STREAM || state_video_stream == STEREO_DEPTH_STREAM)
    {
        web_camSecond_.open(filesystem->getIndexCameraSecond(), cv::CAP_DSHOW);
        web_camSecond_.set(cv::CAP_PROP_AUTOFOCUS, 0);
        web_camSecond_.set(cv::CAP_PROP_AUTO_EXPOSURE, 0.75);
        if(web_camSecond_.isOpened()==false)
        {
            emit sendTerminal("Camera 2 is not open!");
        }
    }
}

void ImageProcessor::setDeviceState(DeviceState state)
{
    state_device = state;
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

void ImageProcessor::setIsDraw(bool is)
{
    isDraw = is;
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

QPixmap ImageProcessor::toMatQpixmapGray(cv::Mat mat)
{
    return QPixmap::fromImage(
           QImage(mat.data,
                  mat.cols,
                  mat.rows,
                  mat.step,
                  QImage::Format_Grayscale8));
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


