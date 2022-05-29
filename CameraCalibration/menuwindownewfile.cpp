#include "menuwindownewfile.h"

MenuWindowNewFile::MenuWindowNewFile(QWidget* parent): QWidget(parent)
{
    initUi();
}

MenuWindowNewFile::~MenuWindowNewFile()
{

}

void MenuWindowNewFile::setDirs(QString str)
{
    pathfolder = str;
    line_folderPath->setText(pathfolder);
}

void MenuWindowNewFile::initUi()
{
    resize(500, 200);

    btnSetFolderPath = new QPushButton("...");
    btnSetOk = new QPushButton("Ok");
    btnSetCancel = new QPushButton("Cancel");

    connect(btnSetOk, SIGNAL(clicked()), this, SLOT(on_btnSetOk_clicked()));
    connect(btnSetCancel, SIGNAL(clicked()), this, SLOT(on_btnSetCancel_clicked()));
    connect(btnSetFolderPath, SIGNAL(clicked()), this, SLOT(on_btnSetFolderPath_clicked()));

    QGroupBox *groupBox= new QGroupBox(tr("Create new project"));
    groupBox->setAlignment(Qt::AlignCenter);

    QVBoxLayout* layout_menu = new QVBoxLayout;

    QHBoxLayout* layout_inputSelectFolder = new QHBoxLayout;

    label_folderPath = new QLabel;
    label_folderPath->setText("Select a folder: ");

    line_folderPath = new QLineEdit;
    line_folderPath->isReadOnly();

    layout_inputSelectFolder->addWidget(label_folderPath);
    layout_inputSelectFolder->addWidget(line_folderPath);
    layout_inputSelectFolder->addWidget(btnSetFolderPath);

    QHBoxLayout* layout_inputNameFolder = new QHBoxLayout;

    label_folderName = new QLabel;
    label_folderName->setText("Select a name ");

    line_nameFolder = new QLineEdit;
    layout_inputNameFolder->addWidget(label_folderName);
    layout_inputNameFolder->addWidget(line_nameFolder);

    QHBoxLayout* layout_btnOkCancel = new QHBoxLayout;
    layout_btnOkCancel->addWidget(btnSetOk);
    layout_btnOkCancel->addWidget(btnSetCancel);

    QWidget* buttonWidget = new QWidget();
    buttonWidget->setLayout(layout_btnOkCancel);


    layout_menu->addLayout(layout_inputSelectFolder);
    layout_menu->addLayout(layout_inputNameFolder);
    layout_menu->addWidget(buttonWidget,0, Qt::AlignRight);

    groupBox->setLayout(layout_menu);

    QHBoxLayout* layout_out = new QHBoxLayout;
    layout_out->addWidget(groupBox);
    setLayout(layout_out);
}

void MenuWindowNewFile::on_btnSetOk_clicked()
{

    if(pathfolder.size()==0)
    {
        QMessageBox::warning(this, tr("Warning"),tr("The folder is none"));
        return;
    }
    if(line_nameFolder->text().size()==0)
    {
        QMessageBox::warning(this, tr("Warning"),tr("The name is none"));
        return;
    }

    if(QDir(pathfolder + "/" + line_nameFolder->text()).exists())
    {
        QMessageBox::warning(this, tr("Warning"),tr("The project is exists"));
        return;
    }

    fileSystem_.setPath(pathfolder + "/" + line_nameFolder->text() +"/");
    fileSystem_.createWorkDir(2);
    fileSystem_.createSettingDefault();
    emit setPathDir(fileSystem_.getFilePath());
    close();
}

void MenuWindowNewFile::on_btnSetCancel_clicked()
{
    close();
}

void MenuWindowNewFile::on_btnSetFolderPath_clicked()
{
    pathfolder = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                         "/home",
                                                         QFileDialog::ShowDirsOnly
                                                         |QFileDialog::DontResolveSymlinks
                                                        |QFileDialog::DontUseNativeDialog);
    line_folderPath->setText(pathfolder);
}


void MenuWindowNewFile::on_tree_inputPatern_clicked(QTreeWidgetItem* item, int col)
{
    spinBox_rowCount->setEnabled(false);
    spinBox_colCount->setEnabled(false);

    spinBox_markerSize->setEnabled(false);
    spinBox_checkerSize->setEnabled(false);
    tree_inputDictionaryName->setEnabled(false);

    pattern = item->text(0);
    if(pattern == "Chessboard" || pattern == "Circles" || pattern == "Assymetric Circles")
    {
        spinBox_rowCount->setEnabled(true);
        spinBox_colCount->setEnabled(true);
    }
    if(pattern == "ChArUco")
    {;
        spinBox_rowCount->setEnabled(true);
        spinBox_colCount->setEnabled(true);

        spinBox_markerSize->setEnabled(true);
        spinBox_checkerSize->setEnabled(true);
        tree_inputDictionaryName->setEnabled(true);
    }
}

void MenuWindowNewFile::on_box_cameraFirst_IndexChanged(int index)
{
    if(index ==-1 || index ==0) box_cameraSecond->setEnabled(false);
    else box_cameraSecond->setEnabled(true);
}

void MenuWindowNewFile::on_box_cameraInput_IndexChanged(int index)
{

}
