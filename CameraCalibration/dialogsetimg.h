#ifndef DIALOGSETIMG_H
#define DIALOGSETIMG_H

#include <QDialog>
#include <QFileDialog>

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

signals:
    void outFolderPath(QString path);
    void setTableItems();
    void signalVideoStream(int countframe);

private slots:
    void on_btn_findpath_clicked();

    void on_buttonBox_accepted();

    void on_btn_getCameraImg_clicked();

private:
    Ui::DialogSetImg *ui;
    FileSystem fs;
    QString pathName;

};

#endif // DIALOGSETIMG_H
