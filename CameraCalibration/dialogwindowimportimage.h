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

#include "filesystem.h"

class DialogWindowImportImage : public QWidget
{
    Q_OBJECT
public:
    DialogWindowImportImage(QWidget* parent = 0);
    ~DialogWindowImportImage();

    void setFileSystem(FileSystem* fs);

signals:
    void signalVideoStream(int numCum, int frameRate, int countframe, bool isSnapshoot);
private slots:
    void on_btnStartVideoStream_clicked();
    void on_btnSetFolderPath_clicked();
    void on_btnSetOk_clicked();
    void on_btnSetCancel_clicked();
    void on_tree_inputPatern_clicked(QTreeWidgetItem* item, int col);

private:
    FileSystem* fs;
    QString pathName;

    QPushButton* btnStartVideoStream;
    QPushButton* btnSetFolderPath;
    QPushButton* btnSetOk;
    QPushButton* btnSetCancel;

    QSpinBox* spinBox_numCamera;
    QSpinBox* spinBox_frameRate;
    QSpinBox* spinBox_countFrame;
    QSpinBox* spinBox_rowCount;
    QSpinBox* spinBox_colCount;
    QSpinBox* spinBox_checkerSize;
    QSpinBox* spinBox_markerSize;

    QCheckBox* checkBox_isPattern;
    QCheckBox* checkBox_isSnapShoot;

    QTreeWidget* tree_inputPatern;
    QTreeWidget* tree_inputDictionaryName;

    QLineEdit* line_folderPath;

    QLabel* label_row;
    QLabel* label_col;
    QLabel* label_checkerSize;
    QLabel* label_markerSize;
    QLabel* label_headerSelectFolder;
    QLabel* label_folderPath;
};

#endif // DIALOGWINDOWIMPORTIMAGE_H
