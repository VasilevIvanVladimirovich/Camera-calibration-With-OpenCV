#include "mainwindow.h"
#include "ui_mainwindow.h"

#define NUMBER_CAM 1
#define FILE_RESULT_YAML "D:/PRoG/Git-repos/Camera-calibration-With-OpenCV/CameraCalibration/Save/SaveResult.YAML"
#define FILE_RESILT_IMG "D:/PRoG/Git-repos/Camera-calibration-With-OpenCV/CameraCalibration/Save/Img/"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->debugLine->setReadOnly(true);



//    processor = new VideoProcessor();

//    processor->moveToThread(new QThread(this));

//    connect(processor->thread(),
//            SIGNAL(started()),
//            processor,
//            SLOT(startVideo()));
//    connect(processor->thread(),
//            SIGNAL(finished()),
//            processor,
//            SLOT(deleteLater()));
//    connect(processor,
//            SIGNAL(outDisplay(QPixmap)),
//            ui->widget_camera,
//            SLOT(setPixmap(QPixmap)));
//     processor->thread()->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_calibration_clicked()
{
    cv::Mat imgsave = imgprocessor->getOutFrame();

//        QImage imgcam((uchar*)imgsave.data,imgsave.cols,imgsave.rows,imgsave.step,QImage::Format_RGB888); //Проверка изображения
//        ui->widget_camera->setPixmap(QPixmap::fromImage(imgcam));


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

//        QImage imgcam((uchar*)imgsave.data,imgsave.cols,imgsave.rows,imgsave.step,QImage::Format_RGB888);
//        ui->labelDebug->setPixmap(QPixmap::fromImage(imgcam));

        std::cout << "cameraMatrix : " << cameraMatrix << std::endl;
        std::cout << "distCoeffs : " << distCoeffs << std::endl;
        std::cout << "Rotation vector : " << R << std::endl;
        std::cout << "Translation vector : " << T << std::endl;

        //Сохранение изображения
        if(cv::imwrite((std::string)FILE_RESILT_IMG + "test.jpg", imgsave) == true)
        {
            ui->debugLine->setText("Save");
        }

        //Сохранение результатов в файл
        std::string fileResultPath = FILE_RESULT_YAML;
        cv::FileStorage fs(fileResultPath,cv::FileStorage::WRITE);
        fs << "cameraMatrix" << cameraMatrix
           << "distCoeffs"   << distCoeffs;
        fs.release();

    }else ui->debugLine->setText("Error");


}

//кнопка переключения ввода изображения (веб камера или файл)
void MainWindow::on_btn_input_clicked()
{
    if(ui->radioButton_file->isChecked())
    {
        if(!(imgprocessor == nullptr))
        {
            //если в указатель указывает на что-то, то его обнуляем, что бы обновить
            imgprocessor->stopedThread();
            imgprocessor = nullptr;
        }
        QString fileName = QFileDialog::getOpenFileName(this,
                                    QString::fromUtf8("Открыть файл"),
                                    QDir::currentPath(),
                                    "Images (*.png *.xpm *.jpg);;All files (*.*)");

        cv::Mat mt = cv::imread(fileName.toStdString());

        imgprocessor = new ImageProcessor(mt);
        connect(imgprocessor,
               SIGNAL(outDisplay(QPixmap)),
               ui->widget_camera,
               SLOT(setPixmap(QPixmap)));


        QImage imgcam((uchar*)mt.data,mt.cols,mt.rows,mt.step,QImage::Format_RGB888); //Проверка изображения
        ui->widget_camera->setPixmap(QPixmap::fromImage(imgcam));

    }
    if(ui->radioButton_web->isChecked())
    {
        if(!(imgprocessor == nullptr))
        {
            //если в указатель указывает на что-то, то его обнуляем, что бы обновить
            imgprocessor->stopedThread();
            imgprocessor = nullptr;
        }

        imgprocessor = new ImageProcessor(NUMBER_CAM);

        connect(imgprocessor,
                SIGNAL(outDisplay(QPixmap)),
                ui->widget_camera,
                SLOT(setPixmap(QPixmap)));

        connect(imgprocessor,
                SIGNAL(finished()),
                imgprocessor,
                SLOT(deleteLater()));


        imgprocessor->start();
    }
}

