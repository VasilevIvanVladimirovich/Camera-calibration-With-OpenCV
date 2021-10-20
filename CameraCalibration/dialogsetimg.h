#ifndef DIALOGSETIMG_H
#define DIALOGSETIMG_H

#include <QDialog>

namespace Ui {
class DialogSetImg;
}

class DialogSetImg : public QDialog
{
    Q_OBJECT
public:
    explicit DialogSetImg(QWidget *parent = nullptr);
    ~DialogSetImg();

private:
    Ui::DialogSetImg *ui;
};

#endif // DIALOGSETIMG_H
