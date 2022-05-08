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

    QFormLayout *layout_importCameras = new QFormLayout;

    box_cameraInput = new QComboBox();
    box_cameraInput->addItem("WebCamera");
    box_cameraInput->addItem("Basler");

    connect(box_cameraInput,SIGNAL(currentIndexChanged(int)),this,SLOT(on_box_cameraInput_IndexChanged(int)));

    layout_importCameras->addRow("Select input camera", box_cameraInput);

    box_cameraFirst = new QComboBox();
    layout_importCameras->addRow("Camera Number first:", box_cameraFirst);

    box_cameraSecond = new QComboBox();
    layout_importCameras->addRow("Camera Number Second:", box_cameraSecond);

    connect(box_cameraFirst,SIGNAL(currentIndexChanged(int)),this,SLOT(on_box_cameraFirst_IndexChanged(int)));

    box_cameraFirst->addItem("NULL");
    box_cameraSecond->addItem("NULL");
    QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
    for (const QCameraDevice &cameraDevice : cameras) {
        box_cameraFirst->addItem(cameraDevice.description());
        box_cameraSecond->addItem(cameraDevice.description());
    }

    layout_selectFolder->addLayout(layout_inputSelectFolder);
    layout_selectFolder->addLayout(layout_importCameras);

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
    spinBox_rowCount->setMinimum(1);
    layout_rowPatternSetting->addWidget(label_row);
    layout_rowPatternSetting->addWidget(spinBox_rowCount);

    QVBoxLayout* layout_colPatternSetting = new QVBoxLayout;
    label_col = new QLabel;
    label_col->setText("Column");

    spinBox_colCount = new QSpinBox;
    spinBox_colCount->setMinimum(1);
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

    box_cameraSecond->setEnabled(false);

    spinBox_rowCount->setEnabled(false);
    spinBox_colCount->setEnabled(false);

    spinBox_markerSize->setEnabled(false);
    spinBox_checkerSize->setEnabled(false);
    tree_inputDictionaryName->setEnabled(false);

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

    int numCamFirst, numCamSecond;
    if(box_cameraFirst->currentIndex()==0)
         numCamFirst=-1;
    else numCamFirst = box_cameraFirst->currentIndex()-1;
    if(box_cameraSecond->currentIndex()==0)
        numCamSecond=-1;
    else {numCamSecond = box_cameraSecond->currentIndex()-1;}

    QString nameCumFirst = box_cameraFirst->itemText(box_cameraFirst->currentIndex());
    QString nameCumSecond = box_cameraSecond->itemText(box_cameraSecond->currentIndex());

    fileSystem_.setPath(pathName+"/");
    if(numCamSecond != -1) fileSystem_.createWorkDir(2);
    else {fileSystem_.createWorkDir(1);}

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

    bool isBaslerCamera = false;
    bool isWebCamera = false;
    if(box_cameraInput->currentIndex() == 1)
        isBaslerCamera = true;
    else if(box_cameraInput->currentIndex() == 0)
        isWebCamera = true;
    fileSystem_.writeSettingCalibInYaml(numCamFirst,
                                        nameCumFirst,
                                        numCamSecond,
                                        nameCumSecond,
                                        pattern,
                                        spinBox_rowCount->value(),
                                        spinBox_colCount->value(),
                                        spinBox_checkerSize->value(),
                                        spinBox_markerSize->value(),
                                        dictionaryName,
                                        isWebCamera,
                                        isBaslerCamera);
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
    spinBox_rowCount->setEnabled(false);
    spinBox_colCount->setEnabled(false);

    spinBox_markerSize->setEnabled(false);
    spinBox_checkerSize->setEnabled(false);
    tree_inputDictionaryName->setEnabled(false);

    pattern = item->text(0);
    if(pattern == "Chessboard" || pattern == "Circles" || pattern == "Assymetric Circles")
    {
        spinBox_rowCount->setEnabled(true);
        spinBox_colCount->setEnabled(true);
    }
    if(pattern == "ChArUco")
    {;
        spinBox_rowCount->setEnabled(true);
        spinBox_colCount->setEnabled(true);

        spinBox_markerSize->setEnabled(true);
        spinBox_checkerSize->setEnabled(true);
        tree_inputDictionaryName->setEnabled(true);
    }
}

void MenuWindowNewFile::on_box_cameraFirst_IndexChanged(int index)
{
    if(index ==-1 || index ==0) box_cameraSecond->setEnabled(false);
    else box_cameraSecond->setEnabled(true);
}

void MenuWindowNewFile::on_box_cameraInput_IndexChanged(int index)
{
    //вывод списка баслер камер
    if(index == 1)
    {
        box_cameraFirst->clear();
        box_cameraSecond->clear();

        box_cameraFirst->addItem("NULL");
        box_cameraSecond->addItem("NULL");


        Pylon::PylonInitialize();


//        Pylon::CTlFactory &TlFactory = Pylon::CTlFactory::GetInstance();
//        Pylon::TlInfoList_t lstInfo;
//        int n = TlFactory.EnumerateTls(lstInfo);
//        Pylon::TlInfoList_t::const_iterator it;
//        for ( it = lstInfo.begin(); it != lstInfo.end(); ++it )
//        {
//            qDebug() << "FriendlyName: " << it->GetFriendlyName ();
//            qDebug() << "FullName: " << it->GetFullName();
//            qDebug() << "VendorName: " << it->GetVendorName() ;
//            qDebug() << "DeviceClass: " << it->GetDeviceClass() ;
//            qDebug() << "";
//        }


        Pylon::CTlFactory &TlFactory = Pylon::CTlFactory::GetInstance();
        Pylon::ITransportLayer * pTl = TlFactory.CreateTl("BaslerUsb");
        Pylon::DeviceInfoList_t lstDevices;
        pTl->EnumerateDevices(lstDevices);
        int n = pTl->EnumerateDevices(lstDevices,true);
        if(n == 0)
        {
            qDebug() << "Cannot find any camera!";
            return;
        }
        Pylon::DeviceInfoList_t::const_iterator it;
        QString name;
        for( it = lstDevices.begin(); it != lstDevices.end(); ++it )
        {
            name = it->GetFriendlyName();
            box_cameraFirst->addItem(name);
            box_cameraSecond->addItem(name);
        }

        Pylon::PylonTerminate();
    }else if(index == 0) // вывод списка вебкамер
    {
        box_cameraFirst->clear();
        box_cameraSecond->clear();

        box_cameraFirst->addItem("NULL");
        box_cameraSecond->addItem("NULL");
        QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
        for (const QCameraDevice &cameraDevice : cameras) {
            box_cameraFirst->addItem(cameraDevice.description());
            box_cameraSecond->addItem(cameraDevice.description());
        }
    }
}
