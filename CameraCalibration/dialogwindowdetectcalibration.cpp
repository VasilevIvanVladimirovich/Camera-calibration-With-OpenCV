#include "dialogwindowdetectcalibration.h"

DialogWindowDetectCalibration::DialogWindowDetectCalibration(QWidget* parent): QWidget(parent)
{

    initUi();
    //resize(1280, 800);



//    btnSetOk = new QPushButton("Ok");
//    btnCancel = new QPushButton("Cancel");

//    connect(btnSetOk, SIGNAL(clicked()), this, SLOT(on_btnSetOk_clicked()));
//    connect(btnCancel, SIGNAL(clicked()), this, SLOT(on_btnCancel_clicked()));

//    QVBoxLayout* layout_menu = new QVBoxLayout;

//    isCalib = new QCheckBox("Calibration");
//    isStereoCalib = new QCheckBox("StereoCalibration");

//    connect(isCalib,SIGNAL(stateChanged(int)),this,SLOT(on_isCalib_stateChanged(int)));
//    connect(isStereoCalib,SIGNAL(stateChanged(int)),this,SLOT(on_isStereoCalib_stateChanged(int)));

//    QHBoxLayout* layout_inputflags = new QHBoxLayout;

//    QVBoxLayout* layout_firstFlags = new QVBoxLayout;
//    layout_firstFlags->addWidget(isCalib);
//    list_inputFlags = new QListWidget;
//    list_inputFlags->setSelectionMode(QAbstractItemView::MultiSelection);
//    list_inputFlags->addItem("CV_CALIB_USE_INTRINSIC_GUESS");
//    list_inputFlags->addItem("CV_CALIB_FIX_PRINCIPAL_POINT");
//    list_inputFlags->addItem("CV_CALIB_FIX_ASPECT_RATIO");
//    list_inputFlags->addItem("CV_CALIB_ZERO_TANGENT_DIST");
//    list_inputFlags->addItem("CV_CALIB_FIX_K1");
//    list_inputFlags->addItem("CV_CALIB_FIX_K2");
//    list_inputFlags->addItem("CV_CALIB_FIX_K3");
//    list_inputFlags->addItem("CV_CALIB_FIX_K4");
//    list_inputFlags->addItem("CV_CALIB_FIX_K5");
//    list_inputFlags->addItem("CV_CALIB_FIX_K6");
//    list_inputFlags->addItem("CV_CALIB_RATIONAL_MODEL");
//    list_inputFlags->addItem("CALIB_THIN_PRISM_MODEL");
//    list_inputFlags->addItem("CALIB_FIX_S1_S2_S3_S4");
//    list_inputFlags->addItem("CALIB_TILTED_MODEL");
//    list_inputFlags->addItem("CALIB_FIX_TAUX_TAUY");
//    list_inputFlags->setEnabled(false);
//    layout_firstFlags->addWidget(list_inputFlags);
//    layout_inputflags->addLayout(layout_firstFlags);


//    QVBoxLayout* layout_SecondFlags = new QVBoxLayout;
//    layout_SecondFlags->addWidget(isStereoCalib);
//    list_inputFlagsStereo = new QListWidget;
//    list_inputFlagsStereo->setSelectionMode(QAbstractItemView::MultiSelection);
//    list_inputFlagsStereo->addItem("CALIB_FIX_INTRINSIC");
//    list_inputFlagsStereo->addItem("CALIB_USE_INTRINSIC_GUESS");
//    list_inputFlagsStereo->addItem("CALIB_USE_EXTRINSIC_GUESS");
//    list_inputFlagsStereo->addItem("CALIB_FIX_PRINCIPAL_POINT ");
//    list_inputFlagsStereo->addItem("CALIB_FIX_FOCAL_LENGTH");
//    list_inputFlagsStereo->addItem("CALIB_FIX_ASPECT_RATIO");
//    list_inputFlagsStereo->addItem("CALIB_SAME_FOCAL_LENGTH");
//    list_inputFlagsStereo->addItem("CALIB_ZERO_TANGENT_DIST");
//    list_inputFlagsStereo->addItem("CALIB_FIX_K1,..., CALIB_FIX_K6");
//    list_inputFlagsStereo->addItem("CALIB_RATIONAL_MODEL");
//    list_inputFlagsStereo->addItem("CALIB_THIN_PRISM_MODEL");
//    list_inputFlagsStereo->addItem("CALIB_FIX_S1_S2_S3_S4");
//    list_inputFlagsStereo->addItem("CALIB_TILTED_MODEL");
//    list_inputFlagsStereo->addItem("CALIB_FIX_TAUX_TAUY");
//    list_inputFlagsStereo->setEnabled(false);
//    layout_SecondFlags->addWidget(list_inputFlagsStereo);
//    layout_inputflags->addLayout(layout_SecondFlags);

//    QVBoxLayout* layout_subpix = new QVBoxLayout;
//    label_subpix = new QLabel;
//    label_subpix->setText("Subpixel");
//    label_iteration = new QLabel;
//    label_iteration->setText("Iteration");

//    spinBox_iterationSub = new QSpinBox;

//    layout_subpix->addWidget(label_subpix);
//    layout_subpix->addWidget(label_iteration);
//    layout_subpix->addWidget(spinBox_iterationSub);

//    layout_menu->addLayout(layout_inputflags);
//    layout_menu->addLayout(layout_subpix);
//    QHBoxLayout* layout_btn = new QHBoxLayout;
//    layout_btn->addWidget(btnSetOk);
//    layout_btn->addWidget(btnCancel);
//    layout_menu->addLayout(layout_btn);

//    QHBoxLayout* layout_main = new QHBoxLayout;   //Горизонтальный
//    layout_main->addLayout(layout_menu);

//    setLayout(layout_main);
}
DialogWindowDetectCalibration::~DialogWindowDetectCalibration()
{

}

void DialogWindowDetectCalibration::on_btnSetOk_clicked()
{
    QList<QListWidgetItem *> selected_itemsFirst ;
    selected_itemsFirst  = list_inputFlags->selectedItems();
    int flagsFirst = 0;
    QString flagsNameFirst = "";
    for(int i = 0; i < selected_itemsFirst .size(); i ++)
    {
        flagsNameFirst += selected_itemsFirst[i]->text() + "/n";
        flagsFirst = flagsFirst | translateFlags(selected_itemsFirst[i]->text());
    }

    QList<QListWidgetItem *> selected_itemsSecond ;
    selected_itemsSecond  = list_inputFlags->selectedItems();
    int flagsSecond = 0;
    QString flagsNameSecond = "";
    for(int i = 0; i < selected_itemsSecond .size(); i ++)
    {
        flagsNameSecond += selected_itemsSecond[i]->text() + "/n";
        //flags = flags | translateFlags(selected_itemsSecond[i]->text());
    }

    bool isCalibration = isCalib->checkState();
    bool isStereoCalibration = isStereoCalib->checkState();
    ///fs_->writeSettingCalibInYaml(spinBox_iterationSub->value(), flagsFirst, flagsNameFirst,
     //                            flagsSecond, flagsNameSecond,isCalibration,isStereoCalibration);
    emit goCalib();
    close();
}

void DialogWindowDetectCalibration::on_btnCancel_clicked()
{
    close();
}

int DialogWindowDetectCalibration::translateFlags(QString textFlag)
{
    if(textFlag == "CV_CALIB_USE_INTRINSIC_GUESS") return 0x00001;
    if(textFlag == "CV_CALIB_FIX_ASPECT_RATIO") return 0x00002;
    if(textFlag == "CV_CALIB_FIX_PRINCIPAL_POINT") return 0x00004;
    if(textFlag == "CV_CALIB_ZERO_TANGENT_DIST") return 0x00008;
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
}

void DialogWindowDetectCalibration::initUi()
{
    resize(1280, 800);
    QHBoxLayout* layout_main = new QHBoxLayout;

    QGroupBox *groupBoxSingleCalib= new QGroupBox(tr("Single calibration"));
    groupBoxSingleCalib->setAlignment(Qt::AlignCenter);

    QGroupBox *groupBoxStereCalib = new QGroupBox(tr("Stereo calibration"));
    groupBoxStereCalib->setAlignment(Qt::AlignCenter);

    layout_main->addWidget(groupBoxSingleCalib);
    layout_main->addWidget(groupBoxStereCalib);

    QVBoxLayout* layout_SingleCalib = new QVBoxLayout;

    QFormLayout *layout_CameraModel = new QFormLayout;
    label_cameraModel = new QLabel("Camera model");

    box_numCamera = new QComboBox();
    box_numCamera->addItem("First camera");
    box_numCamera->addItem("Second camera");

    layout_CameraModel->addRow("Сamera for calibration",box_numCamera);

    box_cameraModel = new QComboBox();
    box_cameraModel->addItem("Opencv");
    box_cameraModel->addItem("Opencv Fisheye");
    layout_CameraModel->addRow(label_cameraModel,box_cameraModel);

    layout_SingleCalib->addLayout(layout_CameraModel);

    QVBoxLayout* layout_mainForCamera1 = new QVBoxLayout;
    check_useParametr1 = new QCheckBox("use existing parameters");
    layout_mainForCamera1->addWidget(check_useParametr1);

    QVBoxLayout* layout_mainForWidget = new QVBoxLayout;

    QGroupBox *groupBoxIntrinsic= new QGroupBox(tr("Intrinsic parametr"));
    groupBoxIntrinsic->setAlignment(Qt::AlignCenter);
    QGridLayout* layout_forIntrinsic1 = new QGridLayout;
    QLabel* label_fx = new QLabel("Fx");
    QLabel* label_fy = new QLabel("Fy");
    QLabel* label_cx = new QLabel("Cx");
    QLabel* label_cy = new QLabel("Cy");
    spin_fx = new QDoubleSpinBox();
    spin_fx->setMaximum(10000);
    spin_fy = new QDoubleSpinBox();
    spin_fy->setMaximum(10000);
    spin_cx = new QDoubleSpinBox();
    spin_cx->setMaximum(10000);
    spin_cy = new QDoubleSpinBox();
    spin_cy->setMaximum(10000);
    isfixedFocal1 = new QCheckBox("Fixed focal");
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
    QGroupBox *groupBoxRadial= new QGroupBox(tr("Radial distortion parametr"));
    QGridLayout* layout_forExtrRadial1 = new QGridLayout;

    QLabel* label_k1 = new QLabel("K1");
    QLabel* label_k2 = new QLabel("K2");
    QLabel* label_k3 = new QLabel("K3");
    QLabel* label_k4 = new QLabel("K4");

    spin_K1 = new QDoubleSpinBox();
    spin_K1->setMaximum(10000);
    spin_K2 = new QDoubleSpinBox();
    spin_K2->setMaximum(10000);
    spin_K3 = new QDoubleSpinBox();
    spin_K3->setMaximum(10000);
    spin_K4 = new QDoubleSpinBox();
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


    QGroupBox *groupBoxTangential= new QGroupBox(tr("Tangential distortion parametr"));
    QLabel* label_tang1 = new QLabel("If the camera is of good quality, turn off");
    iszeroTangent = new QCheckBox("Ignore tangential distortion");
    QGridLayout* layout_forExtrTang1 = new QGridLayout;

    layout_forExtrTang1->addWidget(label_tang1,0,0);
    layout_forExtrTang1->addWidget(iszeroTangent,1,0);

    groupBoxTangential->setLayout(layout_forExtrTang1);

    layout_Extrinsic1->addWidget(groupBoxTangential);


    layout_mainForWidget->addLayout(layout_Extrinsic1);

    parametr = new QWidget();
    parametr->setLayout(layout_mainForWidget);

    layout_mainForCamera1->addWidget(parametr);

    layout_SingleCalib->addLayout(layout_mainForCamera1);
    groupBoxSingleCalib->setLayout(layout_SingleCalib);


    QGridLayout *layout_Stereo = new QGridLayout;
    isUseSingleCalibrated = new QCheckBox("Use a ready-made single calibration");
//    isfixIntrinsic = new QCheckBox();
//    isfixExtrinsic = new QCheckBox();
//    isfixPrincipalPoint = new QCheckBox();
//    isfixFocalLength = new QCheckBox();
//    isfixAspectRatio = new QCheckBox();
//    isSameFocalLeng = new QCheckBox();
//    iszeroTangentStereo = new QCheckBox();

    layout_Stereo->addWidget(isUseSingleCalibrated, 0,0);

    groupBoxStereCalib->setLayout(layout_Stereo);

    setLayout(layout_main);
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

void DialogWindowDetectCalibration::on_isCalib_stateChanged(int state)
{
    list_inputFlags->setEnabled(state);
}

void DialogWindowDetectCalibration::on_isStereoCalib_stateChanged(int state)
{
    list_inputFlagsStereo->setEnabled(state);
}
