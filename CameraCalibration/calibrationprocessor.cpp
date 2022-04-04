#include "calibrationprocessor.h"

CalibrationProcessor::CalibrationProcessor(FileSystem* fs)
{
    fs_ = fs;
}
CalibrationProcessor::CalibrationProcessor()
{
}


void CalibrationProcessor::run()
{
    targetType_ = fs_->getPattern();
    CHECKERBOARD_[0] = fs_->getRow();
    CHECKERBOARD_[1] = fs_->getCol();
    subPixelIter_ = fs_->getSubPixIter();

    if(fs_->getIndexCameraFirst()!=-1 && fs_->isCalibration()==1)
    {
        reloadVectors();
        numCam_=1;
        vectorPathImg_= fs_->getVectorPath(1);
        singleCalibration();
    }
    if(fs_->getIndexCameraSecond()!=-1 && fs_->isCalibration()==1)
    {
        reloadVectors();
        numCam_ = 2;
        vectorPathImg_= fs_->getVectorPath(2);
        singleCalibration();
    }
    if(fs_->getIndexCameraFirst()!=-1 && fs_->getIndexCameraSecond()!=-1 && fs_->isStereoCalibration() == 1)
    {
        reloadVectors();
        stereoCalibration();
    }
}

void CalibrationProcessor::cameraCalibration()
{

    if(targetType_ == "Chessboard" || targetType_ == "Circles" || targetType_ == "Assymetric Circles")
    {
        cv::calibrateCamera(objpoints_, imgpoints_, cv::Size(gray.rows, gray.cols),
                            cameraMatrix_, distCoeffs_, R_, T_, calibrationFlags_);
        rmse_ = Rmse();
    }
    if(targetType_ == "ChArUco")
    {
        rmse_ = cv::aruco::calibrateCameraCharuco(allCharucoCorners, allCharucoIds,
                                                  charucoboard_, imgSizeCharuco_, cameraMatrix_, distCoeffs_,
                                                  R_, T_, calibrationFlags_);
    }

    fs_->createImgUndistorted(cameraMatrix_, distCoeffs_, numCam_);

    QDateTime date;
    date = QDateTime::currentDateTime();
    int countImg;
    countImg = vectorPathImg_.length();

    std::string sizePatern;
    if(targetType_ == "Chessboard" || targetType_ == "Circles" || targetType_ == "Assymetric Circles")
        sizePatern = std::to_string(CHECKERBOARD_[0]) + " x " + std::to_string(CHECKERBOARD_[1]);
    else if(targetType_ == "ChArUco")
        sizePatern = std::to_string(CHECKERBOARD_[0]) + " x " + std::to_string(CHECKERBOARD_[1]) +
                "\n" + "MarkerSize:" + std::to_string(markerSize_) + "\n" + "CheckerSize:" + std::to_string(checkerSize_) +
                "\n" + dictionaryName_.toStdString();

    fs_->saveFileInYaml(objpoints_, imgpoints_, cameraMatrix_, distCoeffs_, R_, T_, countImg, date.toString("yyyy.dd.M--HH:mm:ss"),
                        rmse_, targetType_, sizePatern, numCam_,cv::Size(gray.rows, gray.cols));
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

void CalibrationProcessor::singleCalibration()
{
    QString filename;

    std::vector<cv::Point3f> objp;
    for(int i{0};i<CHECKERBOARD_[1];i++)
    {
        for(int j{0}; j<CHECKERBOARD_[0]; j++)
            objp.push_back(cv::Point3f(j,i,0));
    }
    std::vector<cv::Point2f> corner_pts; // Вектор для хранения пикселей координат углов шахматной доски

    if(targetType_ == "ChArUco")
    {
        dictionary_ = cv::aruco::getPredefinedDictionary(intDictionary_);
        charucoboard_ = cv::aruco::CharucoBoard::create(CHECKERBOARD_[1], CHECKERBOARD_[0], checkerSize_, markerSize_, dictionary_);
        params_ = cv::aruco::DetectorParameters::create();
    }

    bool isSuccess = false;
    for(int i = 0;i<vectorPathImg_.length();i++)
    {
        inputFrame_ = cv::imread(vectorPathImg_[i].toStdString());
        cv::cvtColor(inputFrame_,gray,cv::COLOR_BGR2GRAY);

        if(targetType_ == "ChArUco")
            charucoAccumulation(i);
        if(targetType_ == "Chessboard")
            chessboardAccumulation(i, isSuccess, objp, corner_pts);
        if(targetType_ == "Circles")
            circleAccumulation(i, isSuccess, objp, corner_pts);
        if(targetType_ == "Assymetric Circles")
            aCircleAccumulation(i, isSuccess, objp, corner_pts);
    }
    if(targetType_ == "ChArUco")
            imgSizeCharuco_ = inputFrame_.size();
    cameraCalibration();
    //emit sendOpenFileInViewYamlCalib(fs_->getFilePath());
}

void CalibrationProcessor::charucoAccumulation(int i)
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
       fs_->saveInImgDrawing(saveImg, filename,numCam_);
    } //emit sendStatusImg("No find corners", i);

}

void CalibrationProcessor::chessboardAccumulation(int i, bool isSuccess, std::vector<cv::Point3f> objp, std::vector<cv::Point2f> corner_pts)
{
    QString filename;
    isSuccess = cv::findChessboardCorners(gray,cv::Size(CHECKERBOARD_[0], CHECKERBOARD_[1]), corner_pts);
    if(isSuccess)
    {
        cv::TermCriteria criteria(cv::TermCriteria::EPS | cv::TermCriteria::MAX_ITER, 30, 0.001);
        cv::cornerSubPix(gray,corner_pts, cv::Size(11,11), cv::Size(-1,-1), criteria);

        cv::drawChessboardCorners(inputFrame_, cv::Size(CHECKERBOARD_[0], CHECKERBOARD_[1]), corner_pts, isSuccess);
        objpoints_.push_back(objp);
        imgpoints_.push_back(corner_pts);

        //emit sendStatusImg("Success", i);

        QPixmap saveImg = QPixmap::fromImage(
                          QImage(inputFrame_.data,
                                  inputFrame_.cols,
                                  inputFrame_.rows,
                                  inputFrame_.step,
                         QImage::Format_RGB888).rgbSwapped());

       filename = QString::number(i + 1) + ".png";
       fs_->saveInImgDrawing(saveImg, filename,numCam_);
    }//else //emit sendStatusImg("No find corners", i);
}

void CalibrationProcessor::circleAccumulation(int i, bool isSuccess, std::vector<cv::Point3f> objp, std::vector<cv::Point2f> corner_pts)
{
    QString filename;
    isSuccess = cv::findCirclesGrid(gray, cv::Size(CHECKERBOARD_[0], CHECKERBOARD_[1]), corner_pts,
                cv::CALIB_CB_SYMMETRIC_GRID, cv::SimpleBlobDetector::create());
    if(isSuccess)
    {
        cv::drawChessboardCorners(inputFrame_, cv::Size(CHECKERBOARD_[0], CHECKERBOARD_[1]), corner_pts, isSuccess);
        objpoints_.push_back(objp);
        imgpoints_.push_back(corner_pts);

        //emit sendStatusImg("Success", i);

        QPixmap saveImg = QPixmap::fromImage(
                          QImage(inputFrame_.data,
                                  inputFrame_.cols,
                                  inputFrame_.rows,
                                  inputFrame_.step,
                         QImage::Format_RGB888).rgbSwapped());

       filename = QString::number(i + 1) + ".png";
       fs_->saveInImgDrawing(saveImg, filename, numCam_);
    }//else //emit sendStatusImg("No find corners", i);
}

void CalibrationProcessor::aCircleAccumulation(int i, bool isSuccess, std::vector<cv::Point3f> objp, std::vector<cv::Point2f> corner_pts)
{
    QString filename;
    isSuccess = cv::findCirclesGrid(gray, cv::Size(CHECKERBOARD_[0], CHECKERBOARD_[1]), corner_pts,
                cv::CALIB_CB_ASYMMETRIC_GRID, cv::SimpleBlobDetector::create());
    if(isSuccess)
    {
        cv::drawChessboardCorners(inputFrame_, cv::Size(CHECKERBOARD_[0], CHECKERBOARD_[1]), corner_pts, isSuccess);
        objpoints_.push_back(objp);
        imgpoints_.push_back(corner_pts);

        //emit sendStatusImg("Success", i);

        QPixmap saveImg = QPixmap::fromImage(
                          QImage(inputFrame_.data,
                                  inputFrame_.cols,
                                  inputFrame_.rows,
                                  inputFrame_.step,
                         QImage::Format_RGB888).rgbSwapped());

       filename = QString::number(i + 1) + ".png";
       fs_->saveInImgDrawing(saveImg, filename,numCam_);
    }//else //emit sendStatusImg("No find corners", i);
}

void CalibrationProcessor::reloadVectors()
{
    objpoints_.clear();
    imgpoints_.clear();
    imgpoints_.clear();
    allCharucoCorners.clear();
    allCharucoIds.clear();
    allImgs.clear();
    R_.clear();
    T_.clear();
    vectorPathImg_.clear();
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

void CalibrationProcessor::setSubPixelIter(int iter)
{
    subPixelIter_ = iter;
}

void CalibrationProcessor::setCalibrationFlags(int calibrationFlags)
{
    calibrationFlags_ = calibrationFlags;
}

double CalibrationProcessor::Rmse()
{

    std::vector<cv::Point2f> imagePoints2;
    double totalErr = 0;
    double err;
    size_t totalPoints=0;
    for(size_t i = 0; i < objpoints_.size(); ++i )
    {
         projectPoints(objpoints_[i], R_[i], T_[i], cameraMatrix_, distCoeffs_, imagePoints2);
         err = norm(imgpoints_[i], imagePoints2, cv::NORM_L2);
         size_t n = objpoints_[i].size();
         totalErr        += err*err;
         totalPoints     += n;
    }
    return std::sqrt(totalErr/totalPoints);
}


bool CalibrationProcessor::isFramePattern(cv::Mat* frame, QString pattern, int row, int col,
                                          double icheckerSize, double imarkerSize, int idictionary)
{
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
