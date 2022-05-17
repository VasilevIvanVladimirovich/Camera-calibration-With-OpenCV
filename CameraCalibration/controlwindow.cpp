#include "controlwindow.h"

ControlWindow::ControlWindow()
{
    initUi();
    initImageTable();
    createAction();
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
   imageViewFirst = new CustomGraphicsView(imageSceneFirst);
   imageViewFirst->verticalScrollBar()->blockSignals(true);
   imageViewFirst->horizontalScrollBar()->blockSignals(true);
   imageViewFirst->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   imageViewFirst->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   imageViewFirst->setTransformationAnchor(QGraphicsView::NoAnchor);

   imageSceneSecond = new QGraphicsScene(this);
   imageViewSecond = new CustomGraphicsView(imageSceneSecond);
   imageViewSecond->verticalScrollBar()->blockSignals(true);
   imageViewSecond->horizontalScrollBar()->blockSignals(true);
   imageViewSecond->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   imageViewSecond->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   imageViewSecond->setTransformationAnchor(QGraphicsView::NoAnchor);

   mainSplitter = new QSplitter(Qt::Horizontal);
   streamBrowserSplitter = new QSplitter(Qt::Vertical);

   imageTable = new QTableWidget();

   connect(&fileSystem_,
           SIGNAL(outTableItems(QTableWidgetItem *,QTableWidgetItem *, QTableWidgetItem *)),
           this,
           SLOT(addItem(QTableWidgetItem *,QTableWidgetItem *, QTableWidgetItem *)));

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

   connect(&fileSystem_,
           SIGNAL(outImgDisplayFirst(QPixmap, std::vector<cv::Point2f>,bool)),
           this,
           SLOT(updateFrameFirst(QPixmap, std::vector<cv::Point2f>,bool)));
   connect(&fileSystem_,
           SIGNAL(outImgDisplaySecond(QPixmap, std::vector<cv::Point2f>,bool)),
           this,
           SLOT(updateFrameSecond(QPixmap, std::vector<cv::Point2f>,bool)));

   connect(&fileSystem_,
           SIGNAL(sendTerminalStr(QString)),
           this,
           SLOT(addStringTerminalBrowser(QString)));

   connect(&fileSystem_,
           SIGNAL(updateCantrolUi()),
           this,
           SLOT(updateUi()));

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
   imageTable->setColumnCount(3);
   imageTable->setHorizontalHeaderLabels(QStringList()<<"#"<<"Camera"<<"Status");
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

    settingCameraAction = new QAction("&Setting Camera", this);
    connect(settingCameraAction, SIGNAL(triggered(bool)), this, SLOT(openSettingCamera()));
    fileMenu->addAction(settingCameraAction);

    settingPatternAction = new QAction("&Setting Pattern", this);
    connect(settingPatternAction, SIGNAL(triggered(bool)), this, SLOT(openSettingPattern()));
    fileMenu->addAction(settingPatternAction);

    exitAction = new QAction("&Exit", this);
    connect(exitAction, SIGNAL(triggered(bool)), QApplication::instance(), SLOT(quit()));
    fileMenu->addAction(exitAction);

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

void ControlWindow::changeDefault()
{
    viewState = DEFAULT_IMAGE;
    defaultImageAction->setEnabled(false);

    drawImageAction->setEnabled(true);
    undistortImageAction->setEnabled(true);
}

void ControlWindow::changeDraw()
{
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

void ControlWindow::setStateTable()
{
    std::vector<FileSystem::InformationImageSaved> infoCamera1;
    std::vector<FileSystem::InformationImageSaved> infoCamera2;

    infoCamera1 = fileSystem_.getInfoCamera1();
    infoCamera2 = fileSystem_.getInfoCamera2();

    for(int i = 0; i < imageTable->rowCount();i++)
    {
        if(imageTable->item(i,0)->checkState())
        {
            if(infoCamera1.size()>i)
                infoCamera1[i].isActive=1;
            if(infoCamera2.size()>i)
                infoCamera2[i].isActive=1;
        }else
        {
            if(infoCamera1.size()>i)
                infoCamera1[i].isActive=0;
            if(infoCamera2.size()>i)
                infoCamera2[i].isActive=0;
        }
    }
    fileSystem_.saveInfoCamera1(infoCamera1);
    fileSystem_.saveInfoCamera2(infoCamera2);
}

void ControlWindow::updateUi()
{
    imageTable->clearContents();
    imageTable->setRowCount(0);
    getTableItems();
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
                                                 | QFileDialog::DontResolveSymlinks
                                                 |QFileDialog::DontUseNativeDialog);
    if(pathName.size()==0)
    {
         addStringTerminalBrowser("No selected project file");
         return;
    }

    fileSystem_.setPath(pathName+"/");
    bool isValidPath;
    isValidPath = fileSystem_.isValidOpenDir();
    if(isValidPath)
    {
        mainStatusLabel->setText(pathName);
        addStringTerminalBrowser("Open project: " + pathName);
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
    setStateTable();
    WindowImportImage = new DialogWindowImportImage();
    WindowImportImage->setFileSystem(&fileSystem_);

    connect(WindowImportImage,
            SIGNAL(signalVideoStream(int)),
            this,
            SLOT(videoStream(int)));
    WindowImportImage->show();
}

void ControlWindow::detect()
{
    setStateTable();
    calibrationProcessor_ = new CalibrationProcessor(&fileSystem_);

    connect(calibrationProcessor_,
            SIGNAL(updateCantrolUi()),
            this,
            SLOT(updateUi()));


    calibrationProcessor_->setState(CalibrationProcessor::State::ACCUMULATION);
    calibrationProcessor_->run();
}

void ControlWindow::calibration()
{
    WindowDetectCalibration = new DialogWindowDetectCalibration();
    WindowDetectCalibration->setFileSystem(&fileSystem_);
    WindowDetectCalibration->show();

    connect(WindowDetectCalibration,
            SIGNAL(goCalib()),
            this,
            SLOT(runCalibration()));
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
    fileSystem_.setPath(path);
    mainStatusLabel->setText(path);
    addStringTerminalBrowser("Create project");
    updateUi();
}

void ControlWindow::videoStream(int countframe)
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
                SIGNAL(outDisplay(QPixmap)),
                this,
                SLOT(updateFrameFirst(QPixmap)));

        connect(imgprocessorFirst_,
                SIGNAL(finished()),
                imgprocessorFirst_,
                SLOT(deleteLater()));

        connect(imgprocessorFirst_,
                SIGNAL(setItem(QTableWidgetItem*,QTableWidgetItem*, QTableWidgetItem*)),
                this,
                SLOT(addItem(QTableWidgetItem*,QTableWidgetItem*, QTableWidgetItem*)));

        connect(imgprocessorFirst_,
                SIGNAL(andStream()),
                this,
                SLOT(andStream()));

        connect(imgprocessorFirst_,
                SIGNAL(sendTerminal(QString)),
                this,
                SLOT(addStringTerminalBrowser(QString)));

        imgprocessorFirst_->setFileSystem(&fileSystem_);
        imgprocessorFirst_->setCountFrame(countframe);
        imgprocessorFirst_->start();
    }
    if(camSecond!=-1)
    {
        isRunningSecondCamera = true;
        saveImageAction->setEnabled(true);
        stopVideoAction->setEnabled(true);
        imgprocessorSecond_ = new ImageProcessor(&fileSystem_, "FindSecondCameraStream",data_lock);

        connect(imgprocessorSecond_,
                SIGNAL(outDisplay(QPixmap)),
                this,
                SLOT(updateFrameSecond(QPixmap)));

        connect(imgprocessorSecond_,
                SIGNAL(finished()),
                imgprocessorSecond_,
                SLOT(deleteLater()));
        connect(imgprocessorSecond_,
                SIGNAL(setItem(QTableWidgetItem*,QTableWidgetItem*, QTableWidgetItem*)),
                this,
                SLOT(addItem(QTableWidgetItem*,QTableWidgetItem*, QTableWidgetItem*)));

        connect(imgprocessorFirst_,
                SIGNAL(andStream()),
                this,
                SLOT(andStream()));

        imgprocessorSecond_->setFileSystem(&fileSystem_);
        imgprocessorSecond_->setCountFrame(countframe);
        imgprocessorSecond_->setTransformImg(false);
        imgprocessorSecond_->start();
    }
}

void ControlWindow::videoStream(QString state)
{
    imgprocessor_ = new ImageProcessor(&fileSystem_, state,data_lock);

    connect(imgprocessor_,
            SIGNAL(outDisplayFirst(QPixmap)),
            this,
            SLOT(updateFrameFirst(QPixmap)));

    connect(imgprocessor_,
            SIGNAL(outDisplaySecond(QPixmap)),
            this,
            SLOT(updateFrameSecond(QPixmap)));

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

void ControlWindow::updateFrameFirst(QPixmap pix, std::vector<cv::Point2f> imgpoint,bool isActive)
{
    imageSceneFirst->clear();
    imageSceneFirst->addPixmap(pix);

    QPen pen;
    if(isActive)
        pen.setColor(Qt::green);
    else
        pen.setColor(Qt::gray);
    pen.setWidth(0);
    for(unsigned short int i = 0; imgpoint.size() > i;i++)
    {
        imageSceneFirst->addLine(imgpoint[i].x-5,imgpoint[i].y,imgpoint[i].x+5,imgpoint[i].y,pen);
        imageSceneFirst->addLine(imgpoint[i].x,imgpoint[i].y-5,imgpoint[i].x,imgpoint[i].y+5,pen);
    }
    imageSceneFirst->update();
    imageViewFirst->setSceneRect(pix.rect());
}

void ControlWindow::updateFrameSecond(QPixmap pix, std::vector<cv::Point2f> imgpoint,bool isActive)
{
    imageSceneSecond->clear();
    imageSceneSecond->addPixmap(pix);

    QPen pen;
    if(isActive)
        pen.setColor(Qt::green);
    else
        pen.setColor(Qt::gray);
    pen.setWidth(0);
    for(unsigned short int i = 0; imgpoint.size() > i;i++)
    {
        imageSceneSecond->addLine(imgpoint[i].x-5,imgpoint[i].y,imgpoint[i].x+5,imgpoint[i].y,pen);
        imageSceneSecond->addLine(imgpoint[i].x,imgpoint[i].y-5,imgpoint[i].x,imgpoint[i].y+5,pen);
    }
    imageSceneSecond->update();
    imageViewSecond->setSceneRect(pix.rect());


}

void ControlWindow::on_imageTable_cellClicked(int row,int col)
{
    if(col == 0)
    {
        if(imageTable->item(row,col)->checkState())
        {
            imageTable->item(row,0)->setBackground(Qt::white);
            imageTable->item(row,1)->setBackground(Qt::white);
            imageTable->item(row,2)->setBackground(Qt::white);
        }else
        {
            imageTable->item(row,0)->setBackground(Qt::gray);
            imageTable->item(row,1)->setBackground(Qt::gray);
            imageTable->item(row,2)->setBackground(Qt::gray);
        }
    }
    switch(viewState)
    {
        case DEFAULT_IMAGE:
        fileSystem_.openFileInView(row, imageTable->item(row,0)->checkState());
        break;
        case UNDISTORTED_IMAGE:
        fileSystem_.openUndistImgInView(row);
        break;
    }
}

void ControlWindow::andStream()
{
    saveImageAction->setEnabled(false);
    stopVideoAction->setEnabled(false);

    isRunningFirstCamera = false;
    isRunningSecondCamera = false;

    fileSystem_.getTableItems();
    updateUi();
}

void ControlWindow::addItem(QTableWidgetItem *Item0,QTableWidgetItem *Item1, QTableWidgetItem *Item2)
{
    imageTable->setRowCount(imageTable->rowCount()+1);
    imageTable->setItem(imageTable->rowCount()-1,0,Item0);
    imageTable->setItem(imageTable->rowCount()-1,1,Item1);
    imageTable->setItem(imageTable->rowCount()-1,2,Item2);

    if(Item0->checkState())
    {
        imageTable->item(imageTable->rowCount()-1,0)->setBackground(Qt::white);
        imageTable->item(imageTable->rowCount()-1,1)->setBackground(Qt::white);
        imageTable->item(imageTable->rowCount()-1,2)->setBackground(Qt::white);
    }else
    {
        imageTable->item(imageTable->rowCount()-1,0)->setBackground(Qt::gray);
        imageTable->item(imageTable->rowCount()-1,1)->setBackground(Qt::gray);
        imageTable->item(imageTable->rowCount()-1,2)->setBackground(Qt::gray);
    }
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

void ControlWindow::openSettingCamera()
{

    settingCameraWindow = new SettingCameraWindow(&fileSystem_);
    connect(settingCameraWindow, SIGNAL(sendUpdate()),
            this,SLOT(updateUi()));

    settingCameraWindow->show();
}

void ControlWindow::openSettingPattern()
{
    settingPatternWindow = new SettingPatternWindow(&fileSystem_);
    connect(settingPatternWindow, SIGNAL(sendUpdate()),
            this,SLOT(updateUi()));

    settingPatternWindow->show();
}
