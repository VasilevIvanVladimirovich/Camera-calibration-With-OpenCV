#include "controlwindow.h"

ControlWindow::ControlWindow()
{
    initUi();
    initImageTable();
    createAction();
    setupShortcuts();
    data_lock = new QMutex();
    changeDefault();

    isRunningFirstCamera = false;
    isRunningSecondCamera = false;

    addStringTerminalBrowser("You have successfully opened the program, you can start working!");
}

void ControlWindow::initUi()
{
   // setup menubar
   fileMenu = menuBar()->addMenu("&File");
   viewMenu = menuBar()->addMenu("&View");
   imageMenu = menuBar()->addMenu("&Image");
   analysisMenu= menuBar()->addMenu("&Analysis");
   // setup toolbar
   fileToolBar = addToolBar("File");
   viewToolBar = addToolBar("View");
   imageToolBar = addToolBar("Image");
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

   mainSplitter = new QSplitter(Qt::Horizontal);
   streamBrowserSplitter = new QSplitter(Qt::Vertical);

   imageTable = new QTableWidget();

   connect(&fileSystem_,
           SIGNAL(outTableItems(QTableWidgetItem *, QTableWidgetItem *)),
           this,
           SLOT(addItem(QTableWidgetItem *, QTableWidgetItem *)));

   mainSplitter->addWidget(imageTable);

   terminalBrowser = new QTextBrowser();

   QHBoxLayout* layout_stream = new QHBoxLayout;
   layout_stream->addWidget(imageViewFirst);
   layout_stream->addWidget(imageViewSecond);

   QWidget* widget_stream = new QWidget();
   widget_stream->setLayout(layout_stream);

   streamBrowserSplitter->addWidget(widget_stream);
   streamBrowserSplitter->addWidget(terminalBrowser);

   mainSplitter->addWidget(streamBrowserSplitter);

   calibOutputTextEdit = new QTextEdit();

   connect(&fileSystem_,
           SIGNAL(outImgDisplayFirst(QPixmap)),
           this,
           SLOT(updateFrameFirst(QPixmap)));
   connect(&fileSystem_,
           SIGNAL(outImgDisplaySecond(QPixmap)),
           this,
           SLOT(updateFrameSecond(QPixmap)));

   calibOutputTextEdit->setReadOnly(true);
   calibOutputTextEdit->setMaximumHeight(250);

   chartHistogrammCameraFirst = new QChart();
   chartHistogrammCameraFirst->setTitle("Camera1");
   chartHistogrammCameraFirst->setAnimationOptions(QChart::SeriesAnimations);

   chartHistogrammCameraSecond = new QChart();
   chartHistogrammCameraSecond->setTitle("Camera2");
   chartHistogrammCameraSecond->setAnimationOptions(QChart::SeriesAnimations);

   chartViewHistogrammCameraFirst = new QChartView(chartHistogrammCameraFirst);
   chartViewHistogrammCameraSecond = new QChartView(chartHistogrammCameraSecond);

   tabCalibBrowser = new QTabWidget();

   calibrationBrowser1 = new QTextBrowser();
   calibrationBrowser2 = new QTextBrowser();
   tabCalibBrowser->addTab(calibrationBrowser1,"Camera1");
   tabCalibBrowser->addTab(calibrationBrowser2,"Camera2");

   projectSettingBrowser = new QTextBrowser();

   tabHistogrammCharts = new QTabWidget();
   tabHistogrammCharts->addTab(chartViewHistogrammCameraFirst,"Camera1");
   tabHistogrammCharts->addTab(chartViewHistogrammCameraSecond,"Camera2");

   tabMain = new QTabWidget();
   tabMain->addTab(projectSettingBrowser,"Project");
   tabMain->addTab(tabCalibBrowser,"Calibration");
   tabMain->addTab(tabHistogrammCharts,"RPE Bars");

   mainSplitter->addWidget(tabMain);
   setCentralWidget(mainSplitter);
}

void ControlWindow::initImageTable()
{
   imageTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
   imageTable->setMaximumWidth(250);
   imageTable->setColumnCount(2);
   imageTable->setHorizontalHeaderLabels(QStringList()<<"Camera"<<"Status");
   imageTable->setShowGrid(false);
   imageTable->horizontalHeader()->setStretchLastSection(true);
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

    zoomOutAction = new QAction("&ZoomOut",this);
    connect(zoomOutAction, SIGNAL(triggered(bool)), this, SLOT(zoomOut()));
    viewMenu->addAction(zoomOutAction);
    viewToolBar->addAction(zoomOutAction);

    zoomInAction = new QAction("&ZoomIn",this);
    connect(zoomInAction, SIGNAL(triggered(bool)), this, SLOT(zoomIn()));
    viewMenu->addAction(zoomInAction);
    viewToolBar->addAction(zoomInAction);

    defaultImageAction = new QAction("&Default",this);
    connect(defaultImageAction, SIGNAL(triggered(bool)), this, SLOT(changeDefault()));
    viewMenu->addAction(defaultImageAction);
    viewToolBar->addAction(defaultImageAction);

    drawImageAction = new QAction("&Draw",this);
    connect(drawImageAction, SIGNAL(triggered(bool)), this, SLOT(changeDraw()));
    viewMenu->addAction(drawImageAction);
    viewToolBar->addAction(drawImageAction);

    undistortImageAction = new QAction("&Undistort",this);
    connect(undistortImageAction, SIGNAL(triggered(bool)), this, SLOT(changeUndistort()));
    viewMenu->addAction(undistortImageAction);
    viewToolBar->addAction(undistortImageAction);

    importImageAction = new QAction("&Import image", this);
    connect(importImageAction, SIGNAL(triggered(bool)), this, SLOT(importImage()));
    imageMenu->addAction(importImageAction);
    imageToolBar->addAction(importImageAction);

    detectAction = new QAction("&Detect", this);
    connect(detectAction, SIGNAL(triggered(bool)), this, SLOT(detect()));
    imageMenu->addAction(detectAction);
    imageToolBar->addAction(detectAction);

    calibrationAction = new QAction("&Calibration", this);
    connect(calibrationAction, SIGNAL(triggered(bool)), this, SLOT(calibration()));
    imageMenu->addAction(calibrationAction);
    imageToolBar->addAction(calibrationAction);

    openCompareWindowAction = new QAction("&Compare Window", this);
    connect(openCompareWindowAction, SIGNAL(triggered(bool)), this, SLOT(openCompareWindow()));
    analysisMenu->addAction(openCompareWindowAction);
    analysisToolBar->addAction(openCompareWindowAction);

    streamAction = new QAction("&Stream Action", this);
    connect(streamAction, SIGNAL(triggered(bool)), this, SLOT(openSettingStream()));
    analysisMenu->addAction(streamAction);
    analysisToolBar->addAction(streamAction);

    saveImageAction = new QAction("&Save Image", this);
    connect(saveImageAction, SIGNAL(triggered(bool)), this, SLOT(saveImage()));
    videoToolBar->addAction(saveImageAction);
    saveImageAction->setEnabled(false);

    stopVideoAction = new QAction("&Stop Video", this);
    connect(stopVideoAction, SIGNAL(triggered(bool)), this, SLOT(stopVideo()));
    videoToolBar->addAction(stopVideoAction);
    stopVideoAction->setEnabled(false);
}

void ControlWindow::zoomIn()
{
    imageViewFirst->scale(1.2,1.2);
    imageViewSecond->scale(1.2,1.2);
}

void ControlWindow::zoomOut()
{
    imageViewFirst->scale(1/1.2,1/1.2);
    imageViewSecond->scale(1/1.2,1/1.2);
}

void ControlWindow::changeDefault()
{
    viewState = DEFAULT_IMAGE;
    defaultImageAction->setEnabled(false);

    drawImageAction->setEnabled(true);
    undistortImageAction->setEnabled(true);
}

void ControlWindow::changeDraw()
{
    viewState = DRAW_IMAGE;
    drawImageAction->setEnabled(false);

    defaultImageAction->setEnabled(true);
    undistortImageAction->setEnabled(true);
}

void ControlWindow::changeUndistort()
{
    viewState = UNDISTORTED_IMAGE;
    undistortImageAction->setEnabled(false);

    defaultImageAction->setEnabled(true);
    drawImageAction->setEnabled(true);

}

void ControlWindow::setupShortcuts()
{
    QList<QKeySequence> shortcuts;
    shortcuts << Qt::Key_Plus << Qt::Key_Equal;
    zoomInAction->setShortcuts(shortcuts);
    shortcuts.clear();

    shortcuts << Qt::Key_Minus << Qt::Key_Underscore;
    zoomOutAction->setShortcuts(shortcuts);
    shortcuts.clear();
}

void ControlWindow::replot()
{
//    std::vector<FileSystem::InformationImageSaved> infoCamera1 = fileSystem_.getInfoCamera1();
//    std::vector<FileSystem::InformationImageSaved> infoCamera2 = fileSystem_.getInfoCamera2();

//    setCameraFirst = new QBarSet("Camera1");
//    for(int i = 0;i < infoCamera1.size();i++)
//    {
//        *setCameraFirst<<infoCamera1[i].err;
//        qDebug()<<i;
//    }
//    qDebug()<<"setCameraFirst";
//    setCameraSecond = new QBarSet("Camera2");
//    for(int i = 0;i < infoCamera2.size();i++)
//    {
//        *setCameraSecond<<infoCamera2[i].err;
//        qDebug()<<i;
//    }
//    qDebug()<<"setCameraSecond";
//    seriesCameraFirst = new QBarSeries();
//    seriesCameraFirst->append(setCameraFirst);

//    seriesCameraSecond = new QBarSeries();
//    seriesCameraSecond->append(setCameraSecond);


////    chartHistogrammCameraFirst->remove();
////    chartHistogrammCameraSecond->removeAllSeries();

//    chartHistogrammCameraFirst->addSeries(seriesCameraFirst);
//    chartHistogrammCameraSecond->addSeries(seriesCameraSecond);

//    qDebug()<<"addSeries";

//    QBarCategoryAxis *axisFirst = new QBarCategoryAxis();
//    chartHistogrammCameraFirst->createDefaultAxes();
//    chartHistogrammCameraFirst->axisY()->setRange(0,10);
//    chartHistogrammCameraFirst->setAxisX(axisFirst,seriesCameraFirst);
//    qDebug()<<"axisFirst";

//    QBarCategoryAxis *axisSecond = new QBarCategoryAxis();
//    chartHistogrammCameraSecond->createDefaultAxes();
//    chartHistogrammCameraSecond->axisY()->setRange(0,10);
//    chartHistogrammCameraSecond->setAxisX(axisSecond,seriesCameraSecond);
//    qDebug()<<"axisSecond";

}

void ControlWindow::getTableItems()
{
    imageTable->clearContents();
    imageTable->setRowCount(0);
    fileSystem_.getTableItems();
}

void ControlWindow::updateUi()
{
   projectSettingBrowser->setText(fileSystem_.openProjectSetting());
   calibrationBrowser1->setText(fileSystem_.openCalibSetting(1));
   calibrationBrowser2->setText(fileSystem_.openCalibSetting(2));
}

void ControlWindow::openProject()
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
        mainStatusLabel->setText(pathName);
        getTableItems();
        addStringTerminalBrowser("Open project");
        updateUi();
    }else
        QMessageBox::warning(this, tr("Warning"),tr("The folder does not meet the standard"));
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

//        WindowDetectCalibration = new DialogWindowDetectCalibration();
//        WindowDetectCalibration->setFileSystem(&fileSystem_);
//        WindowDetectCalibration->show();

//        connect(WindowDetectCalibration,
//                SIGNAL(goCalib()),
//                this,
//                SLOT(runCalibration()));
}

void ControlWindow::openCompareWindow()
{
    TableCompare *tableCompare_ = new TableCompare();
    tableCompare_->show();
}

void ControlWindow::saveImage()
{
    if(isRunningFirstCamera)
    {
        imgprocessorFirst_->setIsPressSnap();

    }
    if(isRunningSecondCamera)
    {
        imgprocessorSecond_->setIsPressSnap();
    }
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
    getTableItems();
    fileSystem_.setPath(path);
    mainStatusLabel->setText(path);


    addStringTerminalBrowser("Create project");
    updateUi();
}

void ControlWindow::videoStream(int frameRate, int countframe, bool isSnapShoot)
{
    int camFirst,camSecond;
    camFirst = fileSystem_.getIndexCameraFirst();
    camSecond = fileSystem_.getIndexCameraSecond();
    saveImageAction->setEnabled(true);
    stopVideoAction->setEnabled(true);
    if(camFirst!=-1){
        isRunningFirstCamera = true;
        saveImageAction->setEnabled(true);
        stopVideoAction->setEnabled(true);

        imgprocessorFirst_ = new ImageProcessor(&fileSystem_, "FindFirstCameraStream",data_lock);

        connect(imgprocessorFirst_,
                &ImageProcessor::outDisplay,
                this,
                &ControlWindow::updateFrameFirst);

        connect(imgprocessorFirst_,
                SIGNAL(finished()),
                imgprocessorFirst_,
                SLOT(deleteLater()));

        connect(imgprocessorFirst_,
                SIGNAL(setItem(QTableWidgetItem*, QTableWidgetItem*)),
                this,
                SLOT(addItem(QTableWidgetItem*, QTableWidgetItem*)));

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
        isRunningSecondCamera = true;
        saveImageAction->setEnabled(true);
        stopVideoAction->setEnabled(true);
        imgprocessorSecond_ = new ImageProcessor(&fileSystem_, "FindSecondCameraStream",data_lock);

        connect(imgprocessorSecond_,
                &ImageProcessor::outDisplay,
                this,
                &ControlWindow::updateFrameSecond);

        connect(imgprocessorSecond_,
                SIGNAL(finished()),
                imgprocessorSecond_,
                SLOT(deleteLater()));
        connect(imgprocessorSecond_,
                SIGNAL(setItem(QTableWidgetItem*, QTableWidgetItem*)),
                this,
                SLOT(addItem(QTableWidgetItem*, QTableWidgetItem*)));

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
    imgprocessor_ = new ImageProcessor(&fileSystem_, state,data_lock);

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

void ControlWindow::addStringTerminalBrowser(QString str)
{
    QDateTime date;
    date = QDateTime::currentDateTime();
    terminalBrowserString.push_back(date.toString("HH:mm:ss") + "  " + str + "\n");
    terminalBrowser->setText(terminalBrowserString);
}

void ControlWindow::updateFrameFirst(QPixmap pix)
{
    imageSceneFirst->clear();
    imageSceneFirst->addPixmap(pix);
    imageSceneFirst->update();
    imageViewFirst->setSceneRect(pix.rect());
}

void ControlWindow::updateFrameSecond(QPixmap pix)
{
    imageSceneSecond->clear();
    imageSceneSecond->addPixmap(pix);
    imageSceneSecond->update();
    imageViewSecond->setSceneRect(pix.rect());
}

void ControlWindow::on_imageTable_cellClicked(int row,int col)
{
    switch(viewState)
    {
        case DEFAULT_IMAGE:
        fileSystem_.openFileInView(row);
        break;
        case DRAW_IMAGE:
        fileSystem_.openDrawImgInView(row);
        break;
        case UNDISTORTED_IMAGE:
        fileSystem_.openUndistImgInView(row);
        break;
    }
}

void ControlWindow::andStream()
{
    imageTable->clearContents();
    imageTable->setRowCount(0);
    saveImageAction->setEnabled(false);
    stopVideoAction->setEnabled(false);

    isRunningFirstCamera = false;
    isRunningSecondCamera = false;

    fileSystem_.getTableItems();
    updateUi();
}

void ControlWindow::addItem(QTableWidgetItem *Item1, QTableWidgetItem *Item2)
{
    imageTable->setRowCount(imageTable->rowCount()+1);
    imageTable->setItem(imageTable->rowCount()-1,0,Item1);
    imageTable->setItem(imageTable->rowCount()-1,1,Item2);
}

void ControlWindow::setStatusImg(QString status, int row)
{
    QTableWidgetItem *st = new QTableWidgetItem(status);
    imageTable->setItem(row,2,st);
}

void ControlWindow::runCalibration()
{
    calibrationProcessor_ = new CalibrationProcessor(&fileSystem_);

    connect(calibrationProcessor_,
            SIGNAL(sendCalibBrowser()),
            &fileSystem_,
            SLOT(updateUi()));

    calibrationProcessor_->run();
}

void ControlWindow::openSettingStream()
{
    streamSetting = new StreamSetting(&fileSystem_);

    connect(streamSetting, SIGNAL(sendState(QString)),
            this,SLOT(videoStream(QString)));

    streamSetting->show();
}
