#include "dialogwindowimportimage.h"

DialogWindowImportImage::DialogWindowImportImage(QWidget* parent): QWidget(parent)
{
    resize(500,600);
    btnStartVideoStream = new QPushButton("StartVideoStream");
    btnSetFolderPath = new QPushButton("...");
    btnSetOk = new QPushButton("Ok");
    btnSetCancel = new QPushButton("Cancel");

    connect(btnStartVideoStream, SIGNAL(clicked()), this, SLOT(on_btnStartVideoStream_clicked()));
    connect(btnSetFolderPath, SIGNAL(clicked()), this, SLOT(on_btnSetFolderPath_clicked()));
    connect(btnSetOk, SIGNAL(clicked()), this, SLOT(on_btnSetOk_clicked()));
    connect(btnSetCancel, SIGNAL(clicked()), this, SLOT(on_btnSetCancel_clicked()));

    QVBoxLayout* layout_menu = new QVBoxLayout;
    QFormLayout *layout_importVideoStreamSetting = new QFormLayout;

    spinBox_frameRate = new QSpinBox();
    layout_importVideoStreamSetting->addRow("Frame rate of file saving:", spinBox_frameRate);

    spinBox_countFrame = new QSpinBox();
    layout_importVideoStreamSetting->addRow("Count frame:", spinBox_countFrame);
    QFormLayout *layout_chekingPatternSetting = new QFormLayout;
    checkBox_isSnapShoot = new QCheckBox;
    layout_chekingPatternSetting->addRow(tr("&frame by button:"), checkBox_isSnapShoot);
    QVBoxLayout* layout_selectFolder = new QVBoxLayout;
    label_headerSelectFolder = new QLabel;
    label_headerSelectFolder->setText("Select image from a folder");
    layout_selectFolder->addWidget(label_headerSelectFolder);

    QHBoxLayout* layout_inputSelectFolder = new QHBoxLayout;

    label_folderPath = new QLabel;
    label_folderPath->setText("Folder path");

    line_folderPath = new QLineEdit;

    layout_inputSelectFolder->addWidget(label_folderPath);
    layout_inputSelectFolder->addWidget(line_folderPath);
    layout_inputSelectFolder->addWidget(btnSetFolderPath);

    layout_selectFolder->addLayout(layout_inputSelectFolder);

    QHBoxLayout* layout_okCancel = new QHBoxLayout;

    layout_okCancel->addWidget(btnSetOk);
    layout_okCancel->addWidget(btnSetCancel);

    layout_menu->addLayout(layout_importVideoStreamSetting);
    layout_menu->addLayout(layout_chekingPatternSetting);
    layout_menu->addWidget(btnStartVideoStream);
    layout_menu->addLayout(layout_selectFolder);
    layout_menu->addLayout(layout_okCancel);

    QHBoxLayout* layout_main = new QHBoxLayout;   //Горизонтальный
    layout_main->addLayout(layout_menu);

    setLayout(layout_main);
}

DialogWindowImportImage::~DialogWindowImportImage()
{

}

void DialogWindowImportImage::setFileSystem(FileSystem *fs)
{
    this->fs = fs;
}

void DialogWindowImportImage::on_btnStartVideoStream_clicked()
{
    emit signalVideoStream(spinBox_frameRate->value(),
                           spinBox_countFrame->value(),
                           checkBox_isSnapShoot->checkState());
    close();
}

void DialogWindowImportImage::on_btnSetFolderPath_clicked()
{
    pathName = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                         "/home",
                                                         QFileDialog::ShowDirsOnly
                                                         |QFileDialog::DontResolveSymlinks);
    line_folderPath->setText(pathName);
}

void DialogWindowImportImage::on_btnSetOk_clicked()
{
    fs->copyDirImgInWorkDir(pathName);
    fs->getTableItems();
    close();
}

void DialogWindowImportImage::on_btnSetCancel_clicked()
{
    close();
}




