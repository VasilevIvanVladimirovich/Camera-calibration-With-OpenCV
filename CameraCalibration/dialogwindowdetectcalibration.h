#ifndef DIALOGWINDOWDETECTCALIBRATION_H
#define DIALOGWINDOWDETECTCALIBRATION_H

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

#include "filesystem.h"


class DialogWindowDetectCalibration :public QWidget
{
    Q_OBJECT
public:
    DialogWindowDetectCalibration(QWidget* parent = 0);
    ~DialogWindowDetectCalibration();

    int translateFlags(QString textFlag);

    void setFileSystem(FileSystem* fs);

signals:
    void goCalib();

private slots:
    void on_btnSetOk_clicked();
    void on_btnCancel_clicked();
    void on_tree_inputPatern_clicked(QTreeWidgetItem* item, int col);
private:
    FileSystem* fs_;

    QPushButton* btnSetOk;
    QPushButton* btnCancel;

    QSpinBox* spinBox_rowCount;
    QSpinBox* spinBox_colCount;
    QSpinBox* spinBox_iterationSub;
    QSpinBox* spinBox_checkerSize;
    QSpinBox* spinBox_markerSize;

    QTreeWidget* tree_inputPatern;
    QTreeWidget* tree_inputDictionaryName;

    QListWidget* list_inputFlags;

    QLabel* label_row;
    QLabel* label_col;
    QLabel* label_subpix;
    QLabel* label_iteration;
    QLabel* label_checkerSize;
    QLabel* label_markerSize;
};

#endif // DIALOGWINDOWDETECTCALIBRATION_H
