#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QTimer>
#include <QString>
#include <QThread>
#include <QPicture>

#include<opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>
#include <opencv2/core/persistence.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/calib3d/calib3d.hpp>

#include<WebcamProcessorThread.h>

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

private:
    Ui::MainWindow *ui;

    QImage imgcam;

    WebcamProcessorThread *web_cam_processor = new WebcamProcessorThread;

public slots:
    void processFrameAndUpdateGUI();

private slots:
    void on_btn_calibration_clicked();
};
#endif // MAINWINDOW_H
