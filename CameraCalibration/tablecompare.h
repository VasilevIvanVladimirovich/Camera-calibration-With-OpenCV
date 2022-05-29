#ifndef TABLECOMPARE_H
#define TABLECOMPARE_H

#include <QWidget>
#include <QtWidgets>
#include <QMainWindow>
#include <QDialog>
#include <QTimer>
#include <QString>
#include <QThread>
#include <QPicture>
#include <QtGui>
#include <QFileDialog>
#include <QObject>
#include <QMessageBox>
#include <QTreeWidget>
#include <QListWidget>
#include <QMetaObject>

#include "filesystem.h"

class TableCompare: public QWidget
{
    Q_OBJECT
public:
    TableCompare(QWidget* parent = 0);
    ~TableCompare();

    void setDir(QString dir);

public slots:
    void addItem(QTableWidgetItem* itemFile,
                 QTableWidgetItem* SettingCam1,QTableWidgetItem* itemCamera1Rmse,
                 QTableWidgetItem* SettingCam2, QTableWidgetItem* itemCamera2Rmse,
                 QTableWidgetItem* itemStereoRmse);
    void updateTable();
private slots:
    void on_btnAddFile_clicked();
    void on_table_cellClicked(int,int);
signals:
    void addNewFile(QString);
    void openFile(QString);
private:
    FileSystem fs_;
    QPushButton* btnAddFile_;
    QTableWidget* table_;
};

#endif // TABLECOMPARE_H
