#include "dialogdetect.h"
#include "ui_dialogdetect.h"

DialogDetect::DialogDetect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDetect)
{
    ui->setupUi(this);
    //resize(800,600);
}

DialogDetect::~DialogDetect()
{
    delete ui;
}

void DialogDetect::on_buttonBox_accepted()
{
    QTreeWidgetItem* item;
    item = ui->PatternWidget->currentItem();
    QString pattern = item->text(0);

    emit outTargetType(pattern);
    emit outTargetSize(ui->sb_row->value(), ui->sb_col->value());
    emit outSubPixIter(ui->sb_subIter->value());
    close();
}
