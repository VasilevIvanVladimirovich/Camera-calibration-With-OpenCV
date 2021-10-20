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
