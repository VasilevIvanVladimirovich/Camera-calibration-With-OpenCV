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
    void on_isCalib_stateChanged(int state);
    void on_isStereoCalib_stateChanged(int state);
private:
    FileSystem* fs_;

    QPushButton* btnSetOk;
    QPushButton* btnCancel;

    QSpinBox* spinBox_iterationSub;

    QCheckBox* isCalib;
    QCheckBox* isStereoCalib;

    QListWidget* list_inputFlags;
    QListWidget* list_inputFlagsStereo;

    QLabel* label_subpix;
    QLabel* label_iteration;

    int camSecond_;
    int camFirst_;
};

#endif // DIALOGWINDOWDETECTCALIBRATION_H
