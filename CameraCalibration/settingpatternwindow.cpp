#include "settingpatternwindow.h"

SettingPatternWindow::SettingPatternWindow(FileSystem* fs)
{
    fileSystem_ = fs;
    initUi();
    updateTemplates();
}
SettingPatternWindow::~SettingPatternWindow()
{

}

void SettingPatternWindow::on_tree_inputPatern_clicked(QTreeWidgetItem *item, int col)
{
    spinBox_rowCount->setEnabled(false);
    spinBox_colCount->setEnabled(false);

    spinBox_markerSize->setEnabled(false);
    tree_inputDictionaryName->setEnabled(false);

    for(unsigned short int i = 0; tree_inputPatern->topLevelItemCount() > i;i++)
    {
        QTreeWidgetItem *itemm = tree_inputPatern->topLevelItem( i );
        itemm->setSelected(false);
    }

    QString pattern;
    pattern = item->text(0);
    item->setSelected(true);
    if(pattern == "Chessboard" || pattern == "Circles" || pattern == "Assymetric Circles")
    {
        spinBox_markerSize->setValue(0);
        for(unsigned short int i = 0; tree_inputDictionaryName->topLevelItemCount() > i;i++)
        {
            QTreeWidgetItem *itemm = tree_inputDictionaryName->topLevelItem( i );
            itemm->setSelected(false);
        }
        tree_inputDictionaryName->setEnabled(true);
        spinBox_rowCount->setEnabled(true);
        spinBox_colCount->setEnabled(true);
    }
    if(pattern == "ChArUco")
    {
        spinBox_rowCount->setEnabled(true);
        spinBox_colCount->setEnabled(true);

        spinBox_markerSize->setEnabled(true);
        tree_inputDictionaryName->setEnabled(true);
    }
}

void SettingPatternWindow::on_tree_tree_inputDictionaryName_clicked(QTreeWidgetItem *item, int col)
{
    for(unsigned short int i = 0; tree_inputDictionaryName->topLevelItemCount() > i;i++)
    {
        QTreeWidgetItem *itemm = tree_inputDictionaryName->topLevelItem( i );
        itemm->setSelected(false);
    }
    item->setSelected(true);
}

void SettingPatternWindow::on_btnSetOk_clicked()
{
    QTreeWidgetItem* item_tree_inputDictionaryName;
    QTreeWidgetItem* item_tree_inputPatern;

    for(unsigned short int i = 0; tree_inputPatern->topLevelItemCount() > i;i++)
    {
        QTreeWidgetItem *item = tree_inputPatern->topLevelItem( i );
        if(item->isSelected())
            item_tree_inputPatern = item;
    }

    for(unsigned short int i = 0; tree_inputDictionaryName->topLevelItemCount() > i;i++)
    {
        QTreeWidgetItem *item = tree_inputDictionaryName->topLevelItem( i );
        if(item->isSelected())
            item_tree_inputDictionaryName = item;
    }

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

    fileSystem_->patternSettingSave(pattern,
                                    spinBox_rowCount->value(),
                                    spinBox_colCount->value(),
                                    spinBox_checkerSize->value(),
                                    spinBox_markerSize->value(),
                                    dictionaryName);

    emit sendUpdate();
}

void SettingPatternWindow::on_btnSetCancel_clicked()
{
    close();
}

void SettingPatternWindow::on_btnSetNewTemp_clicked()
{
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

    fileSystem_->addTemplates(pattern,
                              spinBox_rowCount->value(),
                              spinBox_colCount->value(),
                              spinBox_checkerSize->value(),
                              spinBox_markerSize->value(),
                              dictionaryName);
    updateTemplates();
}

void SettingPatternWindow::on_btnSetDelTemp_clicked()
{
    fileSystem_->removeTemplates(templatesPattern->count()-templatesPattern->currentIndex());
    updateTemplates();
}

void SettingPatternWindow::on_box_templatesPattern_IndexChanged(int)
{
    QString currentText = templatesPattern->currentText();

    for(unsigned short int i = 0; tree_inputPatern->topLevelItemCount() > i;i++)
    {
        QTreeWidgetItem *item = tree_inputPatern->topLevelItem( i );
        if(item->text(0) == currentText.section("/",0,0))
        {
            on_tree_inputPatern_clicked(item, 0);
            QList<QTreeWidgetItem*> selectedItems;
            selectedItems = tree_inputDictionaryName->selectedItems();
            if(selectedItems.count() !=0)
            {
                for(unsigned short int i = 0;i<selectedItems.count();i++)
                {
                    selectedItems[i]->setSelected(false);
                }
            }
            break;
        }
    }
    spinBox_rowCount->setValue(currentText.section("/",1,1).toInt());
    spinBox_colCount->setValue(currentText.section("/",2,2).toInt());
    spinBox_checkerSize->setValue(currentText.section("/",3,3).toInt());
    spinBox_markerSize->setValue(0);
    if(currentText.section("/",0,0) == "ChArUco")
    {
        spinBox_checkerSize->setValue(currentText.section("/",4,4).toInt());
        spinBox_markerSize->setValue(currentText.section("/",5,5).toInt());

        for(unsigned short int i = 0; tree_inputDictionaryName->topLevelItemCount() > i;i++)
        {
            QTreeWidgetItem *item = tree_inputDictionaryName->topLevelItem( i );
            if(item->text(0) == currentText.section("/",3,3))
            {
                on_tree_tree_inputDictionaryName_clicked(item,0);
                break;
            }
        }
    }
}

void SettingPatternWindow::initUi()
{
    QVBoxLayout* layout_main = new QVBoxLayout;

    QFormLayout *layout_chekingPatternSetting = new QFormLayout;

    QHBoxLayout* layout_buttonTemplates = new QHBoxLayout;

    btnNewTemplates = new QPushButton("New");
    btnDelTemplates = new QPushButton("Delete");

    connect(btnNewTemplates, SIGNAL(clicked()), this, SLOT(on_btnSetNewTemp_clicked()));
    connect(btnDelTemplates, SIGNAL(clicked()), this, SLOT(on_btnSetDelTemp_clicked()));

    layout_buttonTemplates->addWidget(btnDelTemplates);
    layout_buttonTemplates->addWidget(btnNewTemplates);

    QWidget* widgetButton = new QWidget();
    widgetButton->setLayout(layout_buttonTemplates);

    templatesPattern = new QComboBox();
    connect(templatesPattern,SIGNAL(currentIndexChanged(int)),this,SLOT(on_box_templatesPattern_IndexChanged(int)));

    layout_chekingPatternSetting->addRow(widgetButton, templatesPattern);

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
    connect(tree_inputDictionaryName,SIGNAL(itemClicked(QTreeWidgetItem*, int)),this,SLOT(on_tree_tree_inputDictionaryName_clicked(QTreeWidgetItem*, int)));
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

    layout_rowColPatternSetting->addLayout(layout_rowPatternSetting);
    layout_rowColPatternSetting->addLayout(layout_colPatternSetting);
    layout_rowColPatternSetting->addLayout(layout_checkerSizeSetting);
    layout_rowColPatternSetting->addLayout(layout_markerSizeSetting);

    layout_chekingPatternSetting->addRow(tree_inputPatern,tree_inputDictionaryName);

    spinBox_rowCount->setEnabled(false);
    spinBox_colCount->setEnabled(false);

    spinBox_markerSize->setEnabled(false);
    tree_inputDictionaryName->setEnabled(false);

//


    layout_main->addLayout(layout_chekingPatternSetting);
    layout_main->addLayout(layout_rowColPatternSetting);

    btnSetOk = new QPushButton("Apply");
    btnSetCancel = new QPushButton("Cancel");

    connect(btnSetOk, SIGNAL(clicked()), this, SLOT(on_btnSetOk_clicked()));
    connect(btnSetCancel, SIGNAL(clicked()), this, SLOT(on_btnSetCancel_clicked()));

    layout_main->addWidget(btnSetOk,0,Qt::AlignRight);

    QGroupBox *groupBox= new QGroupBox(tr("Pattern Setting"));
    groupBox->setAlignment(Qt::AlignCenter);

    groupBox->setLayout(layout_main);

    QHBoxLayout* layout_out = new QHBoxLayout;
    layout_out->addWidget(groupBox);
    setLayout(layout_out);
}

void SettingPatternWindow::updateTemplates()
{
    std::vector<FileSystem::TempatesPattern> tempatesPattern = fileSystem_->getTempatesPattern();
    templatesPattern->clear();
    for(unsigned short int i = tempatesPattern.size()-1; i != 0; i--)
    {
        if(tempatesPattern[i].pattern!="ChArUco")

            templatesPattern->addItem(QString(QString::fromStdString(tempatesPattern[i].pattern) + "/" +
                                            QString::number(tempatesPattern[i].row) + "/" +
                                            QString::number(tempatesPattern[i].col) + "/" +
                                            QString::number(tempatesPattern[i].checker)));
        else
            templatesPattern->addItem(QString(QString::fromStdString(tempatesPattern[i].pattern) + "/" +
                                            QString::number(tempatesPattern[i].row) + "/" +
                                            QString::number(tempatesPattern[i].col) + "/" +
                                            QString::fromStdString(tempatesPattern[i].dictionaryName) + "/" +
                                            QString::number(tempatesPattern[i].checker) + "/" +
                                            QString::number(tempatesPattern[i].marker)));
    }
}
