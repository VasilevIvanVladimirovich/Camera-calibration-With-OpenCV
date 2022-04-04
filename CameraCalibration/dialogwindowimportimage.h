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
    void signalVideoStream(int frameRate, int countframe, bool isSnapshoot);

private slots:
    void on_btnStartVideoStream_clicked();
    void on_btnSetFolderPath_clicked();
    void on_btnSetOk_clicked();
    void on_btnSetCancel_clicked();

private:
    FileSystem* fs;
    QString pathName;

    QPushButton* btnStartVideoStream;
    QPushButton* btnSetFolderPath;
    QPushButton* btnSetOk;
    QPushButton* btnSetCancel;

//    QSpinBox* spinBox_numCameraFirst;
//    QSpinBox* spinBox_numCameraSecond;
    QComboBox *box_cameraFirst;
    QComboBox *box_cameraSecond;

    QSpinBox* spinBox_frameRate;
    QSpinBox* spinBox_countFrame;

    QCheckBox* checkBox_isSnapShoot;

    QLineEdit* line_folderPath;

    QLabel* label_headerSelectFolder;
    QLabel* label_folderPath;
};

#endif // DIALOGWINDOWIMPORTIMAGE_H
