#include "mainwindow.h"
#include "ui_mainwindow.h"

#define NUMBER_CAM 1


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->debugLine->setReadOnly(true);

    imgprocessor_ = new ImageProcessor(NUMBER_CAM);
    connect(imgprocessor_,
            SIGNAL(outDisplay(QPixmap)),
            ui->widget_camera,
            SLOT(setPixmap(QPixmap)));

    connect(imgprocessor_,
            SIGNAL(finished()),
            imgprocessor_,
            SLOT(deleteLater()));

    imgprocessor_->start();

    connect(&fileSystem_,
            SIGNAL(outImgDisplay(QPixmap)),
            ui->viewfile,
            SLOT(setPixmap(QPixmap)));

    connect(&fileSystem_,
            SIGNAL(outTextDisplay(QPixmap)),
            ui->textEditViewFile,
            SLOT(setText(QString)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_calibration_clicked()
{
    calibprocessor_ = new CalibrationProcessor(imgprocessor_->getOutFrame());
    calibprocessor_->calibrationChessboardMethod();


//        QImage imgcam((uchar*)imgsave.data,imgsave.cols,imgsave.rows,imgsave.step,QImage::Format_RGB888); //Проверка изображения
//        ui->widget_camera->setPixmap(QPixmap::fromImage(imgcam));


}


void MainWindow::on_openFile_clicked()
{
    QString pathName = QFileDialog::getOpenFileName(this,
                                QString::fromUtf8("Открыть файл"),
                                QDir::currentPath(),
                                "Images (*.png *.jpg *.bmp *.YAML)");

    fileSystem_.openFileInView(pathName);

}

