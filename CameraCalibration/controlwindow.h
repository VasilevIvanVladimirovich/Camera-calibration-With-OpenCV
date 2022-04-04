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

    void setStatusImg(QString status, int row);
private:
    void initUi();
    void initImageTable();
    void createAction();

public slots:
    void addItem(QTableWidgetItem *Item, QTableWidgetItem *Item1, QTableWidgetItem *Item2);
    void setCalibOutputTextEdit(QString string);
    void videoStream(QString);

private slots:
    void openProject();
    void createProject();
    void importImage();
    void detect();
    void openCompareWindow();
    void saveImage();
    void undistortStream();
    void stopVideo();
    void setPath(QString);
    void videoStream(int frameRate, int countframe,bool isSnapShoot);
    void updateFrameFirst(QPixmap);
    void updateFrameSecond(QPixmap);
    void on_imageTable_cellClicked(int row,int col);
    void andStream();
    void runCalibration();
    void openSettingStream();

private:
    QAction *openProjectAction;
    QAction *createProjectAction;
    QAction *exitAction;
    QAction *importImageAction;
    QAction *detectAction;
    QAction *openCompareWindowAction;
    QAction *saveImageAction;
    QAction *undistortStreamAction;
    QAction *stopVideoAction;
    QAction *streamAction;

    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *analysisMenu;

    QToolBar *fileToolBar;
    QToolBar *viewToolBar;
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
};

#endif // CONTROLWINDOW_H
