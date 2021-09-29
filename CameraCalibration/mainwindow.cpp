#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include <QTimer>

#define NUMBER_CAM 1

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{

    ui->setupUi(this);

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

}

