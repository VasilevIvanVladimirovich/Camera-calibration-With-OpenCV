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

public slots:
    void addItem(QTableWidgetItem* itemFile,QTableWidgetItem* itemDate,
                 QTableWidgetItem* itemCount,QTableWidgetItem* itemPattern,
                 QTableWidgetItem* itemSizePattern, QTableWidgetItem* itemRmse);
private slots:
    void on_btnAddFile_clicked();
    void on_btnOpenDir_clicked();
    void on_btnClearTable_clicked();
private:
    FileSystem fs_;
    QPushButton* btnClearTable_;
    QPushButton* btnOpenDir_;
    QPushButton* btnAddFile_;
    QTableWidget* table_;
};

#endif // TABLECOMPARE_H
