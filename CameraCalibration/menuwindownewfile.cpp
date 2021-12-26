#include "menuwindownewfile.h"

MenuWindowNewFile::MenuWindowNewFile(QWidget* parent): QWidget(parent)
{
    resize(400, 300);

    btnSetFolderPath = new QPushButton("...");
    btnSetOk = new QPushButton("Ok");
    btnSetCancel = new QPushButton("Cancel");

    connect(btnSetOk, SIGNAL(clicked()), this, SLOT(on_btnSetOk_clicked()));
    connect(btnSetCancel, SIGNAL(clicked()), this, SLOT(on_btnSetCancel_clicked()));
    connect(btnSetFolderPath, SIGNAL(clicked()), this, SLOT(on_btnSetFolderPath_clicked()));

    QVBoxLayout* layout_menu = new QVBoxLayout;

    QVBoxLayout* layout_selectFolder = new QVBoxLayout;

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

        QFormLayout *layout_chekingPatternSetting = new QFormLayout;

        tree_inputPatern = new QTreeWidget;
        tree_inputPatern->setColumnCount(1);
        tree_inputPatern->setHeaderLabels(QStringList() << "Pattern");
        QList<QTreeWidgetItem *> items_inputPatern;
        items_inputPatern.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("Chessboard"))));
        items_inputPatern.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("Circles"))));
        items_inputPatern.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("Assymetric Circles"))));
        items_inputPatern.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("ChArUco"))));
        tree_inputPatern->insertTopLevelItems(0, items_inputPatern);

    connect(tree_inputPatern,SIGNAL(itemClicked(QTreeWidgetItem*, int)),this,SLOT(on_tree_inputPatern_clicked(QTreeWidgetItem*, int)));

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

    label_row->hide();
    label_col->hide();
    spinBox_rowCount->hide();
    spinBox_colCount->hide();

    label_markerSize->hide();
    label_checkerSize->hide();
    spinBox_markerSize->hide();
    spinBox_checkerSize->hide();
    tree_inputDictionaryName->hide();

////
    layout_chekingPatternSetting->addRow("Select pattern", tree_inputPatern);

    //////////////////////

    QHBoxLayout* layout_btnOkCancel = new QHBoxLayout;
    layout_btnOkCancel->addWidget(btnSetOk);
    layout_btnOkCancel->addWidget(btnSetCancel);

    layout_menu->addLayout(layout_selectFolder);
    layout_menu->addLayout(layout_chekingPatternSetting);
    layout_menu->addLayout(layout_rowColPatternSetting);
    layout_menu->addLayout(layout_btnOkCancel);

    setLayout(layout_menu);
}

MenuWindowNewFile::~MenuWindowNewFile()
{

}

void MenuWindowNewFile::on_btnSetOk_clicked()
{

    fileSystem_.setPath(pathName+"/");
    fileSystem_.createWorkDir();
    emit setPathDir(fileSystem_.getFilePath());

    QTreeWidgetItem* item_tree_inputDictionaryName;
    QTreeWidgetItem* item_tree_inputPatern;
    item_tree_inputPatern = tree_inputPatern->currentItem();
    item_tree_inputDictionaryName = tree_inputDictionaryName->currentItem();

    QString pattern;
    QString dictionaryName;

    if(item_tree_inputPatern == NULL) pattern = "NULL";
    else {
        pattern = item_tree_inputPatern->text(0);
    }

    if(item_tree_inputDictionaryName == NULL) dictionaryName = "NULL";
    else {
        dictionaryName = item_tree_inputDictionaryName->text(0);
    }

        emit setParamentCalib(pattern,
                                 spinBox_rowCount->value(),
                                 spinBox_colCount->value(),
                                 spinBox_checkerSize->value(),
                                 spinBox_markerSize->value(),
                                 dictionaryName);
    close();
}

void MenuWindowNewFile::on_btnSetCancel_clicked()
{
    close();
}

void MenuWindowNewFile::on_btnSetFolderPath_clicked()
{
    pathName = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                         "/home",
                                                         QFileDialog::ShowDirsOnly
                                                         |QFileDialog::DontResolveSymlinks);
    line_folderPath->setText(pathName);
}


void MenuWindowNewFile::on_tree_inputPatern_clicked(QTreeWidgetItem* item, int col)
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

    pattern = item->text(0);
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
