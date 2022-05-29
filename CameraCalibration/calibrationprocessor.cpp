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
        emit sendTerminalMessage("Corner detection started");
        succes = 0;
        bad = 0;

        std::vector<FileSystem::InformationImageSaved> imageInfo1 = fs_->getInfoCamera1();
        std::vector<FileSystem::InformationImageSaved> imageInfo2 = fs_->getInfoCamera2();

        accumulation(imageInfo1);
        fs_->saveInfoCamera1(imageInfo1);

        accumulation(imageInfo2);
        fs_->saveInfoCamera2(imageInfo2);

        emit sendTerminalMessage(QString("\n Successful images: %1 \n Unsuccessful images: %2").arg(succes).arg(bad));
        emit updateCantrolUi();
        break;
    }case CALIBRATION:
        emit sendTerminalMessage("Calibration started");
        FileSystem::SettingCalibration setting = fs_->getCalibrationSetting();
        if(setting.isCaibration)
        {
            if(setting.numCamera == 1)
            {
                fs_->zeroingCalibInfoCamera1();
                std::vector<FileSystem::InformationImageSaved> imageInfo1 = fs_->getInfoCamera1();
                cameraCalibration(imageInfo1, setting);
            }else if(setting.numCamera == 2)
            {
                fs_->zeroingCalibInfoCamera2();
                std::vector<FileSystem::InformationImageSaved> imageInfo2 = fs_->getInfoCamera2();
                cameraCalibration(imageInfo2, setting);
            }
            emit updateCantrolUi();
        }else if(setting.isStereoCaibration)
        {
            stereoCalibration(setting);
        }
        break;
    }
}

void CalibrationProcessor::cameraCalibration(std::vector<FileSystem::InformationImageSaved>& imageInfo, FileSystem::SettingCalibration& setting)
{
    qDebug()<<"cameraCalibration";
    std::vector<std::vector<cv::Point3f>> objpoints;
    std::vector<std::vector<int>> markerIds;
    std::vector<std::vector<cv::Point2f>> imgpoints;
    std::vector<cv::Point3f> objp;
    std::vector<int> indexImages;
    if(targetType_ == "Chessboard")
    {
        for(int i{0}; i<CHECKERBOARD_[1]; i++)
        {
          for(int j{0}; j<CHECKERBOARD_[0]; j++)
            objp.push_back(cv::Point3f(j,i,0));
        }
    }
    if(targetType_ == "Circles")
    {
        checkerSize_ = fs_->getCheckerSize();

        for(int i{0}; i<CHECKERBOARD_[0]; i++)
        {
          for(int j{0}; j<CHECKERBOARD_[1]; j++)
            objp.push_back(cv::Point3f(j*checkerSize_,i*checkerSize_,0));
        }
    }
    if(targetType_ == "Assymetric Circles")
    {
        checkerSize_ = fs_->getCheckerSize();
        for(int i{0}; i<CHECKERBOARD_[1]; i++)
        {
          for(int j{0}; j<CHECKERBOARD_[0]; j++)
            objp.push_back(cv::Point3f(2*j + i%2,i,0));
        }
    }

    int countImage = 0;
    for(int i = 0;imageInfo.size() > i;i++)
    {
        if(imageInfo[i].isActive == 1)
        {
            if(targetType_ == "ChArUco")
                markerIds.push_back(imageInfo[i].charucoIds);
            objpoints.push_back(objp);
            countImage++;
            imgpoints.push_back(imageInfo[i].imgpoint);
            indexImages.push_back(i);
            imageInfo[i].isCalib = 1;
        }
    }


    switch (setting.cameraModel)
    {
    case FileSystem::SettingCalibration::OPENCV:
    {
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
        disCoeffs.at<double>(4) = setting.k3;//k3

        std::vector<cv::Mat> R, T;

        cv::Mat img = cv::imread(static_cast<std::string>(imageInfo[0].cameraPath));

        double rmse;
        double meanErr = 0;
        if(targetType_ == "Chessboard" || targetType_ == "Circles" || targetType_ == "Assymetric Circles")
        {
            cv::calibrateCamera(objpoints, imgpoints, cv::Size(img.rows, img.cols),
                                cameraMatrix, disCoeffs, R, T, calibrationFlags_);
            rmse = Rmse(imageInfo,imgpoints, objpoints, cameraMatrix, disCoeffs,R,T,indexImages,meanErr);
            sendTerminalMessage(QString("Calibration successful \n RMSE: %1").arg(rmse));
            createImgUndistorted(imageInfo,cameraMatrix, disCoeffs, setting.numCamera);
        }else if(targetType_ == "ChArUco")
        {
            checkerSize_ = fs_->getCheckerSize();
            markerSize_ = fs_->getMarkerSize();
            dictionaryName_ = fs_->getDictionaryName();
            dictionary_ = cv::aruco::getPredefinedDictionary(getDictionary(dictionaryName_));
            charucoboard_ = cv::aruco::CharucoBoard::create(CHECKERBOARD_[1], CHECKERBOARD_[0], checkerSize_, markerSize_, dictionary_);
            params_ = cv::aruco::DetectorParameters::create();
            cv::aruco::calibrateCameraCharuco(imgpoints, markerIds, charucoboard_,cv::Size(img.rows, img.cols),
                                            cameraMatrix,disCoeffs,R, T, calibrationFlags_);

            objpoints.resize(markerIds.size());
            for(unsigned int i = 0; i < markerIds.size(); i++)
            {
                 unsigned int nCorners = (unsigned int)markerIds[i].size();
                 objpoints[i].reserve(nCorners);

                  for(unsigned int j = 0; j < nCorners; j++)
                  {
                      int pointId = markerIds[i][j];
                      objpoints[i].push_back(charucoboard_->chessboardCorners[pointId]);
                  }
            }
            rmse = Rmse(imageInfo,imgpoints, objpoints, cameraMatrix, disCoeffs,R,T,indexImages,meanErr);
            sendTerminalMessage(QString("Calibration successful \n RMSE: %1").arg(rmse));
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

        fs_->saveFileInYaml(objpoints,imgpoints, cameraMatrix, disCoeffs, R, T, countImage, date.toString("yyyy.dd.M--HH:mm:ss"), rmse,meanErr, targetType_,
                            sizePatern, setting.numCamera, cv::Size(img.rows, img.cols));
        emit updateCantrolUi();
        break;
    }
    case FileSystem::SettingCalibration::OPENCV_FISHEYE:
    {
        break;
    }
    }

}

void CalibrationProcessor::stereoCalibration(FileSystem::SettingCalibration setting)
{
    std::vector<FileSystem::InformationImageSaved> imageInfo1 = fs_->getInfoCamera1();
    std::vector<FileSystem::InformationImageSaved> imageInfo2 = fs_->getInfoCamera2();
    //поиск калибровочных углов у двух идентичных изображениях

    std::vector<std::vector<cv::Point2f>> imagePoints0;
    std::vector<std::vector<cv::Point2f>> imagePoints1;
    int countImg = 0, size_int=0;
    if(imageInfo1.size() >= imageInfo2.size())
        size_int = imageInfo2.size();
    else if(imageInfo1.size() <= imageInfo2.size())
        size_int = imageInfo1.size();

    for(int i = 0;i<size_int;i++)
    {
        if(imageInfo1[i].isActive && imageInfo2[i].isActive)
        {
            countImg++;
            imagePoints0.push_back(imageInfo1[i].imgpoint);
            imagePoints1.push_back(imageInfo2[i].imgpoint);
            if(imageInfo1[i].imgpoint.size() != imageInfo2[i].imgpoint.size())
            {
                sendTerminalMessage(QString("The number of detected points must be equal"));
                return;
            }

        }
    }

    std::vector<cv::Point3f> objp;
    std::vector<std::vector<cv::Point3f>> objectPoints;
    double squareSize = checkerSize_ = fs_->getCheckerSize();

    for(int j = 0; j < CHECKERBOARD_[0]; j++ )
        for(int k = 0; k < CHECKERBOARD_[1]; k++ )
            objp.push_back(cv::Point3f(j*squareSize, k*squareSize, 0));

    for(int i = 0; i < countImg; i++)
    {
        objectPoints.push_back(objp);
    }

    cv::Size imageSize ;
    cv::Mat img1 = cv::imread(imageInfo1[0].cameraPath);
    imageSize=img1.size();

    //на этом этапе надо порешать с флагами

    cv::Mat R,T;

    cv::Mat E, F;
    int flag = 0;

    if(setting.isSameFocalLengStereo)
        flag |= cv::CALIB_SAME_FOCAL_LENGTH;
    if(setting.isfixAspectRatioStereo)
        flag |= cv::CALIB_FIX_ASPECT_RATIO;
    if(setting.isfixFocalLengthStereo)
        flag |= cv::CALIB_FIX_FOCAL_LENGTH;
    if(setting.isfixPrincipalPointStereo)
        flag |= cv::CALIB_FIX_PRINCIPAL_POINT;
    if(setting.isUseSingleCalibratedInStereo)
        flag |= cv::CALIB_FIX_INTRINSIC;
    if(setting.iszeroTangentS)
        flag |= cv::CALIB_ZERO_TANGENT_DIST;
    if(setting.isfixedK1S)
        flag |= cv::CALIB_FIX_K1;
    if(setting.isfixedK2S)
        flag |= cv::CALIB_FIX_K2;
    if(setting.isfixedK3S)
        flag |= cv::CALIB_FIX_K2;

    qDebug()<<flag;

    cv::Mat cameraMatrix[2];
    cv::Mat disCoeffs[2];

    cv::Mat camMatrix1(3, 3, cv::DataType<double>::type);
    camMatrix1.at<double>(0) = setting.fx1;
    camMatrix1.at<double>(1) = 0;
    camMatrix1.at<double>(2) = setting.cx1;
    camMatrix1.at<double>(3) = 0;
    camMatrix1.at<double>(4) = setting.fy1;
    camMatrix1.at<double>(5) = setting.cy1;
    camMatrix1.at<double>(6) = 0;
    camMatrix1.at<double>(7) = 0;
    camMatrix1.at<double>(8) = 1;

    camMatrix1.copyTo(cameraMatrix[0]);

    cv::Mat dist1(1, 5, cv::DataType<double>::type);
    dist1.at<double>(0) = setting.k1S1;//k1
    dist1.at<double>(1) = setting.k2S1;//k2
    dist1.at<double>(2) = 0;//p1
    dist1.at<double>(3) = 0;//p2
    dist1.at<double>(4) = setting.k3S1;//k3

    dist1.copyTo(disCoeffs[0]);

    cv::Mat camMatrix2(3, 3, cv::DataType<double>::type);
    camMatrix2.at<double>(0) = setting.fx2;
    camMatrix2.at<double>(1) = 0;
    camMatrix2.at<double>(2) = setting.cx2;
    camMatrix2.at<double>(3) = 0;
    camMatrix2.at<double>(4) = setting.fy2;
    camMatrix2.at<double>(5) = setting.cy2;
    camMatrix2.at<double>(6) = 0;
    camMatrix2.at<double>(7) = 0;
    camMatrix2.at<double>(8) = 1;

    camMatrix2.copyTo(cameraMatrix[1]);

    cv::Mat dist2(1, 5, cv::DataType<double>::type);
    dist2.at<double>(0) = setting.k1S2;//k1
    dist2.at<double>(1) = setting.k2S2;//k2
    dist2.at<double>(2) = 0;//p1
    dist2.at<double>(3) = 0;//p2
    dist2.at<double>(4) = setting.k3S2;//k3

    dist2.copyTo(disCoeffs[1]);


    qDebug()<<"double rms";

    //добавить флаги
    double rms = stereoCalibrate(objectPoints, imagePoints0, imagePoints1,
                                cameraMatrix[0], disCoeffs[0],
                                cameraMatrix[1], disCoeffs[1],
                                imageSize, R, T, E, F,flag,
            cv::TermCriteria(cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS, 30, 1e-6));

    // CALIBRATION QUALITY CHECK
    // because the output fundamental matrix implicitly
    // includes all the output information,
    // we can check the quality of calibration using the
    // epipolar geometry constraint: m2^t*F*m1=0

    double averageReprojectionErr;
    double err = 0;
    int npoints = 0;
    std::vector<cv::Vec3f> lines[2];
    for(int i = 0;i < countImg;i++)
    {
        int npt = (int)imagePoints0[i].size();
        cv::Mat imgpt[2];
        for( int k = 0; k < 2; k++ )
        {
            if(k==0)
                imgpt[k] = cv::Mat(imagePoints0[i]);
            else
                imgpt[k] = cv::Mat(imagePoints1[i]);

            cv::undistortPoints(imgpt[k], imgpt[k], cameraMatrix[k], disCoeffs[k], cv::Mat(), cameraMatrix[k]);
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


    qDebug()<<"RMSE " <<rms;
    qDebug()<<"averageReprojectionErr " <<averageReprojectionErr;

    cv::Mat R1, R2, P1, P2, Q;
    cv::Rect validRoi[2];
    qDebug()<<"stereoRectify";
    cv::stereoRectify(cameraMatrix[0], disCoeffs[0],
                   cameraMatrix[1], disCoeffs[1],
                   imageSize, R, T, R1, R2, P1, P2, Q, 1);

//    cv::Mat rmap[2][2];
//    cv::initUndistortRectifyMap(cameraMatrix[0], distCoeffs[0], R1, P1, imageSize, CV_16SC2, rmap[0][0], rmap[0][1]);
//    cv::initUndistortRectifyMap(cameraMatrix[1], distCoeffs[1], R2, P2, imageSize, CV_16SC2, rmap[1][0], rmap[1][1]);

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

    fs_->saveFileInYamlStereo(cameraMatrix[0],cameraMatrix[1], disCoeffs[0],disCoeffs[1], R, T, E, F, R1, R2, P1, P2, Q,
                              imageSize,rms, averageReprojectionErr,date.toString("yyyy.dd.M--HH:mm:ss"),
                                targetType_, sizePatern, countImg);

    sendTerminalMessage(QString("Calibration successful \n RMSE: %1").arg(rms));
}

void CalibrationProcessor::accumulation(std::vector<FileSystem::InformationImageSaved>& imageInfo)
{
    std::vector<cv::Point2f> corner_pts; // Вектор для хранения пикселей координат углов шахматной доски


    if(targetType_ == "ChArUco")
    {
        checkerSize_ = fs_->getCheckerSize();
        markerSize_ = fs_->getMarkerSize();
        dictionaryName_ = fs_->getDictionaryName();
        dictionary_ = cv::aruco::getPredefinedDictionary(getDictionary(dictionaryName_));
        charucoboard_ = cv::aruco::CharucoBoard::create(CHECKERBOARD_[1], CHECKERBOARD_[0], checkerSize_, markerSize_, dictionary_);
        params_ = cv::aruco::DetectorParameters::create();
    }
    cv::Mat gray;
    for(int i = 0;i<imageInfo.size();i++)
    {
        inputFrame_ = cv::imread(static_cast<std::string>(imageInfo[i].cameraPath));

        if(inputFrame_.channels() == 3)
            cv::cvtColor(inputFrame_,gray,cv::COLOR_BGR2GRAY);
        else
            gray = inputFrame_;

        if(targetType_ == "Chessboard")
            chessboardAccumulation(i, gray,corner_pts,imageInfo);
        if(targetType_ == "Circles")
            circleAccumulation(i, gray,corner_pts,imageInfo);
        if(targetType_ == "Assymetric Circles")
            aCircleAccumulation(i, gray,corner_pts,imageInfo);
        if(targetType_ == "ChArUco")
            charucoAccumulation(i,gray,corner_pts,imageInfo);
    }

}

void CalibrationProcessor::charucoAccumulation(int i,cv::Mat gray,std::vector<cv::Point2f> corner_pts, std::vector<FileSystem::InformationImageSaved>& imageInfo)
{
    std::vector<int> markerIds;
    std::vector<std::vector<cv::Point2f>> markerCorners;

    cv::aruco::detectMarkers(gray, charucoboard_->dictionary, markerCorners, markerIds, params_);

    if (markerIds.size() > 0) {
        std::vector<int> charucoIds;
        cv::aruco::interpolateCornersCharuco(markerCorners, markerIds, gray, charucoboard_, corner_pts, charucoIds);

        imageInfo[i].imgpoint = corner_pts;
        imageInfo[i].charucoIds = charucoIds;
        imageInfo[i].state = "Succes";
        succes++;
    }else
    {
        bad++;
        imageInfo[i].state = "NoFind";
        imageInfo[i].isActive = 0;
    }
}

void CalibrationProcessor::chessboardAccumulation(int i,cv::Mat gray,std::vector<cv::Point2f> corner_pts,
                                                  std::vector<FileSystem::InformationImageSaved>& imageInfo)
{
    bool isSuccess = false;
    isSuccess = cv::findChessboardCorners(gray,cv::Size(CHECKERBOARD_[0], CHECKERBOARD_[1]), corner_pts);
    if(isSuccess)
    {
        cv::TermCriteria criteria(cv::TermCriteria::EPS | cv::TermCriteria::MAX_ITER, 30, 0.0001);
        cv::cornerSubPix(gray,corner_pts, cv::Size(11,11), cv::Size(-1,-1), criteria);

       imageInfo[i].imgpoint = corner_pts;
       imageInfo[i].state = "Succes";
       succes++;
    }else
    {
        bad++;
        imageInfo[i].state= "NoFind";
        imageInfo[i].isActive = 0;
    }
}

void CalibrationProcessor::circleAccumulation(int i,cv::Mat gray,std::vector<cv::Point2f> corner_pts,
                                              std::vector<FileSystem::InformationImageSaved>& imageInfo)
{
    bool isSuccess = false;
    isSuccess = cv::findCirclesGrid(gray, cv::Size(CHECKERBOARD_[1], CHECKERBOARD_[0]), corner_pts,
                cv::CALIB_CB_SYMMETRIC_GRID, cv::SimpleBlobDetector::create());
    if(isSuccess)
    {
       imageInfo[i].imgpoint = corner_pts;
       imageInfo[i].state = "Succes";
       succes++;
    }else
    {
        bad++;
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
        succes++;
    }else
    {
        imageInfo[i].state= "NoFind";
        imageInfo[i].isActive = 0;
        bad++;
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
                                  std::vector<int> indexImages,double& meanErr)
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
         meanErr +=err;
         imageInfo[indexImages[i]].reprojectPoint = imagePoints2;
         size_t n = objpoints[i].size();
         totalErr        += err*err;
         totalPoints     += n;
    }
    meanErr /= objpoints.size();
    return std::sqrt(totalErr/totalPoints);
}

int CalibrationProcessor::getDictionary(QString dictionaryName)
{
    if(dictionaryName == "DICT_4X4_50") return 0;
    else if(dictionaryName == "DICT_4X4_50") return 1;
    else if(dictionaryName == "DICT_4X4_100") return 2;
    else if(dictionaryName == "DICT_4X4_1000") return 3;
    else if(dictionaryName == "DICT_5X5_50") return 4;
    else if(dictionaryName == "DICT_5X5_100") return 5;
    else if(dictionaryName == "DICT_5X5_250") return 6;
    else if(dictionaryName == "DICT_5X5_1000") return 7;
    else if(dictionaryName == "DICT_6X6_50") return 8;
    else if(dictionaryName == "DICT_6X6_100") return 9;
    else if(dictionaryName == "DICT_6X6_250") return 10;
    else if(dictionaryName == "DICT_6X6_1000") return 11;
    else if(dictionaryName == "DICT_7X7_50") return 12;
    else if(dictionaryName == "DICT_7X7_100") return 13;
    else if(dictionaryName == "DICT_7X7_250") return 14;
    else if(dictionaryName == "DICT_7X7_1000") return 15;
    else if(dictionaryName == "DICT_ARUCO_ORIGINAL") return 16;
    else if(dictionaryName == "DICT_APRILTAG_16h5") return 17;
    else if(dictionaryName == "DICT_APRILTAG_25h9") return 18;
    else if(dictionaryName == "DICT_APRILTAG_36h10") return 19;
    else if(dictionaryName == "NULL") return -1;
}


bool CalibrationProcessor::isFramePattern(cv::Mat* frame, QString pattern, int row, int col,
                                          double icheckerSize, double imarkerSize, int idictionary)
{
    std::vector<cv::Point2f> corner_pts;

    if(pattern == "Chessboard")
    {
        if(cv::findChessboardCorners(*frame, cv::Size(row, col), corner_pts, cv::CALIB_CB_FAST_CHECK))
        {
            cv::drawChessboardCorners(*frame, cv::Size(row, col), corner_pts, true);
            return true;
        }
    }
    if(pattern == "Circles")
    {
        if(cv::findCirclesGrid(*frame, cv::Size(col,row), corner_pts, cv::CALIB_CB_SYMMETRIC_GRID, cv::SimpleBlobDetector::create()))
        {
            cv::drawChessboardCorners(*frame, cv::Size(col,row), cv::Mat(corner_pts), true);
            return true;
        }
    }
    if(pattern == "Assymetric Circles")
    {
        if(cv::findCirclesGrid(*frame, cv::Size(row, col), corner_pts, cv::CALIB_CB_ASYMMETRIC_GRID, cv::SimpleBlobDetector::create()))
        {
            cv::drawChessboardCorners(*frame, cv::Size(row, col), cv::Mat(corner_pts), true);
            return true;
        }
    }
    if(pattern == "ChArUco")
    {
       cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(idictionary);
       cv::Ptr<cv::aruco::CharucoBoard> board = cv::aruco::CharucoBoard::create(row,col, icheckerSize, imarkerSize, dictionary);
       std::vector<int> markerIds;
       std::vector<std::vector<cv::Point2f>> markerCorners;
       cv::Ptr<cv::aruco::DetectorParameters> params = cv::aruco::DetectorParameters::create();

       cv::aruco::detectMarkers(*frame, board->dictionary, markerCorners, markerIds, params);
       if (markerIds.size() > 0) {
           cv::Mat currentCharucoCorners, currentCharucoIds;
           cv::aruco::interpolateCornersCharuco(markerCorners, markerIds, *frame, board, currentCharucoCorners,currentCharucoIds);

          cv::aruco::drawDetectedMarkers(*frame, markerCorners, markerIds);

            if(currentCharucoCorners.total() > 0)
                        cv::aruco::drawDetectedCornersCharuco(*frame, currentCharucoCorners);

            std::vector<cv::Point2f> charucoCorners;
            std::vector<int> charucoIds;
            cv::aruco::interpolateCornersCharuco(markerCorners, markerIds, *frame, board, charucoCorners, charucoIds);
             //if at least one charuco corner detected
//            if (charucoIds.size() > 0)
//                cv::aruco::drawDetectedCornersCharuco(*frame, charucoCorners, charucoIds, cv::Scalar(255, 0, 0));
            return true;
       }
    }
    return false;
}

void CalibrationProcessor::createImgUndistorted(std::vector<FileSystem::InformationImageSaved>& imageInfo,
                                                cv::Mat cameraMatrix,cv::Mat disCoeffs,
                                                int numCam)
{
    cv::Mat map1,map2, frame, undist;


    frame = cv::imread(static_cast<std::string>(imageInfo[0].cameraPath));
    initUndistortRectifyMap(cameraMatrix, disCoeffs,cv::Mat(),
                            getOptimalNewCameraMatrix(cameraMatrix, disCoeffs, frame.size(), 1, frame.size(), 0),
                            frame.size(),CV_16SC2,map1,map2);

    for(int i = 0; i < imageInfo.size(); i++){

        frame = cv::imread(static_cast<std::string>(imageInfo[i].cameraPath));

        for(int j = 0; j <= frame.rows; j += frame.rows/5)
            line(frame, cv::Point(0, j), cv::Point(frame.cols, j), cv::Scalar(0, 255, 0), 2, 8);

        for(int j = 0; j <= frame.cols; j += frame.cols/5)
            line(frame, cv::Point(j, 0), cv::Point(j, frame.rows), cv::Scalar(0, 255, 0), 2, 8);


        remap(frame, undist, map1, map2, cv::INTER_LINEAR);

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

//    if(dictionaryName == "DICT_4X4_50") intDictionary_ = 0;
//    else if(dictionaryName == "DICT_4X4_100") intDictionary_ = 1;
//    else if(dictionaryName == "DICT_4X4_250") intDictionary_ = 2;
//    else if(dictionaryName == "DICT_4X4_1000") intDictionary_ = 3;
//    else if(dictionaryName == "DICT_5X5_50") intDictionary_ = 4;
//    else if(dictionaryName == "DICT_5X5_100") intDictionary_ = 5;
//    else if(dictionaryName == "DICT_5X5_250") intDictionary_ = 6;
//    else if(dictionaryName == "DICT_5X5_1000") intDictionary_ = 7;
//    else if(dictionaryName == "DICT_6X6_50") intDictionary_ = 8;
//    else if(dictionaryName == "DICT_6X6_100") intDictionary_ = 9;
//    else if(dictionaryName == "DICT_6X6_250") intDictionary_ = 10;
//    else if(dictionaryName == "DICT_6X6_1000") intDictionary_ = 11;
//    else if(dictionaryName == "DICT_7X7_50") intDictionary_ = 12;
//    else if(dictionaryName == "DICT_7X7_100") intDictionary_ = 13;
//    else if(dictionaryName == "DICT_7X7_250") intDictionary_ = 14;
//    else if(dictionaryName == "DICT_7X7_1000") intDictionary_ = 15;
//    else if(dictionaryName == "DICT_ARUCO_ORIGINAL") intDictionary_ = 16;
//    else if(dictionaryName == "DICT_APRILTAG_16h5") intDictionary_ = 17;
//    else if(dictionaryName == "DICT_APRILTAG_25h9") intDictionary_ = 18;
//    else if(dictionaryName == "DICT_APRILTAG_36h10") intDictionary_ = 19;
//    else if(dictionaryName == "NULL") intDictionary_ = -1;
}
