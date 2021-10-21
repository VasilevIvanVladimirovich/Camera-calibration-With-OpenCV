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

#include <stdio.h>
#include <QListWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QImage imgcam_;

    QString debugline_;

    ImageProcessor *imgprocessor_ = nullptr;

    CalibrationProcessor calibprocessor_;

    FileSystem fileSystem_;

public slots:
    void addItem(QTableWidgetItem *Item1,QTableWidgetItem *Item2);
private slots:
    void on_btn_setImg_clicked();
    void on_tableWidget_cellClicked(int row, int column);
    void on_btn_detect_clicked();
};
#endif // MAINWINDOW_H
