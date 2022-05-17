#ifndef SETTINGPATTERNWINDOW_H
#define SETTINGPATTERNWINDOW_H

#include <QObject>
#include <QWidget>
#include <QTreeWidget>
#include <QListWidget>
#include <QSpinBox>
#include <QFormLayout>
#include <QLabel>
#include <QGroupBox>
#include <QPushButton>
#include <QComboBox>
#include "filesystem.h"

class SettingPatternWindow : public QWidget
{
    Q_OBJECT
public:
    SettingPatternWindow(FileSystem* fs);
    ~SettingPatternWindow();

signals:
    void sendUpdate();
private slots:
    void on_tree_inputPatern_clicked(QTreeWidgetItem* item, int col);
    void on_tree_tree_inputDictionaryName_clicked(QTreeWidgetItem* item, int col);
    void on_btnSetOk_clicked();
    void on_btnSetCancel_clicked();
    void on_btnSetNewTemp_clicked();
    void on_btnSetDelTemp_clicked();
    void on_box_templatesPattern_IndexChanged(int);
private:
    void initUi();
    void updateTemplates();
private:
    QPushButton* btnSetOk;
    QPushButton* btnSetCancel;

    QPushButton* btnNewTemplates;
    QPushButton* btnDelTemplates;

    QTreeWidget* tree_inputDictionaryName;
    QTreeWidget* tree_inputPatern;

    QSpinBox* spinBox_rowCount;
    QSpinBox* spinBox_colCount;
    QSpinBox* spinBox_checkerSize;
    QSpinBox* spinBox_markerSize;

    QComboBox* templatesPattern;

    QLabel* label_row;
    QLabel* label_col;
    QLabel* label_checkerSize;
    QLabel* label_markerSize;

    FileSystem *fileSystem_;
};

#endif // SETTINGPATTERNWINDOW_H
