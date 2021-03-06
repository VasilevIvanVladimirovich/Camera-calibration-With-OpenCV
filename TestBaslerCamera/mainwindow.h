#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMatrix4x4>
#include "capturethread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

public slots:
    void updateFrameLabel(QImage);
    void updateFrameLabelTwo(QImage);

private:
    Ui::MainWindow *ui;
    CaptureThread* capturer;
};

#endif // MAINWINDOW_H
