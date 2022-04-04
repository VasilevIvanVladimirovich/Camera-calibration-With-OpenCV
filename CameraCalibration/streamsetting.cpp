#include "streamsetting.h"

StreamSetting::StreamSetting(FileSystem* fs)
{
    fileSystem_ = fs;
    initUi();
}

StreamSetting::~StreamSetting()
{

}

void StreamSetting::initUi()
{
    QVBoxLayout* layout_main = new QVBoxLayout;

    box_setting = new QComboBox();
    layout_main->addWidget(box_setting);

    QHBoxLayout* layout_buttons = new QHBoxLayout;
    btn_ok = new QPushButton("&Ok", this);
    btn_cencel = new QPushButton("&Cencel", this);
    layout_buttons->addWidget(btn_ok);
    layout_buttons->addWidget(btn_cencel);

    layout_main->addLayout(layout_buttons);

    setLayout(layout_main);
    createAction();
    fillBoxSetting();
}

void StreamSetting::createAction()
{
    connect(btn_ok, SIGNAL(clicked()),this,SLOT(on_btn_ok_clicked()));

    connect(btn_cencel, SIGNAL(clicked()),this,SLOT(on_btn_cencel_clicked()));
}

void StreamSetting::fillBoxSetting()
{
    if(fileSystem_->getIndexCameraFirst() != -1)
        box_setting->addItem("FirstCameraStream");
    if(fileSystem_->getIndexCameraSecond() != -1)
        box_setting->addItem("SecondCameraStream");

    if(fileSystem_->getIndexCameraFirst() != -1 && fileSystem_->getIndexCameraSecond() != -1)
    {
        box_setting->addItem("FirstAndSecondCameraStream");
        box_setting->addItem("FirstAndSecondCameraStreamWhithLine");
    }

    if(fileSystem_->isFirstCameraResul())
        box_setting->addItem("FirstCameraCalibratedStream");

    if(fileSystem_->isSecondCameraResul())
        box_setting->addItem("SecondCameraCalibratedStream");

    if(fileSystem_->isSecondCameraResul())
        box_setting->addItem("FirstAndSecondCameraCalibratedStream");

    if(fileSystem_->isStereoCameraResul())
    {
        box_setting->addItem("StereoCalibratedStream");
        box_setting->addItem("DepthMapStream");
    }
}

void StreamSetting::on_btn_ok_clicked()
{
    QString current = box_setting->currentText();
    emit sendState(current);
    close();
}

void StreamSetting::on_btn_cencel_clicked()
{
    close();
}
