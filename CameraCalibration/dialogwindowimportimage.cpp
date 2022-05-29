#include "dialogwindowimportimage.h"

DialogWindowImportImage::DialogWindowImportImage(QWidget* parent): QWidget(parent)
{
    initUi();
}

DialogWindowImportImage::~DialogWindowImportImage()
{

}

void DialogWindowImportImage::initUi()
{
    btnStartVideoStream = new QPushButton("Start");
    btnSetFolderPath1 = new QPushButton("...");
    btnSetFolderPath2 = new QPushButton("...");
    btnSetOk = new QPushButton("Apply");

    connect(btnStartVideoStream, SIGNAL(clicked()), this, SLOT(on_btnStartVideoStream_clicked()));
    connect(btnSetFolderPath1, SIGNAL(clicked()), this, SLOT(on_btnSetFolderPath1_clicked()));
    connect(btnSetFolderPath2, SIGNAL(clicked()), this, SLOT(on_btnSetFolderPath2_clicked()));
    connect(btnSetOk, SIGNAL(clicked()), this, SLOT(on_btnSetOk_clicked()));

    QGroupBox *groupBoxPath= new QGroupBox(tr("Input from path"));
    groupBoxPath->setAlignment(Qt::AlignCenter);
    groupBoxPath->setFixedSize(500,200);

    QGroupBox *groupBoxCamera= new QGroupBox(tr("Input from camera"));
    groupBoxCamera->setAlignment(Qt::AlignCenter);
    groupBoxCamera->setFixedSize(300,200);

    QVBoxLayout* layout_selectFolder = new QVBoxLayout;
    QHBoxLayout* layout_inputSelectFolder1 = new QHBoxLayout;

    label_folderPath1 = new QLabel;
    label_folderPath1->setText("Camera1");

    line_folderPath1 = new QLineEdit;
    line_folderPath1->isReadOnly();

    layout_inputSelectFolder1->addWidget(label_folderPath1);
    layout_inputSelectFolder1->addWidget(line_folderPath1);
    layout_inputSelectFolder1->addWidget(btnSetFolderPath1);

    layout_selectFolder->addLayout(layout_inputSelectFolder1);

    QHBoxLayout* layout_inputSelectFolder2 = new QHBoxLayout;

    label_folderPath2 = new QLabel;
    label_folderPath2->setText("Camera2");

    line_folderPath2 = new QLineEdit;
    line_folderPath2->isReadOnly();

    layout_inputSelectFolder2->addWidget(label_folderPath2);
    layout_inputSelectFolder2->addWidget(line_folderPath2);
    layout_inputSelectFolder2->addWidget(btnSetFolderPath2);

    layout_selectFolder->addLayout(layout_inputSelectFolder2);

    layout_selectFolder->addWidget(btnSetOk,0,Qt::AlignRight);

    groupBoxPath->setLayout(layout_selectFolder);

    QVBoxLayout *layout_importVideoStreamSetting = new QVBoxLayout;

    isCamera1 = new QCheckBox("Cam1");
    layout_importVideoStreamSetting->addWidget(isCamera1);

    isCamera2 = new QCheckBox("Cam2");
    layout_importVideoStreamSetting->addWidget(isCamera2);


    QHBoxLayout *layout_count = new QHBoxLayout;
    spinBox_countFrame = new QSpinBox();
    QLabel* label_frame = new QLabel("Quantity of images:");
    layout_count->addWidget(label_frame);
    layout_count->addWidget(spinBox_countFrame);
    spinBox_countFrame->setMinimum(1);
    spinBox_countFrame->setMaximum(1000);
    layout_importVideoStreamSetting ->addLayout(layout_count);

    isDrawing = new QCheckBox("Drawing a template");
    layout_importVideoStreamSetting->addWidget(isDrawing);

    layout_importVideoStreamSetting->addWidget(btnStartVideoStream,0,Qt::AlignRight);

    groupBoxCamera->setLayout(layout_importVideoStreamSetting);

    QHBoxLayout* layout_main = new QHBoxLayout;   //Горизонтальный
    layout_main->addWidget(groupBoxPath);
    layout_main->addWidget(groupBoxCamera);

    layout_main->setSizeConstraint(QLayout::SetFixedSize);

    setLayout(layout_main);
}

void DialogWindowImportImage::setFileSystem(FileSystem *fs)
{
    this->fs = fs;
}

void DialogWindowImportImage::on_btnStartVideoStream_clicked()
{
    if((fs->getIndexCameraFirst()!=-1 && isCamera1->checkState())||
       (fs->getIndexCameraSecond()!=-1 && isCamera2->checkState()))
    {
        emit signalVideoStream(spinBox_countFrame->value(),
                               isDrawing->checkState(),isCamera1->checkState(),isCamera2->checkState());
        close();
    }
    else if(fs->getIndexCameraFirst()==-1 && isCamera1->checkState())
        QMessageBox::warning(this, tr("Warning"),tr("No selected Camera1"));
    else if(fs->getIndexCameraSecond()==1 && isCamera2->checkState())
        QMessageBox::warning(this, tr("Warning"),tr("No selected Camera2"));

}

void DialogWindowImportImage::on_btnSetFolderPath1_clicked()
{
    pathName1 = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                         "/home",
                                                         QFileDialog::ShowDirsOnly
                                                         |QFileDialog::DontResolveSymlinks
                                                         |QFileDialog::DontUseNativeDialog);
    line_folderPath1->setText(pathName1);
}

void DialogWindowImportImage::on_btnSetFolderPath2_clicked()
{
    pathName2 = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                         "/home",
                                                         QFileDialog::ShowDirsOnly
                                                         |QFileDialog::DontResolveSymlinks
                                                         |QFileDialog::DontUseNativeDialog);
    line_folderPath2->setText(pathName2);
}

void DialogWindowImportImage::on_btnSetOk_clicked()
{
    fs->copyDirImgInWorkDir(pathName1,pathName2);
    close();
}





