#include "dialogwindowdetectcalibration.h"

DialogWindowDetectCalibration::DialogWindowDetectCalibration(QWidget* parent): QWidget(parent)
{
    resize(1280, 800);
    btnSetOk = new QPushButton("Ok");
    btnCancel = new QPushButton("Cancel");

    connect(btnSetOk, SIGNAL(clicked()), this, SLOT(on_btnSetOk_clicked()));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(on_btnCancel_clicked()));

    QVBoxLayout* layout_menu = new QVBoxLayout;

    isCalib = new QCheckBox("Calibration");
    isStereoCalib = new QCheckBox("StereoCalibration");

    connect(isCalib,SIGNAL(stateChanged(int)),this,SLOT(on_isCalib_stateChanged(int)));
    connect(isStereoCalib,SIGNAL(stateChanged(int)),this,SLOT(on_isStereoCalib_stateChanged(int)));

    QHBoxLayout* layout_inputflags = new QHBoxLayout;

    QVBoxLayout* layout_firstFlags = new QVBoxLayout;
    layout_firstFlags->addWidget(isCalib);
    list_inputFlags = new QListWidget;
    list_inputFlags->setSelectionMode(QAbstractItemView::MultiSelection);
    list_inputFlags->addItem("CV_CALIB_USE_INTRINSIC_GUESS");
    list_inputFlags->addItem("CV_CALIB_FIX_PRINCIPAL_POINT");
    list_inputFlags->addItem("CV_CALIB_FIX_ASPECT_RATIO");
    list_inputFlags->addItem("CV_CALIB_ZERO_TANGENT_DIST");
    list_inputFlags->addItem("CV_CALIB_FIX_K1");
    list_inputFlags->addItem("CV_CALIB_FIX_K2");
    list_inputFlags->addItem("CV_CALIB_FIX_K3");
    list_inputFlags->addItem("CV_CALIB_FIX_K4");
    list_inputFlags->addItem("CV_CALIB_FIX_K5");
    list_inputFlags->addItem("CV_CALIB_FIX_K6");
    list_inputFlags->addItem("CV_CALIB_RATIONAL_MODEL");
    list_inputFlags->addItem("CALIB_THIN_PRISM_MODEL");
    list_inputFlags->addItem("CALIB_FIX_S1_S2_S3_S4");
    list_inputFlags->addItem("CALIB_TILTED_MODEL");
    list_inputFlags->addItem("CALIB_FIX_TAUX_TAUY");
    list_inputFlags->setEnabled(false);
    layout_firstFlags->addWidget(list_inputFlags);
    layout_inputflags->addLayout(layout_firstFlags);


    QVBoxLayout* layout_SecondFlags = new QVBoxLayout;
    layout_SecondFlags->addWidget(isStereoCalib);
    list_inputFlagsStereo = new QListWidget;
    list_inputFlagsStereo->setSelectionMode(QAbstractItemView::MultiSelection);
    list_inputFlagsStereo->addItem("CALIB_FIX_INTRINSIC");
    list_inputFlagsStereo->addItem("CALIB_USE_INTRINSIC_GUESS");
    list_inputFlagsStereo->addItem("CALIB_USE_EXTRINSIC_GUESS");
    list_inputFlagsStereo->addItem("CALIB_FIX_PRINCIPAL_POINT ");
    list_inputFlagsStereo->addItem("CALIB_FIX_FOCAL_LENGTH");
    list_inputFlagsStereo->addItem("CALIB_FIX_ASPECT_RATIO");
    list_inputFlagsStereo->addItem("CALIB_SAME_FOCAL_LENGTH");
    list_inputFlagsStereo->addItem("CALIB_ZERO_TANGENT_DIST");
    list_inputFlagsStereo->addItem("CALIB_FIX_K1,..., CALIB_FIX_K6");
    list_inputFlagsStereo->addItem("CALIB_RATIONAL_MODEL");
    list_inputFlagsStereo->addItem("CALIB_THIN_PRISM_MODEL");
    list_inputFlagsStereo->addItem("CALIB_FIX_S1_S2_S3_S4");
    list_inputFlagsStereo->addItem("CALIB_TILTED_MODEL");
    list_inputFlagsStereo->addItem("CALIB_FIX_TAUX_TAUY");
    list_inputFlagsStereo->setEnabled(false);
    layout_SecondFlags->addWidget(list_inputFlagsStereo);
    layout_inputflags->addLayout(layout_SecondFlags);

    QVBoxLayout* layout_subpix = new QVBoxLayout;
    label_subpix = new QLabel;
    label_subpix->setText("Subpixel");
    label_iteration = new QLabel;
    label_iteration->setText("Iteration");

    spinBox_iterationSub = new QSpinBox;

    layout_subpix->addWidget(label_subpix);
    layout_subpix->addWidget(label_iteration);
    layout_subpix->addWidget(spinBox_iterationSub);

    layout_menu->addLayout(layout_inputflags);
    layout_menu->addLayout(layout_subpix);
    QHBoxLayout* layout_btn = new QHBoxLayout;
    layout_btn->addWidget(btnSetOk);
    layout_btn->addWidget(btnCancel);
    layout_menu->addLayout(layout_btn);

    QHBoxLayout* layout_main = new QHBoxLayout;   //Горизонтальный
    layout_main->addLayout(layout_menu);

    setLayout(layout_main);
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
    fs_->writeSettingCalibInYaml(spinBox_iterationSub->value(), flagsFirst, flagsNameFirst,
                                 flagsSecond, flagsNameSecond,isCalibration,isStereoCalibration);
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
