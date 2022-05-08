#ifndef BASLERCAMERA_H
#define BASLERCAMERA_H

#include <QObject>
#include <QWidget>
#include <pylon/PylonIncludes.h>
#include <QImage>
#include <QTimer>

#define DOUBLE_MAX 100000
#define DOUBLE_MIN 0

class BaslerCamera : public QObject
{
    Q_OBJECT
public:
    explicit BaslerCamera(QObject *parent = 0);
    ~BaslerCamera();

    enum BaslerCamera_Type{
        Type_Basler_Freerun, //Установите внутренний триггер камеры
        Type_Basler_Line1, //Установите внешний триггер камеры
        Type_Basler_ExposureTimeAbs, //Установите время экспозиции камеры
        Type_Basler_GainRaw, //Установите коэффициент усиления камеры
        Type_Basler_AcquisitionFrameRateAbs, //Установите частоту камеры
        Type_Basler_Width, //Ширина изображения
        Type_Basler_Height, //Высота изображения
        Type_Basler_LineSource, //Сигнал срабатывания светового
    };

    void initSome(); // заполняет и обновляет список доступных камер basler
    void deleteAll(); //завершить работу с камерой
    int CloseCamera(); // закрыть камеру
    int OpenCamera(QString cameraSN); //открыть камеру
    QStringList cameras(); //спикок камер

    void setExposureTime(double time); // Установить время экспозиции
    int getExposureTime(); // Получить время экспозиции
    int getExposureTimeMin(); // Минимальное время экспозиции
    int getExposureTimeMax(); // Максимальное время экспозиции

    void SetCamera(BaslerCamera::BaslerCamera_Type index, double tmpValue = 0.0); // Установка различных параметров
    double GetCamera(BaslerCamera::BaslerCamera_Type index); // Получить различные параметры

    QString getFeatureTriggerSourceType(); // Типы получения: мягкий триггер, внешний триггер и т.д.
    void setFeatureTriggerModeType(bool on); // Установить триггер режима

    long GrabImage(QImage& image,int timeout = 2000);
    long StartAcquire(); // Начать сбор
    long StopAcquire(); // Остановить поток накопления изображений
private:
    void UpdateCameraList();
    void CopyToImage(Pylon::CGrabResultPtr pInBuffer, QImage &OutImage);
signals:
    void sigCameraUpdate(QStringList list);
    void sigSizeChange(QSize size);
    void sigCameraCount(int count);
    void sigCurrentImage(QImage img);

private slots:
    void onTimerGrabImage();

private:
    Pylon::CInstantCamera m_basler;
    QStringList m_cameralist;
    QString m_currentMode;
    bool m_isOpenAcquire = false; // Стоит ли начинать сбор
    bool m_isOpen = false; // Следует ли включать камеру
    QSize m_size;
};

#endif // BASLERCAMERA_H
