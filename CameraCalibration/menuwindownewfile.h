#ifndef MENUWINDOWNEWFILE_H
#define MENUWINDOWNEWFILE_H

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
#include <QSizePolicy>
#include <QLineEdit>
#include <QComboBox>
#include <QCameraDevice>
#include <QMediaDevices>

#include "filesystem.h"

class MenuWindowNewFile :public QWidget
{
    Q_OBJECT
public:
    MenuWindowNewFile(QWidget* parent = 0);
    ~MenuWindowNewFile();

signals:
    void setPathDir(QString path);
    void setParamentCalib(QString pattern, int row, int col,
                             double checkerSize,double markerSize,
                             QString dictionaryName);
private slots:
    void on_btnSetOk_clicked();
    void on_btnSetCancel_clicked();
    void on_btnSetFolderPath_clicked();
    void on_tree_inputPatern_clicked(QTreeWidgetItem* item, int col);
    void on_box_cameraFirst_IndexChanged(int index);
private:
    FileSystem fileSystem_;

    QString pathName;
    QString pattern;

    QTreeWidget* tree_inputDictionaryName;
    QTreeWidget* tree_inputPatern;

    QSpinBox* spinBox_rowCount;
    QSpinBox* spinBox_colCount;
    QSpinBox* spinBox_checkerSize;
    QSpinBox* spinBox_markerSize;

    QComboBox *box_cameraFirst;
    QComboBox *box_cameraSecond;

    QPushButton* btnSetOk;
    QPushButton* btnSetCancel;
    QPushButton* btnSetFolderPath;

    QLineEdit* line_path;
    QLineEdit* line_folderPath;

    QLabel* label_headerSelectFolder;
    QLabel* label_folderPath;
    QLabel* label_row;
    QLabel* label_col;
    QLabel* label_checkerSize;
    QLabel* label_markerSize;

};

#endif // MENUWINDOWNEWFILE_H
