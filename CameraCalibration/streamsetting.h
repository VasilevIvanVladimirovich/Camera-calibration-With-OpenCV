#ifndef STREAMSETTING_H
#define STREAMSETTING_H

#include <QObject>
#include <QWidget>

#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "filesystem.h"

class StreamSetting : public QWidget
{
    Q_OBJECT
public:
    StreamSetting(FileSystem* fs);
    ~StreamSetting();

private:
    void initUi();
    void createAction();
    void fillBoxSetting();
private slots:
    void on_btn_ok_clicked();
    void on_btn_cencel_clicked();
signals:
    void sendState(QString current);
private:
    QComboBox* box_setting;

    QPushButton* btn_ok;
    QPushButton* btn_cencel;

    FileSystem *fileSystem_;
};

#endif // STREAMSETTING_H
