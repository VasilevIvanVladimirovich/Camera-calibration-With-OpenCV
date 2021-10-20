#ifndef SETIMGWINDOW_H
#define SETIMGWINDOW_H

#include <QMainWindow>

namespace Ui {
class SetImgWindow;
}

class SetImgWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SetImgWindow(QWidget *parent = nullptr);
    ~SetImgWindow();

private:
    Ui::SetImgWindow *ui;
};

#endif // SETIMGWINDOW_H
