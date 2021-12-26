#include "dialogwindowimportimage.h"

DialogWindowImportImage::DialogWindowImportImage(QWidget* parent): QWidget(parent)
{
    resize(1280,800);
    btnStartVideoStream = new QPushButton("StartVideoStream");
    btnSetFolderPath = new QPushButton("...");
    btnSetOk = new QPushButton("Ok");
    btnSetCancel = new QPushButton("Cancel");

    connect(btnStartVideoStream, SIGNAL(clicked()), this, SLOT(on_btnStartVideoStream_clicked()));
    connect(btnSetFolderPath, SIGNAL(clicked()), this, SLOT(on_btnSetFolderPath_clicked()));
    connect(btnSetOk, SIGNAL(clicked()), this, SLOT(on_btnSetOk_clicked()));
    connect(btnSetCancel, SIGNAL(clicked()), this, SLOT(on_btnSetCancel_clicked()));

    QVBoxLayout* layout_menu = new QVBoxLayout;

/////////////////////////
    QFormLayout *layout_importVideoStreamSetting = new QFormLayout;
//////
    spinBox_numCamera = new QSpinBox();
    layout_importVideoStreamSetting->addRow("Camera Number:", spinBox_numCamera);

    spinBox_frameRate = new QSpinBox();
    layout_importVideoStreamSetting->addRow("Frame rate of file saving:", spinBox_frameRate);

    spinBox_countFrame = new QSpinBox();
////
    layout_importVideoStreamSetting->addRow("Count frame:", spinBox_countFrame);
/////////////////////////
    QFormLayout *layout_chekingPatternSetting = new QFormLayout;
//////
//    checkBox_isPattern = new QCheckBox;

//    tree_inputPatern = new QTreeWidget;
//    tree_inputPatern->setColumnCount(1);
//    tree_inputPatern->setHeaderLabels(QStringList() << "Pattern");
//    QList<QTreeWidgetItem *> items_inputPatern;
//    items_inputPatern.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("Chessboard"))));
//    items_inputPatern.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("Circles"))));
//    items_inputPatern.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("Assymetric Circles"))));
//    items_inputPatern.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("ChArUco"))));
//    tree_inputPatern->insertTopLevelItems(0, items_inputPatern);

//    connect(tree_inputPatern,SIGNAL(itemClicked(QTreeWidgetItem*, int)),this,SLOT(on_tree_inputPatern_clicked(QTreeWidgetItem*, int)));

    checkBox_isSnapShoot = new QCheckBox;

//    QHBoxLayout* layout_rowColPatternSetting = new QHBoxLayout;
//    QVBoxLayout* layout_rowPatternSetting = new QVBoxLayout;
//    QVBoxLayout* layout_checkerSizeSetting = new QVBoxLayout;
//    QVBoxLayout* layout_markerSizeSetting = new QVBoxLayout;
//    QVBoxLayout* layout_dictionarySetting = new QVBoxLayout;

//    label_row = new QLabel;
//    label_row->setText("Row");

//    spinBox_rowCount = new QSpinBox;
//    layout_rowPatternSetting->addWidget(label_row);
//    layout_rowPatternSetting->addWidget(spinBox_rowCount);

//    QVBoxLayout* layout_colPatternSetting = new QVBoxLayout;
//    label_col = new QLabel;
//    label_col->setText("Column");

//    spinBox_colCount = new QSpinBox;
//    layout_colPatternSetting->addWidget(label_col);
//    layout_colPatternSetting->addWidget(spinBox_colCount);

//    label_checkerSize = new QLabel;
//    label_checkerSize->setText("Checker Size (mm):");
//    spinBox_checkerSize = new QSpinBox;
//    layout_checkerSizeSetting->addWidget(label_checkerSize);
//    layout_checkerSizeSetting->addWidget(spinBox_checkerSize);

//    label_markerSize = new QLabel;
//    label_markerSize->setText("Marker Size (mm):");
//    spinBox_markerSize = new QSpinBox;
//    layout_markerSizeSetting->addWidget(label_markerSize);
//    layout_markerSizeSetting->addWidget(spinBox_markerSize);

//    tree_inputDictionaryName = new QTreeWidget;
//    tree_inputDictionaryName->setColumnCount(1);
//    tree_inputDictionaryName->setHeaderLabels(QStringList() << "Dictionary");
//    QList<QTreeWidgetItem *> items_DictionaryName;
//    items_DictionaryName.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("DICT_4X4_50"))));
//    items_DictionaryName.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("DICT_4X4_100"))));
//    items_DictionaryName.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("DICT_4X4_1000"))));
//    items_DictionaryName.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("DICT_5X5_50"))));
//    items_DictionaryName.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("DICT_5X5_100"))));
//    items_DictionaryName.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("DICT_5X5_250"))));
//    items_DictionaryName.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("DICT_5X5_1000"))));
//    items_DictionaryName.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("DICT_6X6_50"))));
//    items_DictionaryName.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("DICT_6X6_100"))));
//    items_DictionaryName.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("DICT_6X6_250"))));
//    items_DictionaryName.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("DICT_6X6_1000"))));
//    items_DictionaryName.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("DICT_7X7_50"))));
//    items_DictionaryName.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("DICT_7X7_100"))));
//    items_DictionaryName.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("DICT_7X7_250"))));
//    items_DictionaryName.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("DICT_7X7_1000"))));
//    items_DictionaryName.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("DICT_ARUCO_ORIGINAL"))));
//    items_DictionaryName.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("DICT_APRILTAG_16h5"))));
//    items_DictionaryName.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("DICT_APRILTAG_25h9"))));
//    items_DictionaryName.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("DICT_APRILTAG_36h10"))));
//    tree_inputDictionaryName->insertTopLevelItems(0, items_DictionaryName);
//    layout_dictionarySetting->addWidget(tree_inputDictionaryName);

//    layout_rowColPatternSetting->addLayout(layout_rowPatternSetting);
//    layout_rowColPatternSetting->addLayout(layout_colPatternSetting);
//    layout_rowColPatternSetting->addLayout(layout_checkerSizeSetting);
//    layout_rowColPatternSetting->addLayout(layout_markerSizeSetting);
//    layout_rowColPatternSetting->addLayout(layout_dictionarySetting);

    //
//    label_row->hide();
//    label_col->hide();
//    spinBox_rowCount->hide();
//    spinBox_colCount->hide();

//    label_markerSize->hide();
//    label_checkerSize->hide();
//    spinBox_markerSize->hide();
//    spinBox_checkerSize->hide();
//    tree_inputDictionaryName->hide();
    //

////
    //layout_chekingPatternSetting->addRow(checkBox_isPattern, tree_inputPatern);
    layout_chekingPatternSetting->addRow(tr("&frame by button:"), checkBox_isSnapShoot);
/////////////////////////
    QVBoxLayout* layout_selectFolder = new QVBoxLayout;
/////
    label_headerSelectFolder = new QLabel;
    label_headerSelectFolder->setText("Select image from a folder");
    layout_selectFolder->addWidget(label_headerSelectFolder);

    QHBoxLayout* layout_inputSelectFolder = new QHBoxLayout;

    label_folderPath = new QLabel;
    label_folderPath->setText("Folder path");

    line_folderPath = new QLineEdit;

    layout_inputSelectFolder->addWidget(label_folderPath);
    layout_inputSelectFolder->addWidget(line_folderPath);
    layout_inputSelectFolder->addWidget(btnSetFolderPath);

    layout_selectFolder->addLayout(layout_inputSelectFolder);
/////
/////////////////////////
    QHBoxLayout* layout_okCancel = new QHBoxLayout;
/////
    layout_okCancel->addWidget(btnSetOk);
    layout_okCancel->addWidget(btnSetCancel);
/////
    layout_menu->addLayout(layout_importVideoStreamSetting);
    layout_menu->addLayout(layout_chekingPatternSetting);
//    layout_menu->addLayout(layout_rowColPatternSetting);
    layout_menu->addWidget(btnStartVideoStream);
    layout_menu->addLayout(layout_selectFolder);
    layout_menu->addLayout(layout_okCancel);

    QHBoxLayout* layout_main = new QHBoxLayout;   //Горизонтальный
    layout_main->addLayout(layout_menu);

    setLayout(layout_main);
}

DialogWindowImportImage::~DialogWindowImportImage()
{

}

void DialogWindowImportImage::setFileSystem(FileSystem *fs)
{
    this->fs = fs;
}

void DialogWindowImportImage::on_btnStartVideoStream_clicked()
{
    emit signalVideoStream(spinBox_numCamera->value(),
                           spinBox_frameRate->value(),
                           spinBox_countFrame->value(),
                           checkBox_isSnapShoot->checkState());
    close();
}

void DialogWindowImportImage::on_btnSetFolderPath_clicked()
{
    pathName = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                         "/home",
                                                         QFileDialog::ShowDirsOnly
                                                         |QFileDialog::DontResolveSymlinks);
    line_folderPath->setText(pathName);
}

void DialogWindowImportImage::on_btnSetOk_clicked()
{
    fs->copyDirImgInWorkDir(pathName);
    fs->getTableItems();
    close();
}

void DialogWindowImportImage::on_btnSetCancel_clicked()
{
    close();
}

void DialogWindowImportImage::on_tree_inputPatern_clicked(QTreeWidgetItem* item, int col)
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

    if(checkBox_isPattern->checkState())
    {
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
}



