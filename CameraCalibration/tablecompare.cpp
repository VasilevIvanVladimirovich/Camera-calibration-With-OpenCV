#include "tablecompare.h"

TableCompare::TableCompare(QWidget* parent): QWidget(parent)
{
    btnAddFile_ = new QPushButton("Add new");
    btnAddFile_->setEnabled(false);
    table_ = new QTableWidget();

    connect(btnAddFile_, SIGNAL(clicked()), this, SLOT(on_btnAddFile_clicked()));
    connect(&fs_,
            SIGNAL(outTableItemsCompare(QTableWidgetItem*,
                                        QTableWidgetItem*, QTableWidgetItem*,
                                        QTableWidgetItem*, QTableWidgetItem*,
                                        QTableWidgetItem*)),
            this,
            SLOT(addItem(QTableWidgetItem*,
                         QTableWidgetItem*, QTableWidgetItem*,
                         QTableWidgetItem*, QTableWidgetItem*,
                         QTableWidgetItem*)));



    table_->setFocusPolicy(Qt::NoFocus);
    table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table_->setColumnCount(6);
    table_->setHorizontalHeaderLabels(QStringList()<<"File"<<"SettingCam1"<<"Cam1Rmse"<<"SettingCam2"<<"Cam2Rmse"<<"Stereo");
    table_->setShowGrid(false);
    table_->setSelectionMode(QAbstractItemView::SingleSelection);       // Разрешаем выделение только одного элемента
    table_->setSelectionBehavior(QAbstractItemView::SelectRows);        // Разрешаем выделение построчно
    table_->setSortingEnabled(false);

    connect(table_,SIGNAL(cellClicked(int, int)), this, SLOT(on_table_cellClicked(int,int)));

    QHBoxLayout* layout_forBtn = new QHBoxLayout;
    layout_forBtn->addWidget(btnAddFile_, 2);
    layout_forBtn->addStretch(100);


    QVBoxLayout* layout_main = new QVBoxLayout;
    layout_main->addLayout(layout_forBtn);
    layout_main->addWidget(table_);

    setLayout(layout_main);
}

TableCompare::~TableCompare()
{

}

void TableCompare::setDir(QString dir)
{
    fs_.setPath(dir);
    btnAddFile_->setEnabled(true);
}

void TableCompare::on_btnAddFile_clicked()
{
    emit addNewFile(fs_.getFilePath());
}

void TableCompare::on_table_cellClicked(int row, int)
{
    QTableWidgetItem* nameItem;
    nameItem = table_->item(row,0);
    emit openFile(fs_.getFilePath() + "/" + nameItem->text()+"/");
}

void TableCompare::addItem(QTableWidgetItem* itemFile,
                           QTableWidgetItem* SettingCam1,QTableWidgetItem* itemCamera1Rmse,
                           QTableWidgetItem* SettingCam2, QTableWidgetItem* itemCamera2Rmse,
                           QTableWidgetItem* itemStereoRmse)
{
    table_->setRowCount(table_->rowCount()+1);
    table_->setItem(table_->rowCount()-1,0,itemFile);
    table_->setItem(table_->rowCount()-1,1,SettingCam1);
    table_->setItem(table_->rowCount()-1,2,itemCamera1Rmse);
    table_->setItem(table_->rowCount()-1,3,SettingCam2);
    table_->setItem(table_->rowCount()-1,4,itemCamera2Rmse);
    table_->setItem(table_->rowCount()-1,5,itemStereoRmse);
}

void TableCompare::updateTable()
{
    table_->clearContents();
    table_->setRowCount(0);
    fs_.getTableItemsinTableCompare();
}

