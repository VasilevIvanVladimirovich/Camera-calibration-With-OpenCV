#include "mainwindow.h"
#include "dialogsetimg.h"
#include "ui_mainwindow.h"

#define NUMBER_CAM 1


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->debugLine->setReadOnly(true);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"Include"<<"Status"<<"File name");
    ui->tableWidget->setShowGrid(false);
    // Разрешаем выделение только одного элемента
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    // Разрешаем выделение построчно
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

//    imgprocessor_ = new ImageProcessor(NUMBER_CAM);
//    connect(imgprocessor_,
//            SIGNAL(outDisplay(QPixmap)),
//            ui->widget_img,
//            SLOT(setPixmap(QPixmap)));

//    connect(imgprocessor_,
//            SIGNAL(finished()),
//            imgprocessor_,
//            SLOT(deleteLater()));

//    imgprocessor_->start();


    connect(&fileSystem_,
            SIGNAL(outTableItems(QTableWidgetItem*,QTableWidgetItem*)),
            this,
            SLOT(addItem(QTableWidgetItem*,QTableWidgetItem*)));
    connect(&fileSystem_,
            SIGNAL(outImgDisplay(QPixmap)),
            ui->widget_img,
            SLOT(setPixmap(QPixmap)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addItem(QTableWidgetItem *Item1,QTableWidgetItem *Item2)
{
    ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,Item1);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,2,Item2);
}

//void MainWindow::on_btn_calibration_clicked()
//{
//    calibprocessor_.calibrationChessboardMethod(imgprocessor_->getOutFrame());
//}


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
    dialog.setModal(true);
    dialog.exec();
}

void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    fileSystem_.openFileInView(ui->tableWidget->item(row,2)->text());
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

    dialog.setModal(true);
    dialog.exec();
}

