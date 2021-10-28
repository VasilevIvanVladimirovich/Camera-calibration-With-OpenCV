#include "mainwindow.h"
#include "dialogsetimg.h"
#include "ui_mainwindow.h"

#define NUMBER_CAM 1


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->calibOutputTextEdit->setReadOnly(true);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"Draw"<<"Status"<<"File name");
    ui->tableWidget->setShowGrid(false);
    // Разрешаем выделение только одного элемента
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    // Разрешаем выделение построчно
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);


    connect(&calibprocessor_,
            SIGNAL(sendStatusImg(QString,int)),
            this,
            SLOT(setStatusImg(QString,int)));
    connect(&fileSystem_,
            SIGNAL(outTableItems(QTableWidgetItem*,QTableWidgetItem*)),
            this,
            SLOT(addItem(QTableWidgetItem*,QTableWidgetItem*)));
    connect(&fileSystem_,
            SIGNAL(outImgDisplay(QPixmap)),
            ui->widget_img,
            SLOT(setPixmap(QPixmap)));
    connect(&fileSystem_,
            SIGNAL(outTextDisplayYamlCalib(QString)),
            ui->calibOutputTextEdit,
            SLOT(setText(QString)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

QVector<QString> MainWindow::getVectorImgFromTable()
{
    QVector<QString> vector;
    for(int i = 0;i < ui->tableWidget->rowCount();i++)
    {
        vector.push_back(ui->tableWidget->item(i,2)->text());
    }
    return vector;
}

void MainWindow::videoStream(int countframe)
{
        imgprocessor_ = new ImageProcessor(NUMBER_CAM);
        connect(imgprocessor_,
                SIGNAL(outDisplay(QPixmap)),
                ui->widget_img,
                SLOT(setPixmap(QPixmap)));

        connect(imgprocessor_,
                SIGNAL(finished()),
                imgprocessor_,
                SLOT(deleteLater()));

        connect(imgprocessor_,
                SIGNAL(setItem(QTableWidgetItem*,QTableWidgetItem*)),
                this,
                SLOT(addItem(QTableWidgetItem*,QTableWidgetItem*)));

        imgprocessor_->setPath(fileSystem_.getFilePath());
        imgprocessor_->setCountFrame(countframe);
        imgprocessor_->setTransformImg(false);
        imgprocessor_->start();

}

void MainWindow::on_btn_stopVideo_clicked()
{
    imgprocessor_->stopedThread();
}

void MainWindow::addItem(QTableWidgetItem *Item1,QTableWidgetItem *Item2)
{
    ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,Item1);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,2,Item2);
}


void MainWindow::setStatusImg(QString status, int row)
{
    QTableWidgetItem *st = new QTableWidgetItem(status);
    ui->tableWidget->setItem(row,1,st);
}


void MainWindow::on_btn_setImg_clicked()
{

    ui->tableWidget->clearContents();
    DialogSetImg dialog;
    connect(&dialog,
            SIGNAL(outFolderPath(QString)),
            &fileSystem_,
            SLOT(setPath(QString)));
    connect(&dialog,
            SIGNAL(setTableItems()),
            &fileSystem_,
            SLOT(getTableItems()));
    connect(&dialog,
            SIGNAL(signalVideoStream(int)),
            this,
            SLOT(videoStream(int)));
    dialog.setModal(true);
    dialog.exec();
}

void MainWindow::on_tableWidget_cellClicked(int row, int column)
{

    if(ui->tableWidget->item(row,0)->checkState() == Qt::Checked)
    {
          fileSystem_.openFileInView(row);
    }else fileSystem_.openFileInView(ui->tableWidget->item(row,2)->text());
}


void MainWindow::on_btn_detect_clicked()
{
    DialogDetect dialog;
    connect(&dialog,
            SIGNAL(outTargetType(QString)),
            &calibprocessor_,
            SLOT(setTargetType(QString)));
    connect(&dialog,
            SIGNAL(outTargetSize(int,int)),
            &calibprocessor_,
            SLOT(setTargetSize(int,int)));
    connect(&dialog,
            SIGNAL(outSubPixIter(int)),
            &calibprocessor_,
            SLOT(setSubPixIter(int)));  
    calibprocessor_.setVectorPathImg(getVectorImgFromTable());
    dialog.setModal(true);
    dialog.exec();
    calibprocessor_.accumulationVectorsImg();
    QString output;
    output = "Calib errors: " + QString::number(calibprocessor_.computeReprojectionErrors());
    ui->texteditErrorsCalib->setText(output);
    fileSystem_.openFileInViewYamlCalib("D:/PRoG/Git-repos/Camera-calibration-With-OpenCV/Temp/Result.YAML");
}
void MainWindow::on_chekResultInVideoStream_clicked()
{

    QString filename;
    filename = QFileDialog::getOpenFileName(0, "Открыть", "", "*.YAML");
    videoStream(filename);
}

void MainWindow::videoStream(QString qstring)
{
    imgprocessor_ = new ImageProcessor(NUMBER_CAM);
    connect(imgprocessor_,
            SIGNAL(outDisplay(QPixmap)),
            ui->widget_img,
            SLOT(setPixmap(QPixmap)));

    connect(imgprocessor_,
            SIGNAL(finished()),
            imgprocessor_,
            SLOT(deleteLater()));

    imgprocessor_->setPath(qstring);
    imgprocessor_->setTransformImg(true);
    imgprocessor_->start();
}

