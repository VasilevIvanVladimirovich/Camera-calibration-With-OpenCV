#include "mainwindow.h"
#include "dialogsetimg.h"
#include "ui_mainwindow.h"

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
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"Draw"<<"Undist"<<"Status"<<"File name");
    ui->tableWidget->setShowGrid(false);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);    // Разрешаем выделение только одного элемента
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);    // Разрешаем выделение построчно
    ui->labelFileOpen->setText(tr("Dir is not open"));

    connect(&fileSystem_,
            SIGNAL(outTableItems(QTableWidgetItem*, QTableWidgetItem*, QTableWidgetItem*)),
            this,
            SLOT(addItem(QTableWidgetItem*, QTableWidgetItem*, QTableWidgetItem*)));
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
        vector.push_back(ui->tableWidget->item(i,3)->text());
    return vector;
}

void MainWindow::videoStream(int numCum, int frameRate, int countframe,bool isSnapShoot)
{
    imgprocessor_ = new ImageProcessor(numCum);

    connect(imgprocessor_,
        SIGNAL(outDisplay(QPixmap)),
        ui->widget_img,
        SLOT(setPixmap(QPixmap)));

    connect(imgprocessor_,
        SIGNAL(finished()),
        imgprocessor_,
        SLOT(deleteLater()));

    connect(imgprocessor_,
        SIGNAL(setItem(QTableWidgetItem*, QTableWidgetItem*, QTableWidgetItem*)),
        this,
        SLOT(addItem(QTableWidgetItem*, QTableWidgetItem*, QTableWidgetItem*)));

    imgprocessor_->setPath(fileSystem_.getFilePath()+"/");
    imgprocessor_->setFrameRate(frameRate);
    imgprocessor_->setCountFrame(countframe);
    imgprocessor_->setPattern(fileSystem_.getPattern());
    imgprocessor_->setCheckboardstate(fileSystem_.getRow(), fileSystem_.getCol());
    imgprocessor_->setCheckerSize(fileSystem_.getCheckerSize());
    imgprocessor_->setMarkerSize(fileSystem_.getMarkerSize());
    imgprocessor_->setIsSnapShoot(isSnapShoot);
    imgprocessor_->setDictionaryName(fileSystem_.getDictionaryName());


    imgprocessor_->setTransformImg(false);
    imgprocessor_->start();
}

void MainWindow::on_btn_stopVideo_clicked()
{
    imgprocessor_->stopedThread();
}

void MainWindow::addItem(QTableWidgetItem *Item, QTableWidgetItem *Item1, QTableWidgetItem *Item2)
{
    ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,Item);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,1,Item1);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,3,Item2);
}

void MainWindow::setStatusImg(QString status, int row)
{
    QTableWidgetItem *st = new QTableWidgetItem(status);
    ui->tableWidget->setItem(row,2,st);
}

void MainWindow::runCalib()
{
    calibprocessor_->setPattern(fileSystem_.getPattern());
    calibprocessor_->setTargetSize(fileSystem_.getRow(), fileSystem_.getCol(),
                                   fileSystem_.getMarkerSize(), fileSystem_.getCheckerSize(),
                                   fileSystem_.getDictionaryName());
    calibprocessor_->setSubPixelIter(fileSystem_.getSubPixIter());
    calibprocessor_->setCalibrationFlags(fileSystem_.getCalibFlags());
    calibprocessor_->run();
}

void MainWindow::setPath(QString path)
{
    fileSystem_.setPath(path);
    ui->labelFileOpen->setText(path);
}

void MainWindow::on_btn_setImg_clicked()
{
    if(fileSystem_.isSelectedDir())
    {
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);

        WindowImportImage = new DialogWindowImportImage();

        WindowImportImage->setFileSystem(&fileSystem_);

        connect(WindowImportImage,
                SIGNAL(signalVideoStream(int,int,int,bool)),
                this,
                SLOT(videoStream(int,int,int,bool)));

       WindowImportImage->show();
    }
   else
        QMessageBox::warning(this, tr("Warning"),tr("File is not open"));
}

void MainWindow::on_tableWidget_cellClicked(int row)
{
    if(ui->tableWidget->item(row, 0)->checkState() == Qt::Checked){
        fileSystem_.openDrawImgInView(row);
    }else if(ui->tableWidget->item(row, 1)->checkState() == Qt::Checked)
            fileSystem_.openUndistImgInView(row);
        else
            fileSystem_.openFileInView(ui->tableWidget->item(row, 3)->text());
}

void MainWindow::on_btn_detect_clicked()
{

    if(fileSystem_.isSelectedDir())
    {
        calibprocessor_ = new CalibrationProcessor();

        WindowDetectCalibration = new DialogWindowDetectCalibration();

        connect(calibprocessor_,
            SIGNAL(finished()),
            calibprocessor_,
            SLOT(deleteLater()));

        connect(WindowDetectCalibration,
                SIGNAL(goCalib()),
                this,
                SLOT(runCalib()));

        connect(calibprocessor_,
                SIGNAL(sendStatusImg(QString,int)),
                this,
                SLOT(setStatusImg(QString,int)));

        connect(calibprocessor_,
                SIGNAL(sendOpenFileInViewYamlCalib(QString)),
                &fileSystem_,
                SLOT(openFileInViewYamlCalib(QString)));

        calibprocessor_->setVectorPathImg(getVectorImgFromTable());
        calibprocessor_->setPath(fileSystem_.getFilePath());


        WindowDetectCalibration->setFileSystem(&fileSystem_);
        WindowDetectCalibration->show();
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
    imgprocessor_ = new ImageProcessor(1);

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
//    QString pathName;
//    pathName = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
//                                                 "/home",
//                                                 QFileDialog::ShowDirsOnly
//                                                 | QFileDialog::DontResolveSymlinks);
//    fileSystem_.setPath(pathName+"/");
//    fileSystem_.createWorkDir();
//    ui->labelFileOpen->setText(pathName);

    windowNewFile = new MenuWindowNewFile();
    connect(windowNewFile, SIGNAL(setPathDir(QString)),
            this, SLOT(setPath(QString)));

    connect(windowNewFile, SIGNAL(setParamentCalib(QString, int, int,
                                                   double,double, QString)),
            &fileSystem_, SLOT(writeSettingCalibInYaml(QString, int, int,
                                                      double,double, QString)));

    windowNewFile->show();

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
        QFileInfo check_file(pathName + "Accumulated");
        if(check_file.exists())
            fileSystem_.openFileInViewYamlCalib(fileSystem_.getFilePath());
    }else
        QMessageBox::warning(this, tr("Warning"),tr("The folder does not meet the standard"));
}

void MainWindow::on_OpenTable_clicked()
{
    tableCompare_ = new TableCompare();
    tableCompare_->show();
}


void MainWindow::on_btn_snapshoot_clicked()
{
    imgprocessor_->setIsPressSnap();
}

