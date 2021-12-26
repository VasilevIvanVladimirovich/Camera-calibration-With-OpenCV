#include "dialogwindowdetectcalibration.h"

DialogWindowDetectCalibration::DialogWindowDetectCalibration(QWidget* parent): QWidget(parent)
{
    resize(1280, 800);
    btnSetOk = new QPushButton("Ok");
    btnCancel = new QPushButton("Cancel");

    connect(btnSetOk, SIGNAL(clicked()), this, SLOT(on_btnSetOk_clicked()));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(on_btnCancel_clicked()));


    QVBoxLayout* layout_menu = new QVBoxLayout;
    QHBoxLayout* layout_patternSetting = new QHBoxLayout;

    tree_inputPatern = new QTreeWidget;
    tree_inputPatern->setColumnCount(1);
    tree_inputPatern->setHeaderLabels(QStringList() << "Pattern");
    QList<QTreeWidgetItem *> items;
    items.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("Chessboard"))));
    items.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("Circles"))));
    items.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("Assymetric Circles"))));
    items.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("ChArUco"))));
    tree_inputPatern->insertTopLevelItems(0, items);

    connect(tree_inputPatern,SIGNAL(itemClicked(QTreeWidgetItem*, int)),this,SLOT(on_tree_inputPatern_clicked(QTreeWidgetItem*, int)));

    layout_patternSetting->addWidget(tree_inputPatern);


    QHBoxLayout* layout_rowColPatternSetting = new QHBoxLayout;
    QVBoxLayout* layout_rowPatternSetting = new QVBoxLayout;
    QVBoxLayout* layout_checkerSizeSetting = new QVBoxLayout;
    QVBoxLayout* layout_markerSizeSetting = new QVBoxLayout;
    QVBoxLayout* layout_dictionarySetting = new QVBoxLayout;

    label_row = new QLabel;
    label_row->setText("Row");

    spinBox_rowCount = new QSpinBox;
    layout_rowPatternSetting->addWidget(label_row);
    layout_rowPatternSetting->addWidget(spinBox_rowCount);

    QVBoxLayout* layout_colPatternSetting = new QVBoxLayout;
    label_col = new QLabel;
    label_col->setText("Column");

    spinBox_colCount = new QSpinBox;
    layout_colPatternSetting->addWidget(label_col);
    layout_colPatternSetting->addWidget(spinBox_colCount);

    label_checkerSize = new QLabel;
    label_checkerSize->setText("Checker Size (mm):");
    spinBox_checkerSize = new QSpinBox;
    layout_checkerSizeSetting->addWidget(label_checkerSize);
    layout_checkerSizeSetting->addWidget(spinBox_checkerSize);

    label_markerSize = new QLabel;
    label_markerSize->setText("Marker Size (mm):");
    spinBox_markerSize = new QSpinBox;
    layout_markerSizeSetting->addWidget(label_markerSize);
    layout_markerSizeSetting->addWidget(spinBox_markerSize);

    tree_inputDictionaryName = new QTreeWidget;
    tree_inputDictionaryName->setColumnCount(1);
    tree_inputDictionaryName->setHeaderLabels(QStringList() << "Dictionary");
    QList<QTreeWidgetItem *> items_DictionaryName;
    items_DictionaryName.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("DICT_4X4_50"))));
    items_DictionaryName.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("DICT_4X4_100"))));
    items_DictionaryName.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("DICT_4X4_1000"))));
    items_DictionaryName.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("DICT_5X5_50"))));
    items_DictionaryName.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("DICT_5X5_100"))));
    items_DictionaryName.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("DICT_5X5_250"))));
    items_DictionaryName.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("DICT_5X5_1000"))));
    items_DictionaryName.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("DICT_6X6_50"))));
    items_DictionaryName.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("DICT_6X6_100"))));
    items_DictionaryName.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("DICT_6X6_250"))));
    items_DictionaryName.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("DICT_6X6_1000"))));
    items_DictionaryName.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("DICT_7X7_50"))));
    items_DictionaryName.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("DICT_7X7_100"))));
    items_DictionaryName.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("DICT_7X7_250"))));
    items_DictionaryName.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("DICT_7X7_1000"))));
    items_DictionaryName.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("DICT_ARUCO_ORIGINAL"))));
    items_DictionaryName.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("DICT_APRILTAG_16h5"))));
    items_DictionaryName.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("DICT_APRILTAG_25h9"))));
    items_DictionaryName.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("DICT_APRILTAG_36h10"))));
    tree_inputDictionaryName->insertTopLevelItems(0, items_DictionaryName);
    layout_dictionarySetting->addWidget(tree_inputDictionaryName);

    layout_rowColPatternSetting->addLayout(layout_rowPatternSetting);
    layout_rowColPatternSetting->addLayout(layout_colPatternSetting);
    layout_rowColPatternSetting->addLayout(layout_checkerSizeSetting);
    layout_rowColPatternSetting->addLayout(layout_markerSizeSetting);
    layout_rowColPatternSetting->addLayout(layout_dictionarySetting);

    //layout_menu->addLayout(layout_patternSetting);
    //layout_menu->addLayout(layout_rowColPatternSetting);

    //
    label_row->hide();
    label_col->hide();
    spinBox_rowCount->hide();
    spinBox_colCount->hide();

    label_markerSize->hide();
    label_checkerSize->hide();
    spinBox_markerSize->hide();
    spinBox_checkerSize->hide();
    tree_inputDictionaryName->hide();
    //
    QVBoxLayout* layout_inputflags = new QVBoxLayout;

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
    layout_inputflags->addWidget(list_inputFlags);

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
//    double checkerSize = spinBox_checkerSize->value();
//    double markerSize = spinBox_markerSize->value();

//    checkerSize = checkerSize * 0.001;
//    markerSize = markerSize * 0.001;

//    QString pattern;
//    QString dictionaryName;
//    QTreeWidgetItem* item_tree_inputPatern;
//    QTreeWidgetItem* item_tree_inputDictionaryName;
//    item_tree_inputPatern = tree_inputPatern->currentItem();
//    item_tree_inputDictionaryName = tree_inputDictionaryName->currentItem();
//    if(item_tree_inputPatern == NULL) pattern = "NULL";
//    else {
//        pattern = item_tree_inputPatern->text(0);
//    }

//    if(item_tree_inputDictionaryName == NULL) dictionaryName = "NULL";
//    else {
//        dictionaryName = item_tree_inputDictionaryName->text(0);
//    }

//    emit outTargetType(pattern);
//    emit outTargetSize(spinBox_rowCount->value(), spinBox_colCount->value(),
//                       markerSize, checkerSize, dictionaryName);




    QList<QListWidgetItem *> selected_items ;
    selected_items  = list_inputFlags->selectedItems();
    int flags = 0;
    QString flagsName = "";
    for(int i = 0; i < selected_items .size(); i ++)
    {
        flagsName += selected_items[i]->text() + "/n";
        flags = flags | translateFlags(selected_items[i]->text());
    }
    fs_->writeSettingCalibInYaml(spinBox_iterationSub->value(), flags, flagsName);
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
    label_row->hide();
    label_col->hide();
    spinBox_rowCount->hide();
    spinBox_colCount->hide();

    label_markerSize->hide();
    label_checkerSize->hide();
    spinBox_markerSize->hide();
    spinBox_checkerSize->hide();
    tree_inputDictionaryName->hide();

    QString pattern = item->text(0);
    if(pattern == "Chessboard" || pattern == "Circles" || pattern == "Assymetric Circles")
    {
        label_row->show();
        label_col->show();
        spinBox_rowCount->show();
        spinBox_colCount->show();
    }
    if(pattern == "ChArUco")
    {
        label_row->show();
        label_col->show();
        spinBox_rowCount->show();
        spinBox_colCount->show();

        label_markerSize->show();
        label_checkerSize->show();
        spinBox_markerSize->show();
        spinBox_checkerSize->show();
        tree_inputDictionaryName->show();
    }
}
