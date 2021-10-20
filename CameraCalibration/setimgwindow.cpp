#include "setimgwindow.h"
#include "ui_setimgwindow.h"

SetImgWindow::SetImgWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SetImgWindow)
{
    ui->setupUi(this);
}

SetImgWindow::~SetImgWindow()
{
    delete ui;
}
