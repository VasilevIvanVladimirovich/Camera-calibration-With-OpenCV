#include <QCoreApplication>

#include <pylon/PylonIncludes.h>
#include <pylon/ImageEventHandler.h>
#include <pylon/ConfigurationEventHandler.h>
#include <pylon/PylonGUIIncludes.h>
#include <pylon/GrabResultPtr.h>
#include <pylon/PylonGUI.h>
#include <pylon/PylonBase.h>

#include <QApplication>
#include "mainwindow.h"
#include "mybaslercamera.h"


//class CSampleImageEventHandler : public Pylon::CImageEventHandler
//{
//public:
//    virtual void OnImageGrabbed( Pylon::CInstantCamera& /*camera*/, const Pylon::CGrabResultPtr& ptrGrabResult )
//    {
//        qDebug()<<"CSampleImageEventHandler::OnImageGrabbed called.";
//        // Display the image
//        Pylon::DisplayImage( 1, ptrGrabResult);
//    }
//};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();



//        int exitCode = 0;

//        //инициализация среды
//        Pylon::PylonInitialize();

//        try
//        {
//            // Создайте объект мгновенной камеры для устройства камеры, найденного первым.
//            Pylon::CInstantCamera camera( Pylon::CTlFactory::GetInstance().CreateFirstDevice() );

//            // Зарегистрируйте стандартный обработчик событий конфигурации для включения запуска программного обеспечения.
//            // Обработчик конфигурации программного триггера заменяет конфигурацию по умолчанию
//            // поскольку все зарегистрированные в данный момент обработчики конфигурации удаляются путем установки режима регистрации на RegistrationMode_ReplaceAll.
//            camera.RegisterConfiguration( new Pylon::CSoftwareTriggerConfiguration, Pylon::RegistrationMode_ReplaceAll, Pylon::Cleanup_Delete );

//            // Только в демонстрационных целях добавьте образец обработчика событий конфигурации для распечатки информации
//            // об использовании камеры.
//            camera.RegisterConfiguration( new Pylon::CConfigurationEventHandler, Pylon::RegistrationMode_Append, Pylon::Cleanup_Delete );

//            // Принтер событий изображения служит в качестве образца обработки изображений.
//            // При использовании потока цикла захвата, предоставляемого объектом мгновенной камеры, обработчик события изображения обрабатывает захват
//            // результаты должны быть созданы и зарегистрированы.
//            camera.RegisterImageEventHandler( new Pylon::CImageEventHandler, Pylon::RegistrationMode_Append, Pylon::Cleanup_Delete );

//            // Только в демонстрационных целях зарегистрируйте другой обработчик событий изображения.
//            camera.RegisterImageEventHandler( new CSampleImageEventHandler, Pylon::RegistrationMode_Append, Pylon::Cleanup_Delete );

//            // Откройте устройство камеры.
//            camera.Open();

//            // Можно ли запросить устройство камеры, готово ли оно принять триггер следующего кадра?
//            if (camera.CanWaitForFrameTriggerReady())
//            {
//                // Запустите захват с помощью потока цикла захвата, установив параметр grabLoopType
//                // к GrabLoop_ProvidedByInstantCamera. Результаты захвата передаются обработчикам событий изображений.
//                // Используется стратегия захвата GrabStrategy_OneByOne по умолчанию.
//                camera.StartGrabbing( Pylon::GrabStrategy_OneByOne, Pylon::GrabLoop_ProvidedByInstantCamera );

//                // Дождитесь ввода пользователем, чтобы запустить камеру или выйти из программы.
//                // Захват прекращается, устройство закрывается и автоматически уничтожается, когда объект камеры выходит из поля зрения.

//                bool runLoop = true;
//                while (runLoop)
//                {
//                    if (camera.WaitForFrameTriggerReady( 1000, Pylon::TimeoutHandling_ThrowException ))
//                    {
//                        camera.ExecuteSoftwareTrigger();
//                    }
//                    // Подождите некоторое время, чтобы разрешить обработчику OnImageGrabbed распечатать свои выходные данные,
//                    // чтобы напечатанный текст на консоли был в ожидаемом порядке.
//                    Pylon::WaitObject::Sleep( 250 );
//                }
//            }
//            else
//            {
//                //См. Документацию CInstantCamera::CanWaitForFrameTriggerReady() для получения дополнительной информации.
//                qDebug()<<"This sample can only be used with cameras that can be queried whether they are ready to accept the next frame trigger.";
//            }
//        }
//        catch (const Pylon::GenericException& e)
//        {
//            // Error handling.
//            qDebug()<<"An exception occurred." << e.GetDescription();
//            exitCode = 1;
//        }
//        // Освобождает все ресурсы пилона.
//        Pylon::PylonTerminate();

//        return exitCode;
}
