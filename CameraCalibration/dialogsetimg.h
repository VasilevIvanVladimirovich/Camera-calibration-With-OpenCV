#ifndef DIALOGSETIMG_H
#define DIALOGSETIMG_H

#include <QDialog>
#include <QFileDialog>
#include <QTreeWidget>

#include "filesystem.h"



namespace Ui {
class DialogSetImg;
}

class DialogSetImg : public QDialog
{
    Q_OBJECT
public:
    explicit DialogSetImg(QWidget *parent = nullptr);
    ~DialogSetImg();

    void setFileSystem(FileSystem* fs);


signals:
    void signalVideoStream(int frameRate, int countframe, int row,
                           int col, bool isPattern, bool isSnapshoot, QString pattern);

private slots:
    void on_btn_findpath_clicked();

    void on_buttonBox_accepted();

    void on_btn_getCameraImg_clicked();

private:
    Ui::DialogSetImg *ui;
    FileSystem* fs;
    QString pathName;

};

#endif // DIALOGSETIMG_H
