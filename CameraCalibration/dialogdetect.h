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
private:
    Ui::DialogDetect *ui;

};

#endif // DIALOGDETECT_H
