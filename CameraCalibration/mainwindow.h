#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QTimer>

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc.hpp>

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

    cv::VideoCapture webcam;

    cv::Mat matimg;

    QTimer *tmrTimer;

    QImage imgcam;

public slots:
    void processFrameAndUpdateGUI();

private slots:
    void on_btn_calibration_clicked();
};
#endif // MAINWINDOW_H
