#ifndef CONTROLWINDOW_H
#define CONTROLWINDOW_H

#include <QMainWindow>

#include <QMenuBar>
#include <QToolBar>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTextEdit>
#include <QLabel>
#include <QStatusBar>
#include <QApplication>
#include <QMutex>

#include <QtCharts>
#include <QChartView>
#include <QBarSet>
#include <QBarSeries>
#include <QSplitter>

#include <QTextBrowser>

#include <QTabWidget>

#include <QShortcut>

#include "imageprocessor.h"
#include "calibrationprocessor.h"
#include "filesystem.h"
#include "tablecompare.h"
#include "dialogwindowimportimage.h"
#include "dialogwindowdetectcalibration.h"
#include "menuwindownewfile.h"
#include "streamsetting.h"
#include "settingcamerawindow.h"
#include "settingpatternwindow.h"
#include "customgraphicsview.h"

class ControlWindow: public QMainWindow
{
    Q_OBJECT
public:
    ControlWindow();

    enum ViewState{
        DEFAULT_IMAGE,
        UNDISTORTED_IMAGE
    };

    void setStatusImg(QString status, int row);
private:
    void initUi();
    void initImageTable();
    void createAction();

    void replotHist();

    void getTableItems();
    void setStateTable();

public slots:
    void addItem(QTableWidgetItem *Item0, QTableWidgetItem *Item1, QTableWidgetItem *Item2);
    void videoStream(QString);
    void addStringTerminalBrowser(QString);
    void updateUi();
    void createProject(QString);
private slots:
    void openProject();
    void createProject();

    void importImage();
    void detect();
    void calibration();
    void openCompareWindow();
    void saveImage();
    void stopVideo();
    void setPath(QString);
    void videoStream(int countframe,bool,bool,bool);
    void updateFrameFirstSecond(QPixmap,QPixmap,QMutex*);
    void updateFrameFirst(QPixmap);
    void updateFrameSecond(QPixmap);
    void updateFrameFirst(QPixmap, std::vector<cv::Point2f> imgpoint,bool isActive);
    void updateFrameSecond(QPixmap, std::vector<cv::Point2f> imgpoint,bool isActive);
    void updateFrameFirst(QPixmap pixmap, std::vector<cv::Point2f> imgpoint,
                          std::vector<cv::Point2f> reprojpoint,double err, bool isActive);
    void updateFrameSecond(QPixmap pixmap, std::vector<cv::Point2f> imgpoint,
                           std::vector<cv::Point2f> reprojpoint,double err, bool isActive);
    void on_imageTable_cellClicked(int row,int col);
    void andStream();
    void runCalibration();
    void openSettingStream();
    void openSettingCamera();
    void openSettingPattern();

    void changeDefault();
    void changeUndistort();

    void hideSecondView();

    void histClicked1(int);
    void histClicked2(int);
private:
    QAction *openProjectAction;
    QAction *createProjectAction;
    QAction *settingCameraAction;
    QAction *settingPatternAction;
    QAction *exitAction;
    QAction *importImageAction;
    QAction *detectAction;
    QAction *calibrationAction;
    QAction *openCompareWindowAction;
    QAction *saveImageAction;
    QAction *stopVideoAction;
    QAction *streamAction;

    QAction *defaultImageAction;
    QAction *undistortImageAction;
    QAction *hideSecondViewAction;

    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *toolMenu;
    QMenu *imageMenu;
    QMenu *analysisMenu;

    QToolBar *fileToolBar;
    QToolBar *viewToolBar;
    QToolBar *imageToolBar;
    QToolBar *analysisToolBar;
    QToolBar *videoToolBar;

    QStatusBar *mainStatusBar;
    QLabel *mainStatusLabel;

    QGraphicsScene *imageSceneFirst;
    QGraphicsScene *imageSceneSecond;

    CustomGraphicsView *imageViewFirst;
    CustomGraphicsView *imageViewSecond;

    QTableWidget *imageTable;

    QTextEdit *calibOutputTextEdit;

    FileSystem fileSystem_;

    StreamSetting* streamSetting;
    SettingCameraWindow* settingCameraWindow;
    SettingPatternWindow* settingPatternWindow;
    TableCompare tableCompare;

    ImageProcessor *imgprocessorFirst_;
    ImageProcessor *imgprocessorSecond_;
    ImageProcessor *imgprocessor_;

    CalibrationProcessor *calibrationProcessor_;

    DialogWindowImportImage* WindowImportImage;
    DialogWindowDetectCalibration* WindowDetectCalibration;

    QMutex* data_lock;

    ViewState viewState;

    QTabWidget* tabSetting;
    QTabWidget* tabMain;
    QTabWidget* tabHistogrammCharts;
    QTabWidget* tabCalibBrowser;

    QTextBrowser* terminalBrowser;
    QTextBrowser* projectSettingBrowser;
    QTextBrowser* calibrationBrowser1;
    QTextBrowser* calibrationBrowser2;
    QTextBrowser* stereoBrowser;

    QChart *chartHistogrammCameraFirst;
    QChart *chartHistogrammCameraSecond;
    QChart *chartHistogrammStereo;

    QChartView *chartViewHistogrammCameraFirst;
    QChartView *chartViewHistogrammCameraSecond;
    QChart *chartViewHistogrammStereo;

    QChartView *chartViewScetterCameraFirst;
    QChartView *chartViewScetterCameraSecond;

    QBarSet *setCameraFirst;
    QBarSet *setCameraSecond;

    QBarSeries *seriesCameraFirst;
    QBarSeries *seriesCameraSecond;

    QLineSeries* serieMeanCamFirst;
    QLineSeries* serieMeanCamSecond;

    QSplitter* streamBrowserSplitter;
    QSplitter* mainSplitter;
    QSplitter* tableSetting;
    QSplitter* infoCompare;

    bool isRunningFirstCamera = false;
    bool isRunningSecondCamera = false;
    bool isRunningStereoBasler = false;

    QString terminalBrowserString;
};

#endif // CONTROLWINDOW_H
