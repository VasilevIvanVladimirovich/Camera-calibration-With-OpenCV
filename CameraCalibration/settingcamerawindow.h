#ifndef SETTINGCAMERAWINDOW_H
#define SETTINGCAMERAWINDOW_H

#include <QObject>
#include <QWidget>

#include <QFormLayout>
#include <QComboBox>
#include <QCameraDevice>
#include <QMediaDevices>
#include <QGroupBox>
#include <QPushButton>
#include <QDoubleSpinBox>

#include <pylon/PylonIncludes.h>
#include <pylon/ImageEventHandler.h>
#include <pylon/ConfigurationEventHandler.h>
#include <pylon/PylonGUIIncludes.h>
#include <pylon/GrabResultPtr.h>
#include <pylon/PylonGUI.h>
#include <pylon/PylonBase.h>
#include <pylon/BaslerUniversalInstantCamera.h>
#include "filesystem.h"

class SettingCameraWindow : public QWidget
{
    Q_OBJECT
public:
    SettingCameraWindow(FileSystem* fs);
    ~SettingCameraWindow();

private:
    void initUi();

signals:
    void sendUpdate();
    void sendTerminalStr(QString);
private slots:
   void on_box_cameraInput_IndexChanged(int index);
   void on_box_cameraFirst_IndexChanged(int index);

   void on_btnSetOk_clicked();
   void on_btnSetCancel_clicked();
private:
   QPushButton* btnSetOk;
   QPushButton* btnSetCancel;

   QComboBox* box_cameraInput;
   QComboBox* box_cameraFirst;
   QComboBox* box_cameraSecond;

   QDoubleSpinBox* expose;
   QDoubleSpinBox* gain;

   FileSystem *fileSystem_;
};

#endif // SETTINGCAMERAWINDOW_H
