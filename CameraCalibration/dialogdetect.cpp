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

void DialogDetect::on_buttonBox_accepted()
{
    if(ui->rb_ches->isChecked())
    {
        emit outTargetType("Chessboard");
    }
    emit outTargetSize(ui->sb_row->value(),ui->sb_col->value());
    emit outSubPixIter(ui->sb_subIter->value());
}

