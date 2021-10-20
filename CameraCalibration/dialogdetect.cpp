#include "dialogdetect.h"
#include "ui_dialogdetect.h"

DialogDetect::DialogDetect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDetect)
{
    ui->setupUi(this);
}

DialogDetect::~DialogDetect()
{
    delete ui;
}
