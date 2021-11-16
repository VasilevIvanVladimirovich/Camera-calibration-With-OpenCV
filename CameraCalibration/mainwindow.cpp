#include "mainwindow.h"
#include "dialogsetimg.h"
#include "ui_mainwindow.h"

#define NUMBER_CAM 1

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QAction *newa = new QAction("&New", this);
    QAction *open = new QAction("&Open", this);

    QMenu *file;
    file = menuBar()->addMenu(tr("&File"));
    file->addAction(newa);
    file->addAction(open);
    connect(newa,
            SIGNAL(triggered()),
            this,
            SLOT(setqToolBarNew()));

    connect(open,
            SIGNAL(triggered()),
            this,
            SLOT(setqToolBarOpen()));

    ui->calibOutputTextEdit->setReadOnly(true);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"Draw"<<"Status"<<"File name");
    ui->tableWidget->setShowGrid(false);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);    // Разрешаем выделение только одного элемента
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);    // Разрешаем выделение построчно
    ui->labelFileOpen->setText(tr("Dir is not open"));

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
        vector.push_back(ui->tableWidget->item(i,2)->text());
    return vector;
}

void MainWindow::videoStream(int frameRate,int countframe,int row,
                             int col,bool isCheked,QString pattern)
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

    imgprocessor_->setPath(fileSystem_.getFilePath()+"/");
    imgprocessor_->setFrameRate(frameRate);
    imgprocessor_->setCountFrame(countframe);
    imgprocessor_->setIsPattern(isCheked);
    imgprocessor_->setPattern(pattern);
    imgprocessor_->setCheckboardstate(row,col);
    imgprocessor_->setTransformImg(false);
    imgprocessor_->start();

}

void MainWindow::on_btn_stopVideo_clicked()
{
    imgprocessor_->stopedThread();
}

void MainWindow::addItem(QTableWidgetItem *Item1, QTableWidgetItem *Item2)
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
    if(fileSystem_.isSelectedDir())
    {
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);
        DialogSetImg dialog;

        dialog.setFileSystem(&fileSystem_);

        connect(&dialog,
                SIGNAL(signalVideoStream(int,int,int,int,bool,QString)),
                this,
                SLOT(videoStream(int,int,int,int,bool,QString)));

        dialog.setModal(true);
        dialog.exec();
    }
    else
        QMessageBox::warning(this, tr("Warning"),tr("File is not open"));
}

void MainWindow::on_tableWidget_cellClicked(int row, int column)
{

    if(ui->tableWidget->item(row,0)->checkState() == Qt::Checked){
        fileSystem_.openDrawImgInView(row);
    }else
        fileSystem_.openFileInView(ui->tableWidget->item(row,2)->text());
}

void MainWindow::on_btn_detect_clicked()
{
    if(fileSystem_.isSelectedDir())
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
        calibprocessor_.setPath(fileSystem_.getFilePath());
        calibprocessor_.accumulationVectorsImg();
        fileSystem_.openFileInViewYamlCalib();
    }else
        QMessageBox::warning(this, tr("Warning"),tr("File is not open"));
}

void MainWindow::on_chekResultInVideoStream_clicked()
{

    QString filename;
    filename = QFileDialog::getOpenFileName(0, "Open", "", "*.YAML");
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

void MainWindow::setqToolBarNew()
{
    QString pathName;
    pathName = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "/home",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    fileSystem_.setPath(pathName+"/");
    fileSystem_.createWorkDir();
    ui->labelFileOpen->setText(pathName);
}

void MainWindow::setqToolBarOpen()
{
    QString pathName;
    pathName = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "/home",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);

    fileSystem_.setPath(pathName+"/");
    bool isValidPath;
    isValidPath = fileSystem_.isValidOpenDir();
    if(isValidPath)
    {
        ui->labelFileOpen->setText(pathName);
        fileSystem_.getTableItems();
        fileSystem_.openFileInViewYamlCalib();
    }else
        QMessageBox::warning(this, tr("Warning"),tr("The folder does not meet the standard"));

}

void MainWindow::on_OpenTable_clicked()
{
    tableCompare_ = new TableCompare();
    tableCompare_->show();
}

