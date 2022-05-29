#ifndef DIALOGWINDOWIMPORTIMAGE_H
#define DIALOGWINDOWIMPORTIMAGE_H

#include <QWidget>
#include <QtWidgets>
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
#include <QMetaObject>
#include <QFormLayout>
#include <QList>
#include <QComboBox>
#include <QCameraDevice>
#include <QMediaDevices>

#include "filesystem.h"

class DialogWindowImportImage : public QWidget
{
    Q_OBJECT
public:
    DialogWindowImportImage(QWidget* parent = 0);
    ~DialogWindowImportImage();

    void setFileSystem(FileSystem* fs);

signals:
    void signalVideoStream(int countframe,bool,bool,bool);

private slots:
    void on_btnStartVideoStream_clicked();
    void on_btnSetFolderPath1_clicked();
    void on_btnSetFolderPath2_clicked();
    void on_btnSetOk_clicked();

private:
    void initUi();

private:
    FileSystem* fs;
    QString pathName1;
    QString pathName2;

    QCheckBox* isCamera1;
    QCheckBox* isCamera2;
    QCheckBox* isDrawing;

    QPushButton* btnStartVideoStream;
    QPushButton* btnSetFolderPath1;
    QPushButton* btnSetFolderPath2;
    QPushButton* btnSetOk;

    QComboBox *box_cameraFirst;
    QComboBox *box_cameraSecond;

    QSpinBox* spinBox_countFrame;

    QCheckBox* checkBox_isSnapShoot;

    QLineEdit* line_folderPath1;
    QLineEdit* line_folderPath2;

    QLabel* label_folderPath1;
    QLabel* label_folderPath2;
    QLabel* label_cam1;
    QLabel* label_cam2;

};

#endif // DIALOGWINDOWIMPORTIMAGE_H
