#include "dialogsetimg.h"
#include "ui_dialogsetimg.h"

DialogSetImg::DialogSetImg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetImg)
{
    ui->setupUi(this);
}

DialogSetImg::~DialogSetImg()
{
    delete ui;
}

void DialogSetImg::setFileSystem(FileSystem *fs)
{
    this->fs = fs;
}

void DialogSetImg::on_btn_findpath_clicked()
{
    pathName = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                         "/home",
                                                         QFileDialog::ShowDirsOnly
                                                         |QFileDialog::DontResolveSymlinks);
    ui->label_pathname->setText(pathName);
}

void DialogSetImg::on_buttonBox_accepted()
{
    fs->copyDirImgInWorkDir(pathName);
    fs->getTableItems();
}

void DialogSetImg::on_btn_getCameraImg_clicked()
{
    QTreeWidgetItem* item;
    item = ui->treeWidgetPattern->currentItem();
    QString pattern = item->text(0);
    emit signalVideoStream(ui->frameRate->value(),
                           ui->countFrame->value(),
                           ui->spinRow->value(),
                           ui->spinColumn->value(),
                           ui->checkBoxUsePattern->checkState(),
                           ui->checkBoxUseSnapShoot->checkState(),
                           pattern);
    close();
}

