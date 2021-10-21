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

void DialogSetImg::on_btn_findpath_clicked()
{
    pathName = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                         "/home",
                                                         QFileDialog::ShowDirsOnly
                                                         | QFileDialog::DontResolveSymlinks);
    ui->label_pathname->setText(pathName);
    ui->label_countfindimg->setText(QString::number(fs.countImgInDir(pathName)) + " find img");
}


void DialogSetImg::on_buttonBox_accepted()
{
    emit outFolderPath(pathName);
    emit setTableItems();
}
