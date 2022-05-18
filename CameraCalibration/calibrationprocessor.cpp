#include "calibrationprocessor.h"

CalibrationProcessor::CalibrationProcessor(FileSystem* fs)
{
    fs_ = fs;
}
CalibrationProcessor::CalibrationProcessor()
{
}

void CalibrationProcessor::setState(State state)
{
    state_ = state;
}


void CalibrationProcessor::run()
{
    targetType_ = fs_->getPattern();
    CHECKERBOARD_[0] = fs_->getRow();
    CHECKERBOARD_[1] = fs_->getCol();

    switch (state_){
        case ACCUMULATION:
    {
        std::vector<FileSystem::InformationImageSaved> imageInfo1 = fs_->getInfoCamera1();
        std::vector<FileSystem::InformationImageSaved> imageInfo2 = fs_->getInfoCamera2();

        accumulation(imageInfo1);
        fs_->saveInfoCamera1(imageInfo1);

        accumulation(imageInfo2);
        fs_->saveInfoCamera2(imageInfo2);

        emit updateCantrolUi();
        break;
    }case CALIBRATION:
        qDebug()<<"case CALIBRATION";
        FileSystem::SettingCalibration setting = fs_->getCalibrationSetting();
        qDebug()<<"fs_->getCalibrationSetting()";
        if(setting.isCaibration)
        {
            if(setting.numCamera == 1)
            {
                qDebug()<<"setting.numCamera == 1";
                std::vector<FileSystem::InformationImageSaved> imageInfo1 = fs_->getInfoCamera1();
                cameraCalibration(imageInfo1, setting);
            }else if(setting.numCamera == 1)
            {
                std::vector<FileSystem::InformationImageSaved> imageInfo2 = fs_->getInfoCamera2();
                cameraCalibration(imageInfo2, setting);
            }
            emit updateCantrolUi();
        }
        break;
    }


//    if(fs_->getIndexCameraFirst()!=-1 && fs_->isCalibration()==1)
//    {
//        reloadVectors();
//        numCam_=1;
//        imageInfo1 = fs_->getInfoCamera1();
//        singleCalibration(imageInfo1);
//        fs_->saveInfoCamera1(imageInfo1);
//    }
//    if(fs_->getIndexCameraSecond()!=-1 && fs_->isCalibration()==1)
//    {
//        reloadVectors();
//        numCam_ = 2;
//        imageInfo2 = fs_->getInfoCamera2();
//        singleCalibration(imageInfo2);
//        fs_->saveInfoCamera2(imageInfo2);
//    }
//    if(fs_->getIndexCameraFirst()!=-1 && fs_->getIndexCameraSecond()!=-1 && fs_->isStereoCalibration() == 1)
//    {
//        reloadVectors();
//        stereoCalibration();
//    }
}

void CalibrationProcessor::cameraCalibration(std::vector<FileSystem::InformationImageSaved>& imageInfo, FileSystem::SettingCalibration& setting)
{
    qDebug()<<"cameraCalibration";
    std::vector<std::vector<cv::Point3f>> objpoints;
    std::vector<std::vector<cv::Point2f>> imgpoints;
    std::vector<cv::Point3f> objp;
    std::vector<int> indexImages;
    for(int i{0}; i<CHECKERBOARD_[1]; i++)
    {
      for(int j{0}; j<CHECKERBOARD_[0]; j++)
        objp.push_back(cv::Point3f(j,i,0));
    }
    qDebug()<<"objp";
    int countImage = 0;
    for(int i = 0;imageInfo.size() > i;i++)
    {
        if(imageInfo[i].isActive == 1)
        {
            countImage++;
            objpoints.push_back(objp);
            qDebug()<<"objpoints.push_back(objp);";
            imgpoints.push_back(imageInfo[i].imgpoint);
            qDebug()<<"imgpoints.push_back(imageInfo[i].imgpoint);";
            indexImages.push_back(i);
        }
    }
   qDebug()<<"imageInfo";

    switch (setting.cameraModel)
    {
    case FileSystem::SettingCalibration::OPENCV:
    {
        qDebug()<<"FileSystem::SettingCalibration::OPENCV";

        int calibrationFlags = 0;
        if(setting.isUseParametr)
            calibrationFlags = calibrationFlags | translateFlagsOpencv("CV_CALIB_USE_INTRINSIC_GUESS");
        if(setting.isFixedFocal)
            calibrationFlags = calibrationFlags | translateFlagsOpencv("CV_CALIB_FIX_FOCAL_LENGTH");
        if(setting.isFixedFocal)
            calibrationFlags = calibrationFlags | translateFlagsOpencv("CV_CALIB_FIX_ASPECT_RATIO");
        if(setting.isFixedPrincipalPoint)
            calibrationFlags = calibrationFlags | translateFlagsOpencv("CV_CALIB_FIX_PRINCIPAL_POINT");
        if(setting.iszeroTangent)
            calibrationFlags = calibrationFlags | translateFlagsOpencv("CV_CALIB_FIX_FOCAL_LENGTH");
        if(setting.isfixedK1)
            calibrationFlags = calibrationFlags | translateFlagsOpencv("CV_CALIB_FIX_K1");
        if(setting.isfixedK2)
            calibrationFlags = calibrationFlags | translateFlagsOpencv("CV_CALIB_FIX_K2");
        if(setting.isfixedK3)
            calibrationFlags = calibrationFlags | translateFlagsOpencv("CV_CALIB_FIX_K3");
        if(setting.isfixedK4)
            calibrationFlags = calibrationFlags | translateFlagsOpencv("CV_CALIB_FIX_K4");

        qDebug()<<"calibrationFlags";

        cv::Mat cameraMatrix(3, 3, cv::DataType<double>::type);
        cameraMatrix.at<double>(0) = setting.fx;
        cameraMatrix.at<double>(1) = 0;
        cameraMatrix.at<double>(2) = setting.cx;
        cameraMatrix.at<double>(3) = 0;
        cameraMatrix.at<double>(4) = setting.fy;
        cameraMatrix.at<double>(5) = setting.cy;
        cameraMatrix.at<double>(6) = 0;
        cameraMatrix.at<double>(7) = 0;
        cameraMatrix.at<double>(8) = 1;

        cv::Mat disCoeffs(1, 5, cv::DataType<double>::type);
        disCoeffs.at<double>(0) = setting.k1;//k1
        disCoeffs.at<double>(1) = setting.k2;//k2
        disCoeffs.at<double>(2) = 0;//p1
        disCoeffs.at<double>(3) = 0;//p2
        disCoeffs.at<double>(4) = setting.k1;//k3

        std::vector<cv::Mat> R, T;

        cv::Mat img = cv::imread(static_cast<std::string>(imageInfo[0].cameraPath));

        double rmse;
        if(targetType_ == "Chessboard" || targetType_ == "Circles" || targetType_ == "Assymetric Circles")
        {
            cv::calibrateCamera(objpoints, imgpoints, cv::Size(img.rows, img.cols),
                                cameraMatrix, disCoeffs, R, T, calibrationFlags_);
            rmse = Rmse(imageInfo,imgpoints, objpoints, cameraMatrix, disCoeffs,R,T,indexImages);
            createImgUndistorted(imageInfo,cameraMatrix, disCoeffs, setting.numCamera);
        }

        QDateTime date;
        date = QDateTime::currentDateTime();
        std::string sizePatern;
        if(targetType_ == "Chessboard" || targetType_ == "Circles" || targetType_ == "Assymetric Circles")
            sizePatern = std::to_string(CHECKERBOARD_[0]) + " x " + std::to_string(CHECKERBOARD_[1]);
        else if(targetType_ == "ChArUco")
            sizePatern = std::to_string(CHECKERBOARD_[0]) + " x " + std::to_string(CHECKERBOARD_[1]) +
                    "\n" + "MarkerSize:" + std::to_string(markerSize_) + "\n" + "CheckerSize:" + std::to_string(checkerSize_) +
                    "\n" + dictionaryName_.toStdString();

        fs_->saveFileInYaml(objpoints,imgpoints, cameraMatrix, disCoeffs, R, T, countImage, date.toString("yyyy.dd.M--HH:mm:ss"), rmse, targetType_,
                            sizePatern, setting.numCamera, cv::Size(img.rows, img.cols));
        emit updateCantrolUi();


        break;
    }
    case FileSystem::SettingCalibration::OPENCV_FISHEYE:
    {
        break;
    }
    }



//    if(targetType_ == "Chessboard" || targetType_ == "Circles" || targetType_ == "Assymetric Circles")
//    {
//        //cv::calibrateCamera(objpoints_, imgpoints_, cv::Size(gray.rows, gray.cols),
//        //                    cameraMatrix_, distCoeffs_, R_, T_, calibrationFlags_);
//        //rmse_ = Rmse(imageInfo);
//    }
//    if(targetType_ == "ChArUco")
//    {
//        rmse_ = cv::aruco::calibrateCameraCharuco(allCharucoCorners, allCharucoIds,
//                                                  charucoboard_, imgSizeCharuco_, cameraMatrix_, distCoeffs_,
//                                                  R_, T_, calibrationFlags_);
//    }



//    QDateTime date;
//    date = QDateTime::currentDateTime();
//    int countImg;
//    countImg = imageInfo.size();

//    std::string sizePatern;
//    if(targetType_ == "Chessboard" || targetType_ == "Circles" || targetType_ == "Assymetric Circles")
//        sizePatern = std::to_string(CHECKERBOARD_[0]) + " x " + std::to_string(CHECKERBOARD_[1]);
//    else if(targetType_ == "ChArUco")
//        sizePatern = std::to_string(CHECKERBOARD_[0]) + " x " + std::to_string(CHECKERBOARD_[1]) +
//                "\n" + "MarkerSize:" + std::to_string(markerSize_) + "\n" + "CheckerSize:" + std::to_string(checkerSize_) +
//                "\n" + dictionaryName_.toStdString();

    //fs_->saveFileInYaml(objpoints_, imgpoints_, cameraMatrix_, distCoeffs_, R_, T_, countImg, date.toString("yyyy.dd.M--HH:mm:ss"),
    //                    rmse_, targetType_, sizePatern, numCam_,cv::Size(gray.rows, gray.cols));

//    emit sendCalibBrowser();
}

void CalibrationProcessor::stereoCalibration()
{
    //поиск калибровочных углов у двух идентичных изображениях

    QVector<QString> vectorPathImgFirst = fs_->getVectorPath(1);
    QVector<QString> vectorPathImgSecond = fs_->getVectorPath(2);

    if(vectorPathImgFirst.size() != vectorPathImgSecond.size())
    {
        return;
    }

    int nimages = vectorPathImgSecond.size();

    std::vector<std::vector<cv::Point2f>> imagePoints0;
    std::vector<std::vector<cv::Point2f>> imagePoints1;
    std::vector<cv::Point3f> objp;
    std::vector<std::vector<cv::Point3f>> objectPoints;

    float squareSize = 0.02f;

    for(int j = 0; j < CHECKERBOARD_[0]; j++ )
        for(int k = 0; k < CHECKERBOARD_[1]; k++ )
            objp.push_back(cv::Point3f(j*squareSize, k*squareSize, 0));

    int successPair = 0;
    cv::Size imageSize;
    for(int i=0;i<nimages;i++)
    {
        cv::Mat gray1,gray2;
        cv::Mat img1 = cv::imread(vectorPathImgFirst[i].toStdString());
        cv::Mat img2 = cv::imread(vectorPathImgSecond[i].toStdString());

        if(img1.size()!=img2.size()) //если размер изображений не равный
            continue;

        imageSize=img1.size();
        cv::cvtColor(img1,gray1,cv::COLOR_BGR2GRAY);
        cv::cvtColor(img2,gray2,cv::COLOR_BGR2GRAY);
        bool isSuccessFirst,isSuccessSecond;

        if(targetType_ == "Chessboard")
        {
            std::vector<cv::Point2f> corner_pts1;
            std::vector<cv::Point2f> corner_pts2;
            isSuccessFirst = cv::findChessboardCorners(gray1,cv::Size(CHECKERBOARD_[0], CHECKERBOARD_[1]), corner_pts1);
            isSuccessSecond = cv::findChessboardCorners(gray2,cv::Size(CHECKERBOARD_[0], CHECKERBOARD_[1]), corner_pts2);
            if(isSuccessFirst & isSuccessSecond)
            {
                cv::TermCriteria criteria(cv::TermCriteria::EPS | cv::TermCriteria::MAX_ITER, 30, 0.001);
                cv::TermCriteria criteria2(cv::TermCriteria::EPS | cv::TermCriteria::MAX_ITER, 30, 0.001);
                cv::cornerSubPix(gray1,corner_pts1, cv::Size(11,11), cv::Size(-1,-1), criteria);
                cv::cornerSubPix(gray2,corner_pts2, cv::Size(11,11), cv::Size(-1,-1), criteria2);
                imagePoints0.push_back(corner_pts1);
                imagePoints1.push_back(corner_pts2);
                objectPoints.push_back(objp);
                successPair++;
            }
        }
    }
    nimages = successPair;
    imagePoints0.resize(nimages);
    imagePoints1.resize(nimages);

    //на этом этапе надо порешать с флагами
    cv::Mat cameraMatrix[2], distCoeffs[2];
    cameraMatrix[0] = cv::Mat::eye(3, 3, CV_64F);
    cameraMatrix[1] = cv::Mat::eye(3, 3, CV_64F);
    cv::Mat R, T, E, F;

    //добавить флаги
    double rms = stereoCalibrate(objectPoints, imagePoints0, imagePoints1,
                                cameraMatrix[0], distCoeffs[0],
                                cameraMatrix[1], distCoeffs[1],
                                imageSize, R, T, E, F,
                                cv::CALIB_SAME_FOCAL_LENGTH + cv::CALIB_FIX_K3);

    // CALIBRATION QUALITY CHECK
    // because the output fundamental matrix implicitly
    // includes all the output information,
    // we can check the quality of calibration using the
    // epipolar geometry constraint: m2^t*F*m1=0

    double averageReprojectionErr;
    double err = 0;
    int npoints = 0;
    std::vector<cv::Vec3f> lines[2];
    for(int i = 0;i < nimages;i++)
    {
        int npt = (int)imagePoints0[i].size();
        cv::Mat imgpt[2];
        for( int k = 0; k < 2; k++ )
        {
            if(k==0)
                imgpt[k] = cv::Mat(imagePoints0[i]);
            else
                imgpt[k] = cv::Mat(imagePoints1[i]);

            cv::undistortPoints(imgpt[k], imgpt[k], cameraMatrix[k], distCoeffs[k], cv::Mat(), cameraMatrix[k]);
            cv::computeCorrespondEpilines(imgpt[k], k+1, F, lines[k]);
        }
        for(int j = 0; j < npt; j++ )
        {
            double errij = fabs(imagePoints0[i][j].x*lines[1][j][0] +
                                imagePoints0[i][j].y*lines[1][j][1] + lines[1][j][2]) +
                           fabs(imagePoints1[i][j].x*lines[0][j][0] +
                                imagePoints1[i][j].y*lines[0][j][1] + lines[0][j][2]);
            err += errij;
        }
        npoints += npt;
    }
    averageReprojectionErr = err/npoints;

    cv::Mat R1, R2, P1, P2, Q;
    cv::Rect validRoi[2];
    cv::stereoRectify(cameraMatrix[0], distCoeffs[0],
                   cameraMatrix[1], distCoeffs[1],
                   imageSize, R, T, R1, R2, P1, P2, Q, cv::CALIB_ZERO_DISPARITY, 0.5, imageSize, &validRoi[0], &validRoi[1]);

    cv::Mat rmap[2][2];
    cv::initUndistortRectifyMap(cameraMatrix[0], distCoeffs[0], R1, P1, imageSize, CV_16SC2, rmap[0][0], rmap[0][1]);
    cv::initUndistortRectifyMap(cameraMatrix[1], distCoeffs[1], R2, P2, imageSize, CV_16SC2, rmap[1][0], rmap[1][1]);

    qDebug()<<"RMSE " <<rms;
    qDebug()<<"averageReprojectionErr " <<averageReprojectionErr;
    /////save in YAMl
    QDateTime date;
    date = QDateTime::currentDateTime();

    std::string sizePatern;
    if(targetType_ == "Chessboard" || targetType_ == "Circles" || targetType_ == "Assymetric Circles")
        sizePatern = std::to_string(CHECKERBOARD_[0]) + " x " + std::to_string(CHECKERBOARD_[1]);
    else if(targetType_ == "ChArUco")
        sizePatern = std::to_string(CHECKERBOARD_[0]) + " x " + std::to_string(CHECKERBOARD_[1]) +
                "\n" + "MarkerSize:" + std::to_string(markerSize_) + "\n" + "CheckerSize:" + std::to_string(checkerSize_) +
                "\n" + dictionaryName_.toStdString();

    fs_->saveFileInYamlStereo(cameraMatrix[0],cameraMatrix[1], distCoeffs[0],distCoeffs[1], R, T, E, F, R1, R2, P1, P2, Q,
                              imageSize, validRoi[0],validRoi[1], rmap[0][0], rmap[0][1], rmap[1][0], rmap[1][1],
                              rms, averageReprojectionErr,date.toString("yyyy.dd.M--HH:mm:ss"), targetType_, sizePatern, nimages);
    /////

    cv::Mat canvas;
    double sf;
    int w, h;
    sf = 600./MAX(imageSize.width, imageSize.height);
    w = cvRound(imageSize.width*sf);
    h = cvRound(imageSize.height*sf);
    canvas.create(h, w*2, CV_8UC3);

    //вывод откалибровочных изображений
    for(int i = 0;i<nimages;i++)
    {
        for(int k=0;k<2;k++)
        {
            cv::Mat rempImg,colorImg;
            if(k==0)
                colorImg = cv::imread(vectorPathImgFirst[i].toStdString());
            else
                colorImg = cv::imread(vectorPathImgSecond[i].toStdString());

            cv::remap(colorImg, rempImg, rmap[k][0], rmap[k][1], cv::INTER_LINEAR);
            cv::Mat canvasPart = canvas(cv::Rect(w*k, 0, w, h));
            resize(colorImg, canvasPart, canvasPart.size(), 0, 0, cv::INTER_AREA);
            cv::Rect vroi(cvRound(validRoi[k].x*sf), cvRound(validRoi[k].y*sf),
                      cvRound(validRoi[k].width*sf), cvRound(validRoi[k].height*sf));
            cv::rectangle(canvasPart, vroi, cv::Scalar(0,0,255), 3, 8);
        }

         for(int j = 0; j < canvas.rows; j += 16 )
             line(canvas, cv::Point(0, j), cv::Point(canvas.cols, j), cv::Scalar(0, 255, 0), 1, 8);
         imshow("rectified", canvas);
    }

}

void CalibrationProcessor::accumulation(std::vector<FileSystem::InformationImageSaved>& imageInfo)
{
//    std::vector<cv::Point3f> objp;
//    for(int i{0};i<CHECKERBOARD_[1];i++)
//    {
//        for(int j{0}; j<CHECKERBOARD_[0]; j++)
//            objp.push_back(cv::Point3f(j,i,0));
//    }
    std::vector<cv::Point2f> corner_pts; // Вектор для хранения пикселей координат углов шахматной доски

    if(targetType_ == "ChArUco")
    {
        dictionary_ = cv::aruco::getPredefinedDictionary(intDictionary_);
        charucoboard_ = cv::aruco::CharucoBoard::create(CHECKERBOARD_[1], CHECKERBOARD_[0], checkerSize_, markerSize_, dictionary_);
        params_ = cv::aruco::DetectorParameters::create();
    }
    cv::Mat gray;
    for(int i = 0;i<imageInfo.size();i++)
    {
        inputFrame_ = cv::imread(static_cast<std::string>(imageInfo[i].cameraPath));
        cv::cvtColor(inputFrame_,gray,cv::COLOR_BGR2GRAY);

        if(targetType_ == "Chessboard")
            chessboardAccumulation(i, gray,corner_pts,imageInfo);
        //if(targetType_ == "Circles")
            //circleAccumulation(i, gray,corner_pts,imageInfo);
        //if(targetType_ == "Assymetric Circles")
            //aCircleAccumulation(i, gray,corner_pts,imageInfo);

//        if(targetType_ == "ChArUco")
//            charucoAccumulation(i,imageInfo);
    }
    if(targetType_ == "ChArUco")
            imgSizeCharuco_ = inputFrame_.size();
    //cameraCalibration(imageInfo);
    //emit sendOpenFileInViewYamlCalib(fs_->getFilePath());
}

void CalibrationProcessor::charucoAccumulation(int i,std::vector<FileSystem::InformationImageSaved>& imageInfo)
{
    QString filename;
    cv::Mat inputFrameCopy;
    inputFrame_.copyTo(inputFrameCopy);

    std::vector<int> markerIds;
    std::vector<std::vector<cv::Point2f>> markerCorners;

    cv::aruco::detectMarkers(inputFrame_, charucoboard_->dictionary, markerCorners, markerIds, params_);

    if (markerIds.size() > 0) {
        cv::Mat currentCharucoCorners, currentCharucoIds;
        cv::aruco::interpolateCornersCharuco(markerCorners, markerIds, inputFrame_, charucoboard_, currentCharucoCorners,currentCharucoIds);

        cv::aruco::drawDetectedMarkers(inputFrameCopy, markerCorners, markerIds);

        if(currentCharucoCorners.total() > 0)
                    cv::aruco::drawDetectedCornersCharuco(inputFrameCopy, currentCharucoCorners, currentCharucoIds);

        std::vector<cv::Point2f> charucoCorners;
        std::vector<int> charucoIds;
        cv::aruco::interpolateCornersCharuco(markerCorners, markerIds, inputFrame_, charucoboard_, charucoCorners, charucoIds);
        if (charucoIds.size() > 0)
            cv::aruco::drawDetectedCornersCharuco(inputFrameCopy, charucoCorners, charucoIds, cv::Scalar(255, 0, 0));

        allCharucoIds.push_back(charucoIds);
        allCharucoCorners.push_back(charucoCorners);

        //emit sendStatusImg("Success", i);

        QPixmap saveImg = QPixmap::fromImage(
                          QImage(inputFrameCopy.data,
                                  inputFrameCopy.cols,
                                  inputFrameCopy.rows,
                                  inputFrameCopy.step,
                         QImage::Format_RGB888).rgbSwapped());

       filename = QString::number(i + 1) + ".png";
       saveInImgDrawing(saveImg,filename,i,imageInfo);
       imageInfo[i].state = "Succes";
    }else
        imageInfo[i].state = "Bad";
}

void CalibrationProcessor::chessboardAccumulation(int i,cv::Mat gray,std::vector<cv::Point2f> corner_pts,
                                                  std::vector<FileSystem::InformationImageSaved>& imageInfo)
{
    bool isSuccess = false;
    isSuccess = cv::findChessboardCorners(gray,cv::Size(CHECKERBOARD_[0], CHECKERBOARD_[1]), corner_pts);
    if(isSuccess)
    {
        cv::TermCriteria criteria(cv::TermCriteria::EPS | cv::TermCriteria::MAX_ITER, 30, 0.001);
        cv::cornerSubPix(gray,corner_pts, cv::Size(11,11), cv::Size(-1,-1), criteria);

       imageInfo[i].imgpoint = corner_pts;
       imageInfo[i].state = "Succes";
    }else
    {
        imageInfo[i].state= "NoFind";
        imageInfo[i].isActive = 0;
    }
}

void CalibrationProcessor::circleAccumulation(int i,cv::Mat gray,std::vector<cv::Point2f> corner_pts,
                                              std::vector<FileSystem::InformationImageSaved>& imageInfo)
{
    bool isSuccess = false;
    isSuccess = cv::findCirclesGrid(gray, cv::Size(CHECKERBOARD_[0], CHECKERBOARD_[1]), corner_pts,
                cv::CALIB_CB_SYMMETRIC_GRID, cv::SimpleBlobDetector::create());
    if(isSuccess)
    {
       imageInfo[i].imgpoint = corner_pts;
       imageInfo[i].state = "Succes";
    }else
    {
        imageInfo[i].state= "NoFind";
        imageInfo[i].isActive = 0;
    }
}

void CalibrationProcessor::aCircleAccumulation(int i,cv::Mat gray,std::vector<cv::Point2f> corner_pts,
                                               std::vector<FileSystem::InformationImageSaved>& imageInfo)
{
    bool isSuccess = false;
    isSuccess = cv::findCirclesGrid(gray, cv::Size(CHECKERBOARD_[0], CHECKERBOARD_[1]), corner_pts,
                cv::CALIB_CB_ASYMMETRIC_GRID, cv::SimpleBlobDetector::create());
    if(isSuccess)
    {
        imageInfo[i].imgpoint = corner_pts;
        imageInfo[i].state = "Succes";
    }else
    {
        imageInfo[i].state= "NoFind";
        imageInfo[i].isActive = 0;
    }
}

void CalibrationProcessor::reloadVectors()
{

}

int CalibrationProcessor::translateFlagsOpencv(QString textFlag)
{
    if(textFlag == "CV_CALIB_USE_INTRINSIC_GUESS") return 0x00001;
    if(textFlag == "CV_CALIB_FIX_ASPECT_RATIO") return 0x00002;
    if(textFlag == "CV_CALIB_FIX_PRINCIPAL_POINT") return 0x00004;
    if(textFlag == "CV_CALIB_ZERO_TANGENT_DIST") return 0x00008;
    if(textFlag == "CV_CALIB_FIX_FOCAL_LENGTH") return 0x00010;
    if(textFlag == "CV_CALIB_FIX_K1") return 0x00020;
    if(textFlag == "CV_CALIB_FIX_K2") return 0x00040;
    if(textFlag == "CV_CALIB_FIX_K3") return 0x00080;
    if(textFlag == "CV_CALIB_FIX_K4") return 0x00800;
    else return 0;
}

int CalibrationProcessor::translateFlagsFisheye(QString textFlag)
{
//    if(textFlag == "CV_CALIB_USE_INTRINSIC_GUESS") return 1 << 0;
//    if(textFlag == "CV_CALIB_FIX_ASPECT_RATIO") return 0x00002;
//    if(textFlag == "CV_CALIB_FIX_PRINCIPAL_POINT") return 1 << 9;
//    if(textFlag == "CV_CALIB_ZERO_TANGENT_DIST") return 0x00008;
//    if(textFlag == "CV_CALIB_FIX_K1") return 0x00020;
//    if(textFlag == "CV_CALIB_FIX_K2") return 0x00040;
//    if(textFlag == "CV_CALIB_FIX_K3") return 0x00080;
//    if(textFlag == "CV_CALIB_FIX_K4") return 0x00800;
    return 0;
}


void CalibrationProcessor::setPattern(QString Pattern)
{
    targetType_ = Pattern;
}

void CalibrationProcessor::setRowCol(int row, int col)
{
    CHECKERBOARD_[0] = row;
    CHECKERBOARD_[1] = col;
}

void CalibrationProcessor::setMarkerSize(double markerSize)
{
    markerSize_ = markerSize;
}

void CalibrationProcessor::setCheckerSize(double checkerSize)
{
    checkerSize_ = checkerSize;
}

void CalibrationProcessor::setDictionaryName(QString dictionaryName)
{
    dictionaryName_ = dictionaryName;
}

void CalibrationProcessor::setCalibrationFlags(int calibrationFlags)
{
    calibrationFlags_ = calibrationFlags;
}

double CalibrationProcessor::Rmse(std::vector<FileSystem::InformationImageSaved>& imageInfo,
                                  std::vector<std::vector<cv::Point2f>> imgpoints,
                                  std::vector<std::vector<cv::Point3f>> objpoints,
                                  cv::Mat cameraMatrix,cv::Mat disCoeffs,
                                  std::vector<cv::Mat> R,std::vector<cv::Mat> T,
                                  std::vector<int> indexImages)
{

    std::vector<cv::Point2f> imagePoints2;
    double totalErr = 0;
    double err;
    size_t totalPoints=0;
    for(size_t i = 0; i < objpoints.size(); ++i )
    {
         projectPoints(objpoints[i], R[i], T[i], cameraMatrix, disCoeffs, imagePoints2);
         err = norm(imgpoints[i], imagePoints2, cv::NORM_L2);
         imageInfo[indexImages[i]].err = err;
         imageInfo[indexImages[i]].reprojectPoint = imagePoints2;
         size_t n = objpoints[i].size();
         totalErr        += err*err;
         totalPoints     += n;
    }
    return std::sqrt(totalErr/totalPoints);
}


bool CalibrationProcessor::isFramePattern(cv::Mat* frame, QString pattern, int row, int col,
                                          double icheckerSize, double imarkerSize, int idictionary)
{
    cv::Mat gray;
    cv::cvtColor(*frame, gray, cv::COLOR_BGR2GRAY);
    std::vector<cv::Point2f> corner_pts;

    if(pattern == "Chessboard")
    {
        if(cv::findChessboardCorners(gray, cv::Size(row, col), corner_pts, cv::CALIB_CB_FAST_CHECK))
        {
            cv::drawChessboardCorners(*frame, cv::Size(row, col), corner_pts, true);
            return true;
        }
    }
    if(pattern == "Circles")
    {
        if(cv::findCirclesGrid(gray, cv::Size(row, col), corner_pts, cv::CALIB_CB_SYMMETRIC_GRID, cv::SimpleBlobDetector::create()))
        {
            cv::drawChessboardCorners(*frame, cv::Size(row, col), cv::Mat(corner_pts), true);
            return true;
        }
    }
    if(pattern == "Assymetric Circles")
    {
        if(cv::findCirclesGrid(gray, cv::Size(row, col), corner_pts, cv::CALIB_CB_ASYMMETRIC_GRID, cv::SimpleBlobDetector::create()))
        {
            cv::drawChessboardCorners(*frame, cv::Size(row, col), cv::Mat(corner_pts), true);
            return true;
        }
    }
    if(pattern == "ChArUco")
    {
       cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(idictionary);
       cv::Ptr<cv::aruco::CharucoBoard> board = cv::aruco::CharucoBoard::create(col, row, icheckerSize, imarkerSize, dictionary);
       std::vector<int> markerIds;
       std::vector<std::vector<cv::Point2f>> markerCorners;
       cv::Ptr<cv::aruco::DetectorParameters> params = cv::aruco::DetectorParameters::create();

       cv::aruco::detectMarkers(gray, board->dictionary, markerCorners, markerIds, params);
       if (markerIds.size() > 0) {
           cv::Mat currentCharucoCorners, currentCharucoIds;
           cv::aruco::interpolateCornersCharuco(markerCorners, markerIds, *frame, board, currentCharucoCorners,currentCharucoIds);

          cv::aruco::drawDetectedMarkers(*frame, markerCorners, markerIds);

            if(currentCharucoCorners.total() > 0)
                        cv::aruco::drawDetectedCornersCharuco(*frame, currentCharucoCorners, currentCharucoIds);

            std::vector<cv::Point2f> charucoCorners;
            std::vector<int> charucoIds;
            cv::aruco::interpolateCornersCharuco(markerCorners, markerIds, *frame, board, charucoCorners, charucoIds);
             //if at least one charuco corner detected
            if (charucoIds.size() > 0)
                cv::aruco::drawDetectedCornersCharuco(*frame, charucoCorners, charucoIds, cv::Scalar(255, 0, 0));
            return true;
       }
    }
    return false;
}

void CalibrationProcessor::createImgUndistorted(std::vector<FileSystem::InformationImageSaved>& imageInfo,
                                                cv::Mat cameraMatrix,cv::Mat disCoeffs,
                                                int numCam)
{
    cv::Mat undist, frame;

    for(int i = 0; i < imageInfo.size(); i++){

        frame = cv::imread(static_cast<std::string>(imageInfo[i].cameraPath));
        cv::undistort(frame, undist, cameraMatrix, disCoeffs);

        QPixmap saveImg = QPixmap::fromImage(
                           QImage(undist.data,
                                  undist.cols,
                                  undist.rows,
                                  undist.step,
                           QImage::Format_RGB888).rgbSwapped());

        QString undistDir;
        if(numCam == 1)
            undistDir = fs_->getFilePath() + "Camera1/" + "Undistorted/" + QString::number(i+1) + ".png";
        else if(numCam == 2)
            undistDir = fs_->getFilePath() + "Camera2/" + "Undistorted/" + QString::number(i+1) + ".png";

        QFile file(undistDir);

        file.open(QIODevice::WriteOnly);
        saveImg.save(&file, "png");
        imageInfo[i].undistortedPath = undistDir.toStdString();
        file.close();
    }

    if(numCam == 1)
        fs_->saveInfoCamera1(imageInfo);
    else if(numCam == 2)
        fs_->saveInfoCamera2(imageInfo);

}

void CalibrationProcessor::saveInImgDrawing(QPixmap qpixmap, QString fileName,int i,
                                            std::vector<FileSystem::InformationImageSaved>& imageInfo)
{
//    QString path;
//    if(numCam_ == 1)
//        path = fs_->getFilePath() + "Camera1/" + "Drawnable/" + fileName;
//    else if(numCam_ == 2)
//        path = fs_->getFilePath() + "Camera2/" + "Drawnable/" + fileName;
//    QFile file(path);
//    file.open(QIODevice::WriteOnly);
//    qpixmap.save(&file, "png");
//    imageInfo[i].drawPath = path.toStdString();
//    file.close();
}

void CalibrationProcessor::setTargetType(QString qstring)
{
    targetType_ = qstring;
}

void CalibrationProcessor::setTargetSize(int row, int col, double markerSize, double checkerSize, QString dictionaryName)
{
    CHECKERBOARD_[0] = row;
    CHECKERBOARD_[1] = col;

    markerSize_ = markerSize;
    checkerSize_ = checkerSize;

    dictionaryName_ = dictionaryName;

    if(dictionaryName == "DICT_4X4_50") intDictionary_ = 0;
    else if(dictionaryName == "DICT_4X4_100") intDictionary_ = 1;
    else if(dictionaryName == "DICT_4X4_250") intDictionary_ = 2;
    else if(dictionaryName == "DICT_4X4_1000") intDictionary_ = 3;
    else if(dictionaryName == "DICT_5X5_50") intDictionary_ = 4;
    else if(dictionaryName == "DICT_5X5_100") intDictionary_ = 5;
    else if(dictionaryName == "DICT_5X5_250") intDictionary_ = 6;
    else if(dictionaryName == "DICT_5X5_1000") intDictionary_ = 7;
    else if(dictionaryName == "DICT_6X6_50") intDictionary_ = 8;
    else if(dictionaryName == "DICT_6X6_100") intDictionary_ = 9;
    else if(dictionaryName == "DICT_6X6_250") intDictionary_ = 10;
    else if(dictionaryName == "DICT_6X6_1000") intDictionary_ = 11;
    else if(dictionaryName == "DICT_7X7_50") intDictionary_ = 12;
    else if(dictionaryName == "DICT_7X7_100") intDictionary_ = 13;
    else if(dictionaryName == "DICT_7X7_250") intDictionary_ = 14;
    else if(dictionaryName == "DICT_7X7_1000") intDictionary_ = 15;
    else if(dictionaryName == "DICT_ARUCO_ORIGINAL") intDictionary_ = 16;
    else if(dictionaryName == "DICT_APRILTAG_16h5") intDictionary_ = 17;
    else if(dictionaryName == "DICT_APRILTAG_25h9") intDictionary_ = 18;
    else if(dictionaryName == "DICT_APRILTAG_36h10") intDictionary_ = 19;
    else if(dictionaryName == "NULL") intDictionary_ = -1;
}
