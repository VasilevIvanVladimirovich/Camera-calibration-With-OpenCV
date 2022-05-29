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
    void updateUi();

private slots:
    void on_btnSetOk_clicked();
    void on_btnCancel_clicked();

    void on_box_numCamera_change(int);
    void on_tree_inputPatern_clicked(QTreeWidgetItem* item, int col);
private:
    FileSystem* fs_;

    QPushButton* btnSetOk;
    QPushButton* btnCancel;

    QDoubleSpinBox* spin_fx;
    QDoubleSpinBox* spin_fy;
    QDoubleSpinBox* spin_cx;
    QDoubleSpinBox* spin_cy;

    QDoubleSpinBox* spin_Fx1;
    QDoubleSpinBox* spin_Cx1;
    QDoubleSpinBox* spin_Fx2;
    QDoubleSpinBox* spin_Cx2;
    QDoubleSpinBox* spin_Fy1;
    QDoubleSpinBox* spin_Cy1;
    QDoubleSpinBox* spin_Fy2;
    QDoubleSpinBox* spin_Cy2;

    QWidget* parametr;

    FileSystem::SettingCalibration setting;

    QDoubleSpinBox* spin_K1;
    QDoubleSpinBox* spin_K2;
    QDoubleSpinBox* spin_K3;
    QDoubleSpinBox* spin_K4;

    QDoubleSpinBox* spin_K1S1;
    QDoubleSpinBox* spin_K2S1;
    QDoubleSpinBox* spin_K3S1;
    QDoubleSpinBox* spin_K1S2;
    QDoubleSpinBox* spin_K2S2;
    QDoubleSpinBox* spin_K3S2;
    QCheckBox* iszeroTangentS;

    QGroupBox *groupBoxSingleCalib;
    QGroupBox *groupBoxStereCalib;

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
    QCheckBox* isfixedK1S;
    QCheckBox* isfixedK2S;
    QCheckBox* isfixedK3S;

    QCheckBox* isUseSingleCalibratedInStereo;
    QCheckBox* isfixPrincipalPointStereo;
    QCheckBox* isfixFocalLengthStereo;
    QCheckBox* isfixAspectRatioStereo;
    QCheckBox* isSameFocalLengStereo;

    QTabWidget* tab_cameraSingleCalibration;

    QLabel* label_cameraModel;
    QLabel* label_subpix;
    QLabel* label_iteration;

    int camSecond_;
    int camFirst_;
};

#endif // DIALOGWINDOWDETECTCALIBRATION_H
