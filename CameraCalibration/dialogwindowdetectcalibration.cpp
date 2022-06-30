#include "dialogwindowdetectcalibration.h"

DialogWindowDetectCalibration::DialogWindowDetectCalibration(QWidget* parent): QWidget(parent)
{
    initUi();
}
DialogWindowDetectCalibration::~DialogWindowDetectCalibration()
{

}

void DialogWindowDetectCalibration::updateUi()
{
    setting = fs_->getCalibrationSetting();
//    groupBoxSingleCalib->setChecked(setting.isCaibration);
//    groupBoxStereCalib->setChecked(setting.isStereoCaibration);
//    check_useParametr1->setChecked(setting.isUseParametr);
//    isfixedFocal1->setChecked(setting.isFixedFocal);
//    isfixedAspectRatio->setChecked(setting.isFixedAspectRatio);
//    isfixedPrincipalPoint1->setChecked(setting.isFixedPrincipalPoint);
//    isfixedK1->setChecked(setting.isfixedK1);
//    isfixedK2->setChecked(setting.isfixedK2);
//    isfixedK3->setChecked(setting.isfixedK3);
//    isfixedK4->setChecked(setting.isfixedK4);
//    iszeroTangent->setChecked(setting.iszeroTangent);

//    isUseSingleCalibratedInStereo->setChecked(setting.isUseSingleCalibratedInStereo);
//    isfixPrincipalPointStereo->setChecked(setting.isfixPrincipalPointStereo);
//    isfixFocalLengthStereo->setChecked(setting.isfixFocalLengthStereo);
//    isfixAspectRatioStereo->setChecked(setting.isfixAspectRatioStereo);
//    isSameFocalLengStereo->setChecked(setting.isSameFocalLengStereo);
//    iszeroTangentS->setChecked(setting.iszeroTangentS);
//    isfixedK1S->setChecked(setting.isfixedK1S);
//    isfixedK2S->setChecked(setting.isfixedK2S);
//    isfixedK3S->setChecked(setting.isfixedK3S);

    if(setting.cameraModel == FileSystem::SettingCalibration::OPENCV)
        box_cameraModel->setCurrentIndex(0);

    spin_fx->setValue(setting.fx);
    spin_fy->setValue(setting.fy);
    spin_cx->setValue(setting.cx);
    spin_cy->setValue(setting.cy);
    spin_K1->setValue(setting.k1);
    spin_K2->setValue(setting.k2);
    spin_K3->setValue(setting.k3);
    spin_K4->setValue(setting.k4);

    spin_Fx1->setValue(setting.fx1);
    spin_Cx1->setValue(setting.cx1);
    spin_Fx2->setValue(setting.fx2);
    spin_Cx2->setValue(setting.cx2);
    spin_Fy1->setValue(setting.fy1);
    spin_Cy1->setValue(setting.cy1);
    spin_Fy2->setValue(setting.fy2);
    spin_Cy2->setValue(setting.cy2);

    spin_K1S1->setValue(setting.k1S1);
    spin_K2S1->setValue(setting.k2S1);
    spin_K3S1->setValue(setting.k3S1);
    spin_K1S2->setValue(setting.k1S2);
    spin_K2S2->setValue(setting.k2S2);
    spin_K3S2->setValue(setting.k3S2);

}

void DialogWindowDetectCalibration::on_btnSetOk_clicked()
{
    FileSystem::SettingCalibration setting;

    setting.isCaibration = groupBoxSingleCalib->isChecked();
    setting.isStereoCaibration = groupBoxStereCalib->isChecked();
    setting.isUseParametr = check_useParametr1->isChecked();
    setting.isFixedFocal = isfixedFocal1->isChecked();
    setting.isFixedAspectRatio = isfixedAspectRatio->isChecked();
    setting.isFixedPrincipalPoint = isfixedPrincipalPoint1->isChecked();
    setting.isfixedK1 = isfixedK1->isChecked();
    setting.isfixedK2 = isfixedK2->isChecked();
    setting.isfixedK3 = isfixedK3->isChecked();
    setting.isfixedK4 = isfixedK4->isChecked();
    setting.iszeroTangent = iszeroTangent->isChecked();

    setting.isUseSingleCalibratedInStereo = isUseSingleCalibratedInStereo->isChecked();
    setting.isSameFocalLengStereo = isSameFocalLengStereo->isChecked();
    setting.isfixAspectRatioStereo = isfixAspectRatioStereo->isChecked();
    setting.isfixFocalLengthStereo = isfixFocalLengthStereo->isChecked();
    setting.isfixPrincipalPointStereo = isfixPrincipalPointStereo->isChecked();
    setting.iszeroTangentS = iszeroTangentS->isChecked();
    setting.isfixedK1S = isfixedK1S->isChecked();
    setting.isfixedK2S = isfixedK2S->isChecked();
    setting.isfixedK3S = isfixedK3S->isChecked();

    setting.numCamera = box_numCamera->currentIndex()+1;

    if(box_cameraModel->currentIndex() == 0)
         setting.cameraModel = FileSystem::SettingCalibration::OPENCV;
    else if(box_cameraModel->currentIndex() == 1)
        setting.cameraModel = FileSystem::SettingCalibration::OPENCV_FISHEYE;

    setting.fx = spin_fx->value();
    setting.fy = spin_fy->value();
    setting.cx = spin_cx->value();
    setting.cy = spin_cy->value();
    setting.k1 = spin_K1->value();
    setting.k2 = spin_K2->value();
    setting.k3 = spin_K3->value();
    setting.k4 = spin_K4->value();

    setting.fx1 = spin_Fx1->value();
    setting.fy1 = spin_Fy1->value();
    setting.fx2 = spin_Fx2->value();
    setting.fy2 = spin_Fy2->value();
    setting.cx1 = spin_Cx1->value();
    setting.cy1 = spin_Cy1->value();
    setting.cx2 = spin_Cx2->value();
    setting.cy2 = spin_Cy2->value();

    setting.k1S1 = spin_K1S1->value();
    setting.k2S1 = spin_K2S1->value();
    setting.k3S1 = spin_K3S1->value();
    setting.k1S2 = spin_K1S2->value();
    setting.k2S2 = spin_K2S2->value();
    setting.k3S2 = spin_K3S2->value();

    fs_->saveCalibrationSetting(setting);

    emit goCalib();
    close();
}

void DialogWindowDetectCalibration::on_btnCancel_clicked()
{
    close();
}

void DialogWindowDetectCalibration::on_box_numCamera_change(int numCam)
{
    fs_->saveCalibrationSetting(setting);
    fs_->parseResultCalibr(numCam+1);
    updateUi();
}

int DialogWindowDetectCalibration::translateFlags(QString textFlag)
{
    if(textFlag == "CV_CALIB_USE_INTRINSIC_GUESS") return 0x00001;
    if(textFlag == "CV_CALIB_FIX_ASPECT_RATIO") return 0x00002;
    if(textFlag == "CV_CALIB_FIX_PRINCIPAL_POINT") return 0x00004;
    if(textFlag == "CV_CALIB_ZERO_TANGENT_DIST") return 0x00008;
    if(textFlag == "CV_CALIB_FIX_FOCAL_LENGTH") return 0x00010;
    if(textFlag == "CV_CALIB_FIX_K1") return 0x00020;
    if(textFlag == "CV_CALIB_FIX_K2") return 0x00040;
    if(textFlag == "CV_CALIB_FIX_K3") return 0x00080;
    if(textFlag == "CV_CALIB_FIX_K4") return 0x00800;
    if(textFlag == "CV_CALIB_FIX_K5") return 0x01000;
    if(textFlag == "CV_CALIB_FIX_K6") return 0x02000;
    if(textFlag == "CV_CALIB_RATIONAL_MODEL") return 0x04000;
    if(textFlag == "CALIB_THIN_PRISM_MODEL") return 0x08000;
    if(textFlag == "CALIB_FIX_S1_S2_S3_S4") return 0x10000;
    if(textFlag == "CALIB_TILTED_MODEL") return 0x40000;
    if(textFlag == "CALIB_FIX_TAUX_TAUY") return 0x80000;
    else return 0;
}

void DialogWindowDetectCalibration::setFileSystem(FileSystem *fs)
{
    fs_ = fs;
    fs_->parseResultCalibr(1);
    updateUi();
}

void DialogWindowDetectCalibration::initUi()
{
    QVBoxLayout* layout_Vertical = new QVBoxLayout;

    QHBoxLayout* layout_main = new QHBoxLayout;

    layout_Vertical->addLayout(layout_main);

    groupBoxSingleCalib= new QGroupBox(tr("Single calibration"));
    groupBoxSingleCalib->setAlignment(Qt::AlignLeft);
    groupBoxSingleCalib->setCheckable(true);
    groupBoxSingleCalib->setChecked(false);


    groupBoxStereCalib = new QGroupBox(tr("Stereo calibration"));
    groupBoxStereCalib->setAlignment(Qt::AlignLeft);
    groupBoxStereCalib->setCheckable(true);
    groupBoxStereCalib->setChecked(false);

    layout_main->addWidget(groupBoxSingleCalib);
    layout_main->addWidget(groupBoxStereCalib);

    QVBoxLayout* layout_SingleCalib = new QVBoxLayout;

    QFormLayout *layout_CameraModel = new QFormLayout;
    label_cameraModel = new QLabel("Camera model");

    box_numCamera = new QComboBox();
    box_numCamera->addItem("First");
    box_numCamera->addItem("Second");

    connect(box_numCamera,SIGNAL(currentIndexChanged(int)),
            this,SLOT(on_box_numCamera_change(int)));

    layout_CameraModel->addRow("Сam",box_numCamera);

    box_cameraModel = new QComboBox();
    box_cameraModel->addItem("Opencv");
    //box_cameraModel->addItem("Opencv Fisheye");
    layout_CameraModel->addRow(label_cameraModel,box_cameraModel);

    layout_SingleCalib->addLayout(layout_CameraModel);

    QVBoxLayout* layout_mainForCamera1 = new QVBoxLayout;
    check_useParametr1 = new QCheckBox("Use existing parameters");
    layout_mainForCamera1->addWidget(check_useParametr1);

    QVBoxLayout* layout_mainForWidget = new QVBoxLayout;

    QGroupBox *groupBoxIntrinsic= new QGroupBox(tr("Intrinsic parameters"));
    groupBoxIntrinsic->setAlignment(Qt::AlignCenter);
    QGridLayout* layout_forIntrinsic1 = new QGridLayout;
    QLabel* label_fx = new QLabel("Fx");
    QLabel* label_fy = new QLabel("Fy");
    QLabel* label_cx = new QLabel("Cx");
    QLabel* label_cy = new QLabel("Cy");
    spin_fx = new QDoubleSpinBox();
    spin_fx->setMinimum(-10000);
    spin_fx->setMaximum(10000);
    spin_fy = new QDoubleSpinBox();
    spin_fy->setMinimum(-10000);
    spin_fy->setMaximum(10000);
    spin_cx = new QDoubleSpinBox();
    spin_cx->setMinimum(-10000);
    spin_cx->setMaximum(10000);
    spin_cy = new QDoubleSpinBox();
    spin_cy->setMinimum(-10000);
    spin_cy->setMaximum(10000);
    isfixedFocal1 = new QCheckBox("Fixed focal length");
    isfixedAspectRatio = new QCheckBox("Fixed aspect ratio");
    isfixedPrincipalPoint1 = new QCheckBox("Fixed principal point");

    layout_forIntrinsic1->addWidget(label_fx,0,0);
    layout_forIntrinsic1->addWidget(spin_fx,0,1);
    layout_forIntrinsic1->addWidget(isfixedFocal1,0,2);

    layout_forIntrinsic1->addWidget(label_fy,1,0);
    layout_forIntrinsic1->addWidget(spin_fy,1,1);
    layout_forIntrinsic1->addWidget(isfixedAspectRatio,1,2);

    layout_forIntrinsic1->addWidget(label_cx,2,0);
    layout_forIntrinsic1->addWidget(spin_cx,2,1);
    layout_forIntrinsic1->addWidget(isfixedPrincipalPoint1,2,2);

    layout_forIntrinsic1->addWidget(label_cy,3,0);
    layout_forIntrinsic1->addWidget(spin_cy,3,1);

    groupBoxIntrinsic->setLayout(layout_forIntrinsic1);

    layout_mainForWidget->addWidget(groupBoxIntrinsic);

    QHBoxLayout* layout_Extrinsic1 = new QHBoxLayout;
    QGroupBox *groupBoxRadial= new QGroupBox(tr("Radial distortion parameters"));
    QGridLayout* layout_forExtrRadial1 = new QGridLayout;

    QLabel* label_k1 = new QLabel("K1");
    QLabel* label_k2 = new QLabel("K2");
    QLabel* label_k3 = new QLabel("K3");
    QLabel* label_k4 = new QLabel("K4");

    spin_K1 = new QDoubleSpinBox();
    spin_K1->setMinimum(-10000);
    spin_K1->setMaximum(10000);
    spin_K2 = new QDoubleSpinBox();
    spin_K2->setMinimum(-10000);
    spin_K2->setMaximum(10000);
    spin_K3 = new QDoubleSpinBox();
    spin_K3->setMinimum(-10000);
    spin_K3->setMaximum(10000);
    spin_K4 = new QDoubleSpinBox();
    spin_K4->setMinimum(-10000);
    spin_K4->setMaximum(10000);

    isfixedK1 = new QCheckBox("Fixed K1");
    isfixedK2 = new QCheckBox("Fixed K2");
    isfixedK3 = new QCheckBox("Fixed K3");
    isfixedK4 = new QCheckBox("Fixed K4");

    layout_forExtrRadial1->addWidget(label_k1,0,0);
    layout_forExtrRadial1->addWidget(spin_K1,0,1);
    layout_forExtrRadial1->addWidget(isfixedK1,0,2);

    layout_forExtrRadial1->addWidget(label_k2,1,0);
    layout_forExtrRadial1->addWidget(spin_K2,1,1);
    layout_forExtrRadial1->addWidget(isfixedK2,1,2);

    layout_forExtrRadial1->addWidget(label_k3,2,0);
    layout_forExtrRadial1->addWidget(spin_K3,2,1);
    layout_forExtrRadial1->addWidget(isfixedK3,2,2);

    layout_forExtrRadial1->addWidget(label_k4,3,0);
    layout_forExtrRadial1->addWidget(spin_K4,3,1);
    layout_forExtrRadial1->addWidget(isfixedK4,3,2);

    groupBoxRadial->setLayout(layout_forExtrRadial1);

    layout_Extrinsic1->addWidget(groupBoxRadial);

    QGroupBox *groupBoxTangential= new QGroupBox(tr("Tangential distortion parameters"));
    QGridLayout* layout_forExtrTang1 = new QGridLayout;
    iszeroTangent = new QCheckBox("Ignore tangential distortion");
    layout_forExtrTang1->addWidget(iszeroTangent,0,0);

    groupBoxTangential->setLayout(layout_forExtrTang1);

    layout_Extrinsic1->addWidget(groupBoxTangential);

    layout_mainForWidget->addLayout(layout_Extrinsic1);

    parametr = new QWidget();
    parametr->setLayout(layout_mainForWidget);

    layout_mainForCamera1->addWidget(parametr);

    layout_SingleCalib->addLayout(layout_mainForCamera1);
    groupBoxSingleCalib->setLayout(layout_SingleCalib);


    QGridLayout *layout_Stereo = new QGridLayout;
    isUseSingleCalibratedInStereo = new QCheckBox("Use an existing calibration");
    layout_Stereo->addWidget(isUseSingleCalibratedInStereo, 0,0);
    QGroupBox *groupBoxIntrinsicStereo= new QGroupBox(tr("Intrinsic parameters"));
    layout_Stereo->addWidget(groupBoxIntrinsicStereo, 1,0);
    groupBoxIntrinsic->setAlignment(Qt::AlignCenter);
    QGridLayout* layout_forIntrinsicStereo = new QGridLayout;
    QLabel* label_F1 = new QLabel("F1");
    QLabel* label_C1 = new QLabel("C1");
    QLabel* label_F2 = new QLabel("F2");
    QLabel* label_C2 = new QLabel("C2");

    spin_Fx1 = new QDoubleSpinBox();
    spin_Fx1->setMinimum(-10000);
    spin_Fx1->setMaximum(10000);
    spin_Cx1 = new QDoubleSpinBox();
    spin_Cx1->setMinimum(-10000);
    spin_Cx1->setMaximum(10000);
    spin_Fx2 = new QDoubleSpinBox();
    spin_Fx2->setMinimum(-10000);
    spin_Fx2->setMaximum(10000);
    spin_Cx2 = new QDoubleSpinBox();
    spin_Cx2->setMinimum(-10000);
    spin_Cx2->setMaximum(10000);
    spin_Fy1 = new QDoubleSpinBox();
    spin_Fy1->setMinimum(-10000);
    spin_Fy1->setMaximum(10000);
    spin_Cy1 = new QDoubleSpinBox();
    spin_Cy1->setMinimum(-10000);
    spin_Cy1->setMaximum(10000);
    spin_Fy2 = new QDoubleSpinBox();
    spin_Fy2->setMinimum(-10000);
    spin_Fy2->setMaximum(10000);
    spin_Cy2 = new QDoubleSpinBox();
    spin_Cy2->setMinimum(-10000);
    spin_Cy2->setMaximum(10000);
    isfixPrincipalPointStereo = new QCheckBox("Fixed principal point");
    isfixFocalLengthStereo = new QCheckBox("Fixed focal length");
    isfixAspectRatioStereo = new QCheckBox("Fixed aspect ratio");
    isSameFocalLengStereo = new QCheckBox("Enforce Fx1=Fx2, Fy1=Fy2");

    layout_forIntrinsicStereo->addWidget(label_F1,0,0);
    layout_forIntrinsicStereo->addWidget(new QLabel("("),0,1);
    layout_forIntrinsicStereo->addWidget(spin_Fx1,0,2);
    layout_forIntrinsicStereo->addWidget(new QLabel(","),0,3);
    layout_forIntrinsicStereo->addWidget(spin_Fy1,0,4);
    layout_forIntrinsicStereo->addWidget(new QLabel(")"),0,5);
    layout_forIntrinsicStereo->addWidget(isfixFocalLengthStereo,0,6);

    layout_forIntrinsicStereo->addWidget(label_F2,1,0);
    layout_forIntrinsicStereo->addWidget(new QLabel("("),1,1);
    layout_forIntrinsicStereo->addWidget(spin_Fx2,1,2);
    layout_forIntrinsicStereo->addWidget(new QLabel(","),1,3);
    layout_forIntrinsicStereo->addWidget(spin_Fy2,1,4);
    layout_forIntrinsicStereo->addWidget(new QLabel(")"),1,5);
    layout_forIntrinsicStereo->addWidget(isSameFocalLengStereo,1,6);

    layout_forIntrinsicStereo->addWidget(label_C1,2,0);
    layout_forIntrinsicStereo->addWidget(new QLabel("("),2,1);
    layout_forIntrinsicStereo->addWidget(spin_Cx1,2,2);
    layout_forIntrinsicStereo->addWidget(new QLabel(","),2,3);
    layout_forIntrinsicStereo->addWidget(spin_Cy1,2,4);
    layout_forIntrinsicStereo->addWidget(new QLabel(")"),2,5);
    layout_forIntrinsicStereo->addWidget(isfixAspectRatioStereo,2,6);


    layout_forIntrinsicStereo->addWidget(label_C2,3,0);
    layout_forIntrinsicStereo->addWidget(new QLabel("("),3,1);
    layout_forIntrinsicStereo->addWidget(spin_Cx2,3,2);
    layout_forIntrinsicStereo->addWidget(new QLabel(","),3,3);
    layout_forIntrinsicStereo->addWidget(spin_Cy2,3,4);
    layout_forIntrinsicStereo->addWidget(new QLabel(")"),3,5);
    layout_forIntrinsicStereo->addWidget(isfixPrincipalPointStereo,3,6);

    groupBoxIntrinsicStereo->setLayout(layout_forIntrinsicStereo);

    QHBoxLayout* layout_ExtrinsicStereo = new QHBoxLayout;
    QGroupBox *groupBoxRadiaStereo= new QGroupBox(tr("Radial distortion parameters"));
    layout_Stereo->addWidget(groupBoxRadiaStereo, 2,0);
    QGridLayout* layout_forExtrRadialStereo = new QGridLayout;

    spin_K1S1 = new QDoubleSpinBox();
    spin_K1S1->setMinimum(-10000);
    spin_K1S1->setMaximum(10000);
    spin_K2S1 = new QDoubleSpinBox();
    spin_K2S1->setMinimum(-10000);
    spin_K2S1->setMaximum(10000);
    spin_K3S1 = new QDoubleSpinBox();
    spin_K3S1->setMinimum(-10000);
    spin_K3S1->setMaximum(10000);
    spin_K1S2 = new QDoubleSpinBox();
    spin_K1S2->setMinimum(-10000);
    spin_K1S2->setMaximum(10000);
    spin_K2S2 = new QDoubleSpinBox();
    spin_K2S2->setMinimum(-10000);
    spin_K2S2->setMaximum(10000);
    spin_K3S2 = new QDoubleSpinBox();
    spin_K3S2->setMinimum(-10000);
    spin_K3S2->setMaximum(10000);
    isfixedK1S = new QCheckBox("Fixed K1");
    isfixedK2S = new QCheckBox("Fixed K2");
    isfixedK3S = new QCheckBox("Fixed K3");

    layout_forExtrRadialStereo->addWidget(new QLabel("K1"),0,0);
    layout_forExtrRadialStereo->addWidget(spin_K1S1,0,1);
    layout_forExtrRadialStereo->addWidget(spin_K1S2,0,2);
    layout_forExtrRadialStereo->addWidget(isfixedK1S,0,3);

    layout_forExtrRadialStereo->addWidget(new QLabel("K2"),1,0);
    layout_forExtrRadialStereo->addWidget(spin_K2S1,1,1);
    layout_forExtrRadialStereo->addWidget(spin_K2S2,1,2);
    layout_forExtrRadialStereo->addWidget(isfixedK2S,1,3);

    layout_forExtrRadialStereo->addWidget(new QLabel("K3"),2,0);
    layout_forExtrRadialStereo->addWidget(spin_K3S1,2,1);
    layout_forExtrRadialStereo->addWidget(spin_K3S2,2,2);
    layout_forExtrRadialStereo->addWidget(isfixedK3S,2,3);

    groupBoxRadiaStereo->setLayout(layout_forExtrRadialStereo);

    layout_ExtrinsicStereo->addWidget(groupBoxRadiaStereo);

    QGroupBox *groupBoxTangentialS = new QGroupBox(tr("Tangential distortion parameters"));
    layout_Stereo->addWidget(groupBoxTangentialS, 2,1);
    QGridLayout* layout_TangentialS = new QGridLayout;
    iszeroTangentS = new QCheckBox("Ignore tangential distortion");
    layout_TangentialS->addWidget(iszeroTangentS,0,0);
    groupBoxTangentialS->setLayout(layout_TangentialS);

    layout_ExtrinsicStereo->addWidget(groupBoxTangentialS);



    groupBoxStereCalib->setLayout(layout_Stereo);

    btnSetOk = new QPushButton("Ok");
    connect(btnSetOk, SIGNAL(clicked()), this, SLOT(on_btnSetOk_clicked()));
    btnCancel = new QPushButton("Cancel");
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(on_btnCancel_clicked()));

    QHBoxLayout* layout_Button = new QHBoxLayout;
    layout_Button->addWidget(btnSetOk);
    layout_Button->addWidget(btnCancel);

    QWidget* buttonWidget = new QWidget();
    buttonWidget->setLayout(layout_Button);
    layout_Vertical->addWidget(buttonWidget,0, Qt::AlignRight);

    spin_fx->setDecimals(7);
    spin_fy->setDecimals(7);
    spin_cx->setDecimals(7);
    spin_cy->setDecimals(7);
    spin_K1->setDecimals(7);
    spin_K2->setDecimals(7);
    spin_K3->setDecimals(7);
    spin_K4->setDecimals(7);

    spin_K1S1->setDecimals(7);
    spin_K2S1->setDecimals(7);
    spin_K3S1->setDecimals(7);
    spin_K1S2->setDecimals(7);
    spin_K2S2->setDecimals(7);
    spin_K3S2->setDecimals(7);

    spin_Fx1->setDecimals(7);
    spin_Cx1->setDecimals(7);
    spin_Fx2->setDecimals(7);
    spin_Cx2->setDecimals(7);
    spin_Fy1->setDecimals(7);
    spin_Cy1->setDecimals(7);
    spin_Fy2->setDecimals(7);
    spin_Cy2->setDecimals(7);

    layout_Vertical->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(layout_Vertical);
}

void DialogWindowDetectCalibration::on_tree_inputPatern_clicked(QTreeWidgetItem *item, int col)
{
//    label_row->hide();
//    label_col->hide();
//    spinBox_rowCount->hide();
//    spinBox_colCount->hide();

//    label_markerSize->hide();
//    label_checkerSize->hide();
//    spinBox_markerSize->hide();
//    spinBox_checkerSize->hide();
//    tree_inputDictionaryName->hide();

    QString pattern = item->text(0);
    if(pattern == "Chessboard" || pattern == "Circles" || pattern == "Assymetric Circles")
    {
//        label_row->show();
//        label_col->show();
//        spinBox_rowCount->show();
//        spinBox_colCount->show();
    }
    if(pattern == "ChArUco")
    {
//        label_row->show();
//        label_col->show();
//        spinBox_rowCount->show();
//        spinBox_colCount->show();

//        label_markerSize->show();
//        label_checkerSize->show();
//        spinBox_markerSize->show();
//        spinBox_checkerSize->show();
//        tree_inputDictionaryName->show();
    }
}
