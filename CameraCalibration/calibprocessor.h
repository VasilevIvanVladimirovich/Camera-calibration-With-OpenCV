#ifndef CALIBPROCESSOR_H
#define CALIBPROCESSOR_H

#include <QObject>

class CalibProcessor : public QObject
{
    Q_OBJECT
public:
    explicit CalibProcessor(QObject *parent = nullptr);

signals:

};

#endif // CALIBPROCESSOR_H
