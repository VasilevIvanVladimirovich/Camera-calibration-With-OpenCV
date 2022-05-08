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

class ControlWindow: public QMainWindow
{
    Q_OBJECT
public:
    ControlWindow();

    enum ViewState{
        DEFAULT_IMAGE,
        DRAW_IMAGE,
        UNDISTORTED_IMAGE
    };

    void setStatusImg(QString status, int row);
private:
    void initUi();
    void initImageTable();
    void createAction();
    void setupShortcuts();

    void replot();

    void getTableItems();
    void updateUi();
public slots:
    void addItem(QTableWidgetItem *Item1, QTableWidgetItem *Item2);
    void videoStream(QString);
    void addStringTerminalBrowser(QString);
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
    void videoStream(int frameRate, int countframe,bool isSnapShoot);
    void updateFrameFirst(QPixmap);
    void updateFrameSecond(QPixmap);
    void on_imageTable_cellClicked(int row,int col);
    void andStream();
    void runCalibration();
    void openSettingStream();

    void zoomIn();
    void zoomOut();

    void changeDefault();
    void changeDraw();
    void changeUndistort();
private:
    QAction *openProjectAction;
    QAction *createProjectAction;
    QAction *exitAction;
    QAction *importImageAction;
    QAction *detectAction;
    QAction *calibrationAction;
    QAction *openCompareWindowAction;
    QAction *saveImageAction;
    QAction *stopVideoAction;
    QAction *streamAction;


    QAction *defaultImageAction;
    QAction *drawImageAction;
    QAction *undistortImageAction;

    QAction* zoomInAction;
    QAction* zoomOutAction;

    QMenu *fileMenu;
    QMenu *viewMenu;
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

    QGraphicsView *imageViewFirst;
    QGraphicsView *imageViewSecond;

    QTableWidget *imageTable;

    QTextEdit *calibOutputTextEdit;

    FileSystem fileSystem_;

    StreamSetting* streamSetting;

    ImageProcessor *imgprocessorFirst_;
    ImageProcessor *imgprocessorSecond_;
    ImageProcessor *imgprocessor_;

    CalibrationProcessor *calibrationProcessor_;

    DialogWindowImportImage* WindowImportImage;
    DialogWindowDetectCalibration* WindowDetectCalibration;

    QMutex* data_lock;

    ViewState viewState;

    QTabWidget* tabMain;
    QTabWidget* tabHistogrammCharts;
    QTabWidget* tabCalibBrowser;

    QTextBrowser* terminalBrowser;
    QTextBrowser* projectSettingBrowser;
    QTextBrowser* calibrationBrowser1;
    QTextBrowser* calibrationBrowser2;

    QChart *chartHistogrammCameraFirst;
    QChart *chartHistogrammCameraSecond;

    QChartView *chartViewHistogrammCameraFirst;
    QChartView *chartViewHistogrammCameraSecond;

    QBarSet *setCameraFirst;
    QBarSet *setCameraSecond;

    QBarSeries *seriesCameraFirst;
    QBarSeries *seriesCameraSecond;

    QSplitter* streamBrowserSplitter;
    QSplitter* mainSplitter;

    bool isRunningFirstCamera;
    bool isRunningSecondCamera;

    QString terminalBrowserString;
};

#endif // CONTROLWINDOW_H
