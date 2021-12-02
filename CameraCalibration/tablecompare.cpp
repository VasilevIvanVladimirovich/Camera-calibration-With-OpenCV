#include "tablecompare.h"

TableCompare::TableCompare(QWidget* parent): QWidget(parent)
{
    resize(800,600);
    btnOpenDir_= new QPushButton("Open Dir");
    btnAddFile_ = new QPushButton("Add file");
    btnClearTable_ = new QPushButton("Clear");
    table_ = new QTableWidget();

    connect(btnAddFile_, SIGNAL(clicked()), this, SLOT(on_btnAddFile_clicked()));
    connect(btnOpenDir_, SIGNAL(clicked()), this, SLOT(on_btnOpenDir_clicked()));
    connect(btnClearTable_, SIGNAL(clicked()), this, SLOT(on_btnClearTable_clicked()));
    connect(&fs_,
            SIGNAL(outTableItemsCompare(QTableWidgetItem*,QTableWidgetItem*,
                                        QTableWidgetItem*, QTableWidgetItem*,
                                        QTableWidgetItem*, QTableWidgetItem*)),
            this,
            SLOT(addItem(QTableWidgetItem*,QTableWidgetItem*,
                         QTableWidgetItem*, QTableWidgetItem*,
                         QTableWidgetItem*, QTableWidgetItem*)));


    table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table_->setColumnCount(6);
    table_->setHorizontalHeaderLabels(QStringList()<<"File"<<"Date"<<"Count img"<<"Pattern"<<"SizePattern"<<"Rmse");
    table_->setShowGrid(false);
    table_->setSelectionMode(QAbstractItemView::SingleSelection);    // Разрешаем выделение только одного элемента
    table_->setSelectionBehavior(QAbstractItemView::SelectRows);    // Разрешаем выделение построчно
    table_->setSortingEnabled(false);

    QHBoxLayout* layout_forBtn = new QHBoxLayout;
    layout_forBtn->addWidget(btnOpenDir_, 1);
    layout_forBtn->addWidget(btnAddFile_, 2);
    layout_forBtn->addStretch(100);
    layout_forBtn->addWidget(btnClearTable_, 2);

    QVBoxLayout* layout_main = new QVBoxLayout;
    layout_main->addLayout(layout_forBtn);
    layout_main->addWidget(table_);

    setLayout(layout_main);

}

TableCompare::~TableCompare()
{

}

void TableCompare::on_btnAddFile_clicked()
{
    QString pathName;
    pathName = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "/home",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    fs_.setPath(pathName + "/");
    fs_.getOneTableItemsinTableCompare();
}

void TableCompare::on_btnOpenDir_clicked()
{
    QString pathName;
    pathName = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "/home",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    fs_.setPath(pathName + "/");
    fs_.getTableItemsinTableCompare();
}

void TableCompare::on_btnClearTable_clicked()
{
    table_->clearContents();
    table_->setRowCount(0);
}

void TableCompare::addItem(QTableWidgetItem* itemFile,QTableWidgetItem* itemDate,
                           QTableWidgetItem* itemCount,QTableWidgetItem* itemPattern,
                           QTableWidgetItem* itemSizePattern, QTableWidgetItem* itemRmse)
{
    table_->setRowCount(table_->rowCount()+1);
    table_->setItem(table_->rowCount()-1,0,itemFile);
    table_->setItem(table_->rowCount()-1,1,itemDate);
    table_->setItem(table_->rowCount()-1,2,itemCount);
    table_->setItem(table_->rowCount()-1,3,itemPattern);
    table_->setItem(table_->rowCount()-1,4,itemSizePattern);
    table_->setItem(table_->rowCount()-1,5,itemRmse);
}

