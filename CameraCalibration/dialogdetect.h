#ifndef DIALOGDETECT_H
#define DIALOGDETECT_H

#include <QDialog>

namespace Ui {
class DialogDetect;
}

class DialogDetect : public QDialog
{
    Q_OBJECT

public:
    explicit DialogDetect(QWidget *parent = nullptr);
    ~DialogDetect();
signals:
    void outTargetType(QString qstring);
    void outTargetSize(int row,int col);
    void outSubPixIter(int count);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DialogDetect *ui;
};

#endif // DIALOGDETECT_H
