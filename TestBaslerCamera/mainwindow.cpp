#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    isStart = false;
    capturer = new CaptureThread();
    connect(capturer,&CaptureThread::outputImg,this,&MainWindow::updateFrameLabel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_2_clicked()
{
    capturer->start();
}


void MainWindow::on_pushButton_clicked()
{
    ui->label->setText("Текущая экспозиция:" + QString::number(capturer->getExposureTime()));
}


void MainWindow::updateFrameLabel(QImage img)
{
    QPixmap pix = QPixmap::fromImage(img);
    ui->label_3->setPixmap(pix);
}


void MainWindow::on_pushButton_4_clicked()
{
    ui->label_2->setText("Текущий  Gain " + QString::number(capturer->getGain()));
}


void MainWindow::on_pushButton_5_clicked()
{
    capturer->setSetting(ui->spinBox->value(),ui->spinBox_2->value());
}

