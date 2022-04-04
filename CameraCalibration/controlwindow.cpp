#include "controlwindow.h"

ControlWindow::ControlWindow()
{
    initUi();
    initImageTable();
    createAction();
}

void ControlWindow::initUi()
{
   // setup menubar
   fileMenu = menuBar()->addMenu("&File");
   viewMenu = menuBar()->addMenu("&View");
   analysisMenu= menuBar()->addMenu("&Analysis");
   // setup toolbar
   fileToolBar = addToolBar("File");
   viewToolBar = addToolBar("View");
   analysisToolBar= addToolBar("Analysis");
   videoToolBar = addToolBar("Video");

   // setup status bar
   mainStatusBar = statusBar();
   mainStatusLabel = new QLabel("mainStatusBar");
   mainStatusBar->addPermanentWidget(mainStatusLabel);
   mainStatusLabel->setText("Dir is not open");

   imageSceneFirst = new QGraphicsScene(this);
   imageViewFirst = new QGraphicsView(imageSceneFirst);

   imageSceneSecond = new QGraphicsScene(this);
   imageViewSecond = new QGraphicsView(imageSceneSecond);

   QHBoxLayout* layout_main = new QHBoxLayout;

   imageTable = new QTableWidget();

   connect(&fileSystem_,
           SIGNAL(outTableItems(QTableWidgetItem *, QTableWidgetItem *, QTableWidgetItem *)),
           this,
           SLOT(addItem(QTableWidgetItem *, QTableWidgetItem *, QTableWidgetItem *)));

   layout_main->addWidget(imageTable);

   QVBoxLayout* layout_output = new QVBoxLayout;

   QHBoxLayout* layout_stream = new QHBoxLayout;
   layout_stream->addWidget(imageViewFirst);
   layout_stream->addWidget(imageViewSecond);

   layout_output->addLayout(layout_stream);

   calibOutputTextEdit = new QTextEdit();

   connect(&fileSystem_,
           SIGNAL(outImgDisplayFirst(QPixmap)),
           this,
           SLOT(updateFrameFirst(QPixmap)));
   connect(&fileSystem_,
           SIGNAL(outImgDisplaySecond(QPixmap)),
           this,
           SLOT(updateFrameSecond(QPixmap)));


   connect(&fileSystem_,
           SIGNAL(outTextDisplayYamlCalib(QString)),
           this,
           SLOT(setCalibOutputTextEdit(QString)));


   calibOutputTextEdit->setReadOnly(true);
   calibOutputTextEdit->setMaximumHeight(250);
   //layout_output->addWidget(calibOutputTextEdit);

   layout_main->addLayout(layout_output);

   QWidget *mainWidget = new QWidget();
   mainWidget->setLayout(layout_main);

   setCentralWidget(mainWidget);
}

void ControlWindow::initImageTable()
{
   imageTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
   imageTable->setMaximumWidth(400);
   imageTable->setColumnCount(4);
   imageTable->setHorizontalHeaderLabels(QStringList()<<"Draw"<<"Undist"<<"Status"<<"File name");
   imageTable->setShowGrid(false);
   imageTable->setSelectionMode(QAbstractItemView::SingleSelection);
   imageTable->setSelectionBehavior(QAbstractItemView::SelectRows);

   connect(imageTable,
           SIGNAL(cellClicked(int, int)),
           this,
           SLOT(on_imageTable_cellClicked(int,int)));
}

void ControlWindow::createAction()
{
    createProjectAction = new QAction("&Create", this);
    connect(createProjectAction, SIGNAL(triggered(bool)), this, SLOT(createProject()));
    fileMenu->addAction(createProjectAction);
    fileToolBar->addAction(createProjectAction);

    openProjectAction = new QAction("&Open", this);
    connect(openProjectAction, SIGNAL(triggered(bool)), this, SLOT(openProject()));
    fileMenu->addAction(openProjectAction);
    fileToolBar->addAction(openProjectAction);

    exitAction = new QAction("&Exit", this);
    connect(exitAction, SIGNAL(triggered(bool)), QApplication::instance(), SLOT(quit()));
    fileMenu->addAction(exitAction);

    importImageAction = new QAction("&Import image", this);
    connect(importImageAction, SIGNAL(triggered(bool)), this, SLOT(importImage()));
    viewMenu->addAction(importImageAction);
    viewToolBar->addAction(importImageAction);

    detectAction = new QAction("&Detect", this);
    connect(detectAction, SIGNAL(triggered(bool)), this, SLOT(detect()));
    viewMenu->addAction(detectAction);
    viewToolBar->addAction(detectAction);

    openCompareWindowAction = new QAction("&Compare Window", this);
    connect(openCompareWindowAction, SIGNAL(triggered(bool)), this, SLOT(openCompareWindow()));
    analysisMenu->addAction(openCompareWindowAction);
    analysisToolBar->addAction(openCompareWindowAction);

    streamAction = new QAction("&Stream Action", this);
    connect(streamAction, SIGNAL(triggered(bool)), this, SLOT(openSettingStream()));
    analysisMenu->addAction(streamAction);
    analysisToolBar->addAction(streamAction);

    undistortStreamAction = new QAction("&Undistort Stream", this);
    connect(undistortStreamAction, SIGNAL(triggered(bool)), this, SLOT(undistortStream()));
    videoToolBar->addAction(undistortStreamAction);

    saveImageAction = new QAction("&Save Image", this);
    connect(saveImageAction, SIGNAL(triggered(bool)), this, SLOT(saveImage()));
    videoToolBar->addAction(saveImageAction);
    saveImageAction->setEnabled(false);

    stopVideoAction = new QAction("&Stop Video", this);
    connect(stopVideoAction, SIGNAL(triggered(bool)), this, SLOT(stopVideo()));
    videoToolBar->addAction(stopVideoAction);
    stopVideoAction->setEnabled(false);
}

void ControlWindow::openProject()
{
    QString pathName;
    pathName = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "/home",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    fileSystem_.setPath(pathName+"/");
    mainStatusLabel->setText(pathName);
    fileSystem_.getTableItems();
//    bool isValidPath;
//    isValidPath = fileSystem_.isValidOpenDir();
//    if(isValidPath)
//    {
//        mainStatusLabel->setText(pathName);
//        fileSystem_.getTableItems();
//        fileSystem_.openFileInViewYamlCalib(fileSystem_.getFilePath());
//    }else
//        QMessageBox::warning(this, tr("Warning"),tr("The folder does not meet the standard"));
}

void ControlWindow::createProject()
{
    MenuWindowNewFile *windowNewFile = new MenuWindowNewFile();
    connect(windowNewFile, SIGNAL(setPathDir(QString)),
            this, SLOT(setPath(QString)));//text label path
    windowNewFile->show();
}

void ControlWindow::importImage()
{
    if(fileSystem_.isSelectedDir())
    {
        imageTable->clearContents();
        imageTable->setRowCount(0);

        WindowImportImage = new DialogWindowImportImage();
        WindowImportImage->setFileSystem(&fileSystem_);

        connect(WindowImportImage,
                SIGNAL(signalVideoStream(int,int,bool)),
                this,
                SLOT(videoStream(int,int,bool)));
       WindowImportImage->show();
    }
   else
        QMessageBox::warning(this, tr("Warning"),tr("File is not open"));
}

void ControlWindow::detect()
{

        WindowDetectCalibration = new DialogWindowDetectCalibration();
        WindowDetectCalibration->setFileSystem(&fileSystem_);
        WindowDetectCalibration->show();

        connect(WindowDetectCalibration,
                SIGNAL(goCalib()),
                this,
                SLOT(runCalibration()));

//        connect(calibprocessor_,
//                SIGNAL(sendStatusImg(QString,int)),
//                this,
//                SLOT(setStatusImg(QString,int)));

//        connect(calibprocessor_,
//                SIGNAL(sendOpenFileInViewYamlCalib(QString)),
//                &fileSystem_,
//                SLOT(openFileInViewYamlCalib(QString)));


}

void ControlWindow::openCompareWindow()
{
    TableCompare *tableCompare_ = new TableCompare();
    tableCompare_->show();
}

void ControlWindow::saveImage()
{
    imgprocessorFirst_->setIsPressSnap();
    imgprocessorSecond_->setIsPressSnap();
}

void ControlWindow::undistortStream()
{
//    QString filename;
//    filename = QFileDialog::getOpenFileName(0, "Open", "", "*.YAML");
//    videoStream(filename);
}

void ControlWindow::stopVideo()
{
//    imgprocessorFirst_->stopedThread();
//    imgprocessorSecond_->stopedThread();
//    saveImageAction->setEnabled(false);
//    stopVideoAction->setEnabled(false);
}

void ControlWindow::setPath(QString path)
{
    fileSystem_.setPath(path);
    mainStatusLabel->setText(path);
}

void ControlWindow::videoStream(int frameRate, int countframe, bool isSnapShoot)
{
    int camFirst,camSecond;
    camFirst = fileSystem_.getIndexCameraFirst();
    camSecond = fileSystem_.getIndexCameraSecond();
    if(camFirst!=-1){
        saveImageAction->setEnabled(true);
        stopVideoAction->setEnabled(true);

        imgprocessorFirst_ = new ImageProcessor(&fileSystem_, "FindFirstCameraStream");

        connect(imgprocessorFirst_,
                &ImageProcessor::outDisplay,
                this,
                &ControlWindow::updateFrameFirst);

        connect(imgprocessorFirst_,
                SIGNAL(finished()),
                imgprocessorFirst_,
                SLOT(deleteLater()));

        connect(imgprocessorFirst_,
                SIGNAL(setItem(QTableWidgetItem*, QTableWidgetItem*, QTableWidgetItem*)),
                this,
                SLOT(addItem(QTableWidgetItem*, QTableWidgetItem*, QTableWidgetItem*)));

        connect(imgprocessorFirst_,
                SIGNAL(andStream()),
                this,
                SLOT(andStream()));

        imgprocessorFirst_->setFileSystem(&fileSystem_);
        imgprocessorFirst_->setFrameRate(frameRate);
        imgprocessorFirst_->setCountFrame(countframe);
        imgprocessorFirst_->setIsSnapShoot(isSnapShoot);
        imgprocessorFirst_->setTransformImg(false);
        imgprocessorFirst_->start();
    }
    if(camSecond!=-1)
    {
        saveImageAction->setEnabled(true);
        stopVideoAction->setEnabled(true);
        imgprocessorSecond_ = new ImageProcessor(&fileSystem_, "FindSecondCameraStream");

        connect(imgprocessorSecond_,
                &ImageProcessor::outDisplay,
                this,
                &ControlWindow::updateFrameSecond);

        connect(imgprocessorSecond_,
                SIGNAL(finished()),
                imgprocessorSecond_,
                SLOT(deleteLater()));
        connect(imgprocessorSecond_,
                SIGNAL(setItem(QTableWidgetItem*, QTableWidgetItem*, QTableWidgetItem*)),
                this,
                SLOT(addItem(QTableWidgetItem*, QTableWidgetItem*, QTableWidgetItem*)));

        connect(imgprocessorFirst_,
                SIGNAL(andStream()),
                this,
                SLOT(andStream()));

        imgprocessorSecond_->setFileSystem(&fileSystem_);
        imgprocessorSecond_->setFrameRate(frameRate);
        imgprocessorSecond_->setCountFrame(countframe);
        imgprocessorSecond_->setIsSnapShoot(isSnapShoot);
        imgprocessorSecond_->setTransformImg(false);
        imgprocessorSecond_->start();
    }
}

void ControlWindow::videoStream(QString state)
{
    imgprocessor_ = new ImageProcessor(&fileSystem_, state);

    connect(imgprocessor_,
            &ImageProcessor::outDisplayFirst,
            this,
            &ControlWindow::updateFrameFirst);

    connect(imgprocessor_,
            &ImageProcessor::outDisplaySecond,
            this,
            &ControlWindow::updateFrameSecond);

    imgprocessor_->start();
}

void ControlWindow::updateFrameFirst(QPixmap pix)
{
    imageSceneFirst->clear();
    imageViewFirst->resetTransform();
    imageSceneFirst->addPixmap(pix);
    imageSceneFirst->update();
    imageViewFirst->setSceneRect(pix.rect());
}

void ControlWindow::updateFrameSecond(QPixmap pix)
{
    imageSceneSecond->clear();
    imageViewSecond->resetTransform();
    imageSceneSecond->addPixmap(pix);
    imageSceneSecond->update();
    imageViewSecond->setSceneRect(pix.rect());
}

void ControlWindow::on_imageTable_cellClicked(int row,int col)
{
    if(imageTable->item(row, 0)->checkState() == Qt::Checked){
        fileSystem_.openDrawImgInView(imageTable->item(row, 3)->text());
    }else if(imageTable->item(row, 1)->checkState() == Qt::Checked)
            fileSystem_.openUndistImgInView(imageTable->item(row, 3)->text());
          else
        fileSystem_.openFileInView(imageTable->item(row, 3)->text());
}

void ControlWindow::andStream()
{
    saveImageAction->setEnabled(false);
    stopVideoAction->setEnabled(false);
}

void ControlWindow::addItem(QTableWidgetItem *Item, QTableWidgetItem *Item1, QTableWidgetItem *Item2)
{
    imageTable->setRowCount(imageTable->rowCount()+1);
    imageTable->setItem(imageTable->rowCount()-1,0,Item);
    imageTable->setItem(imageTable->rowCount()-1,1,Item1);
    imageTable->setItem(imageTable->rowCount()-1,3,Item2);
}

void ControlWindow::setCalibOutputTextEdit(QString string)
{
    calibOutputTextEdit->setText(string);
}

void ControlWindow::setStatusImg(QString status, int row)
{
    QTableWidgetItem *st = new QTableWidgetItem(status);
    imageTable->setItem(row,2,st);
}

void ControlWindow::runCalibration()
{
    calibrationProcessor_ = new CalibrationProcessor(&fileSystem_);
    calibrationProcessor_->run();
}

void ControlWindow::openSettingStream()
{
    streamSetting = new StreamSetting(&fileSystem_);

    connect(streamSetting, SIGNAL(sendState(QString)),
            this,SLOT(videoStream(QString)));

    streamSetting->show();
}
