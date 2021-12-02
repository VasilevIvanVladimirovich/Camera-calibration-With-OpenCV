#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QTimer>
#include <QString>
#include <QThread>
#include <QPicture>
#include <QtGui>
#include <QFileDialog>
#include <QObject>
#include <QMessageBox>
#include <QTreeWidget>
#include <QListWidget>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/persistence.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include "imageprocessor.h"
#include "calibrationprocessor.h"
#include "filesystem.h"
#include "dialogdetect.h"
#include "tablecompare.h"

#include <stdio.h>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QVector<QString> getVectorImgFromTable();
    void videoStream(QString qstring);

public slots:
    void setqToolBarNew();
    void setqToolBarOpen();
    void videoStream(int frameRate, int countframe, int row,
                     int col, bool isPattern, bool isSnapShoot, QString pattern);
    void addItem(QTableWidgetItem *Item, QTableWidgetItem *item1, QTableWidgetItem *Item2);
    void setStatusImg(QString status,int row);
private slots:
    void on_btn_setImg_clicked();
    void on_tableWidget_cellClicked(int row, int column);
    void on_btn_detect_clicked();
    void on_btn_stopVideo_clicked();
    void on_chekResultInVideoStream_clicked();
    void on_OpenTable_clicked();
    void on_btn_snapshoot_clicked();

private:
    Ui::MainWindow *ui;

    QImage imgcam_;

    QString debugline_;

    ImageProcessor *imgprocessor_ = nullptr;

    CalibrationProcessor *calibprocessor_ = nullptr;

    FileSystem fileSystem_;

    TableCompare* tableCompare_;
};
#endif // MAINWINDOW_H
