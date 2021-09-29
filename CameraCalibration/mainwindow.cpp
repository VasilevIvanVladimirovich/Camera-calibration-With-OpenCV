#include "mainwindow.h"
#include "ui_mainwindow.h"



#define NUMBER_CAM 1

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->debugLine->setReadOnly(true);

    webcam.open(NUMBER_CAM);
    if(webcam.isOpened()==false)
    {
        ui->debugLine->setText("Error: webcam not accessed successfully");
        return;
    }
    tmrTimer = new QTimer(this);
    connect(tmrTimer, SIGNAL(timeout()),this,SLOT(processFrameAndUpdateGUI()));
    tmrTimer->start(20);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::processFrameAndUpdateGUI()
{
    webcam.read(matimg);

    QImage imgcam((uchar*)matimg.data,matimg.cols,matimg.rows,matimg.step,QImage::Format_RGB888);
    ui->widget_camera->setPixmap(QPixmap::fromImage(imgcam));
}

void MainWindow::on_btn_calibration_clicked()
{
    cv::Mat imgsave;
    imgsave=matimg;


    int CHECKERBOARD[2]{6,9}; //размер шахматной доски

    std::vector<std::vector<cv::Point3f> > objpoints; //Вектор, для хранения векторов 3d точек для каждого изображения шахматной доски
    std::vector<std::vector<cv::Point2f> > imgpoints; //Вектор, для хранения векторов 2d точек для каждого изображения шахматной доски

    //Определение мировых координат для 3d точке
    std::vector<cv::Point3f> objp;
    for(int i{0};i<CHECKERBOARD[1];i++)
    {
        for(int j{0}; j<CHECKERBOARD[0]; j++)
        objp.push_back(cv::Point3f(j,i,0));
    }

    std::vector<cv::Point2f> corner_pts; // Вектор для хранения пикселей координат углов шахматной доски
    bool success;

    //Поиск углов шахматной доски
    //Если на изображении найдено нужное количество углов, то успех = истина
    success = cv::findChessboardCorners(imgsave,cv::Size(CHECKERBOARD[0], CHECKERBOARD[1]),corner_pts);
    if(success)
    {
        ui->debugLine->setText("Success");
        cv::drawChessboardCorners(imgsave, cv::Size(CHECKERBOARD[0], CHECKERBOARD[1]), corner_pts, success);
        objpoints.push_back(objp);
        imgpoints.push_back(corner_pts);
        cv::Mat cameraMatrix,distCoeffs,R,T;

        cv::calibrateCamera(objpoints, imgpoints, cv::Size(imgsave.rows,imgsave.cols), cameraMatrix, distCoeffs, R, T);

        QImage imgcam((uchar*)imgsave.data,imgsave.cols,imgsave.rows,imgsave.step,QImage::Format_RGB888);
        ui->labelDebug->setPixmap(QPixmap::fromImage(imgcam));

        std::cout << "cameraMatrix : " << cameraMatrix << std::endl;
        std::cout << "distCoeffs : " << distCoeffs << std::endl;
        std::cout << "Rotation vector : " << R << std::endl;
        std::cout << "Translation vector : " << T << std::endl;
    }else ui->debugLine->setText("Error");


}

