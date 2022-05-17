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

private:
    void initUi();

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

    //QSpinBox* spinBox_iterationSub;
    QDoubleSpinBox* spin_fx;
    QDoubleSpinBox* spin_fy;
    QDoubleSpinBox* spin_cx;
    QDoubleSpinBox* spin_cy;

    QWidget* parametr;

    QDoubleSpinBox* spin_K1;
    QDoubleSpinBox* spin_K2;
    QDoubleSpinBox* spin_K3;
    QDoubleSpinBox* spin_K4;

    QCheckBox* isCalib;
    QCheckBox* isStereoCalib;

    QListWidget* list_inputFlags;
    QListWidget* list_inputFlagsStereo;

    QComboBox* box_cameraModel;
    QComboBox* box_numCamera;

    QCheckBox* check_useParametr1;

    QCheckBox* isfixedFocal1;
    QCheckBox* isfixedAspectRatio;
    QCheckBox* isfixedPrincipalPoint1;

    QCheckBox* isfixedK1;
    QCheckBox* isfixedK2;
    QCheckBox* isfixedK3;
    QCheckBox* isfixedK4;
    QCheckBox* iszeroTangent;

    QCheckBox* isUseSingleCalibrated;
    QCheckBox* isfixIntrinsic;
    QCheckBox* isfixExtrinsic;
    QCheckBox* isfixPrincipalPoint;
    QCheckBox* isfixFocalLength;
    QCheckBox* isfixAspectRatio;
    QCheckBox* isSameFocalLeng;
    QCheckBox* iszeroTangentStereo;

    QTabWidget* tab_cameraSingleCalibration;

    QLabel* label_cameraModel;
    QLabel* label_subpix;
    QLabel* label_iteration;

    int camSecond_;
    int camFirst_;
};

#endif // DIALOGWINDOWDETECTCALIBRATION_H
