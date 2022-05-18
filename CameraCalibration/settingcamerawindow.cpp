#include "settingcamerawindow.h"

SettingCameraWindow::SettingCameraWindow(FileSystem* fs)
{
    fileSystem_ = fs;
    initUi();
}

SettingCameraWindow::~SettingCameraWindow()
{

}

void SettingCameraWindow::initUi()
{

    QFormLayout *layout_importCameras = new QFormLayout;

    box_cameraInput = new QComboBox();
    box_cameraInput->addItem("WebCamera");
    box_cameraInput->addItem("Basler");

    connect(box_cameraInput,SIGNAL(currentIndexChanged(int)),this,SLOT(on_box_cameraInput_IndexChanged(int)));

    layout_importCameras->addRow("Select input camera", box_cameraInput);

    box_cameraFirst = new QComboBox();
    layout_importCameras->addRow("Camera Number first:", box_cameraFirst);

    box_cameraSecond = new QComboBox();
    layout_importCameras->addRow("Camera Number Second:", box_cameraSecond);

    connect(box_cameraFirst,SIGNAL(currentIndexChanged(int)),this,SLOT(on_box_cameraFirst_IndexChanged(int)));

    box_cameraFirst->addItem("NULL");
    box_cameraSecond->addItem("NULL");
    QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
    for (const QCameraDevice &cameraDevice : cameras)
    {
        box_cameraFirst->addItem(cameraDevice.description());
        box_cameraSecond->addItem(cameraDevice.description());
    }

    btnSetOk = new QPushButton("Ok");
    btnSetCancel = new QPushButton("Cancel");
    layout_importCameras->addRow(btnSetOk, btnSetCancel);

    connect(btnSetOk, SIGNAL(clicked()), this, SLOT(on_btnSetOk_clicked()));
    connect(btnSetCancel, SIGNAL(clicked()), this, SLOT(on_btnSetCancel_clicked()));

    QGroupBox *groupBox= new QGroupBox(tr("Camera Setting"));
    groupBox->setAlignment(Qt::AlignCenter);

    groupBox->setLayout(layout_importCameras);

    QHBoxLayout* layout_out = new QHBoxLayout;
    layout_out->addWidget(groupBox);
    setLayout(layout_out);
}

void SettingCameraWindow::on_box_cameraInput_IndexChanged(int index)
{
    //вывод списка баслер камер
    if(index == 1)
    {
        box_cameraFirst->clear();
        box_cameraSecond->clear();

        box_cameraFirst->addItem("NULL");
        box_cameraSecond->addItem("NULL");


        Pylon::PylonInitialize();


//        Pylon::CTlFactory &TlFactory = Pylon::CTlFactory::GetInstance();
//        Pylon::TlInfoList_t lstInfo;
//        int n = TlFactory.EnumerateTls(lstInfo);
//        Pylon::TlInfoList_t::const_iterator it;
//        for ( it = lstInfo.begin(); it != lstInfo.end(); ++it )
//        {
//            qDebug() << "FriendlyName: " << it->GetFriendlyName ();
//            qDebug() << "FullName: " << it->GetFullName();
//            qDebug() << "VendorName: " << it->GetVendorName() ;
//            qDebug() << "DeviceClass: " << it->GetDeviceClass() ;
//            qDebug() << "";
//        }


        Pylon::CTlFactory &TlFactory = Pylon::CTlFactory::GetInstance();
        Pylon::ITransportLayer * pTl = TlFactory.CreateTl("BaslerUsb");
        Pylon::DeviceInfoList_t lstDevices;
        pTl->EnumerateDevices(lstDevices);
        int n = pTl->EnumerateDevices(lstDevices);
        if(n == 0)
        {
            qDebug() << "Cannot find any camera!";
            return;
        }
        Pylon::DeviceInfoList_t::const_iterator it;
        QString name;
        for( it = lstDevices.begin(); it != lstDevices.end(); it++)
        {

            name = it->GetFriendlyName();
            qDebug()<<name.toStdString().c_str();
            box_cameraFirst->addItem(name);
            box_cameraSecond->addItem(name);
        }

        Pylon::PylonTerminate();
    }else if(index == 0) // вывод списка вебкамер
    {
        box_cameraFirst->clear();
        box_cameraSecond->clear();

        box_cameraFirst->addItem("NULL");
        box_cameraSecond->addItem("NULL");
        QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
        for (const QCameraDevice &cameraDevice : cameras) {
            box_cameraFirst->addItem(cameraDevice.description());
            box_cameraSecond->addItem(cameraDevice.description());
        }
    }
}

void SettingCameraWindow::on_box_cameraFirst_IndexChanged(int index)
{
    if(index ==-1 || index ==0) box_cameraSecond->setEnabled(false);
    else box_cameraSecond->setEnabled(true);
}

void SettingCameraWindow::on_btnSetOk_clicked()
{
    int numCamFirst, numCamSecond;
    if(box_cameraFirst->currentIndex()==0)
        numCamFirst=-1;
    else numCamFirst = box_cameraFirst->currentIndex()-1;

    if(box_cameraSecond->currentIndex()==0)
        numCamSecond=-1;
    else {numCamSecond = box_cameraSecond->currentIndex()-1;}

    QString nameCumFirst = box_cameraFirst->itemText(box_cameraFirst->currentIndex());
    QString nameCumSecond = box_cameraSecond->itemText(box_cameraSecond->currentIndex());

    bool isBaslerCamera = false;
    bool isWebCamera = false;
    if(box_cameraInput->currentIndex() == 1)
        isBaslerCamera = true;
    else if(box_cameraInput->currentIndex() == 0)
        isWebCamera = true;

    fileSystem_->cameraSettingSave(nameCumFirst,numCamFirst,
                                   nameCumSecond, numCamSecond,
                                   isWebCamera, isBaslerCamera);
    emit sendUpdate();

    close();
}

void SettingCameraWindow::on_btnSetCancel_clicked()
{
    close();
}
