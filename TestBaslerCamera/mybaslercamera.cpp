#include "mybaslercamera.h"

MyBaslerCamera::MyBaslerCamera()
{
    Pylon::PylonInitialize();
}

MyBaslerCamera::~MyBaslerCamera()
{
     Pylon::PylonTerminate();
}

void MyBaslerCamera::init()
{
    qDebug()<<"init";
    // Создайте объект мгновенной камеры для устройства камеры, найденного первым.
    baslerCamera_ = new Pylon::CBaslerUniversalInstantCamera(Pylon::CTlFactory::GetInstance().CreateFirstDevice());
    qDebug()<<baslerCamera_->GetDeviceInfo().GetFriendlyName();
    qDebug()<<baslerCamera_->GetDeviceInfo().GetModelName();

    // Зарегистрируйте стандартный обработчик событий конфигурации для включения запуска программного обеспечения.
    // Обработчик конфигурации программного триггера заменяет конфигурацию по умолчанию
    // поскольку все зарегистрированные в данный момент обработчики конфигурации удаляются путем установки режима регистрации на RegistrationMode_ReplaceAll.
    baslerCamera_->RegisterConfiguration( new Pylon::CSoftwareTriggerConfiguration, Pylon::RegistrationMode_ReplaceAll, Pylon::Cleanup_Delete );

    // Только в демонстрационных целях добавьте образец обработчика событий конфигурации для распечатки информации
    // об использовании камеры.
    baslerCamera_->RegisterConfiguration( new Pylon::CConfigurationEventHandler, Pylon::RegistrationMode_Append, Pylon::Cleanup_Delete );

    // Принтер событий изображения служит в качестве образца обработки изображений.
    // При использовании потока цикла захвата, предоставляемого объектом мгновенной камеры, обработчик события изображения обрабатывает захват
    // результаты должны быть созданы и зарегистрированы.
    // baslerCamera_->RegisterImageEventHandler( new Pylon::CImageEventHandler, Pylon::RegistrationMode_Append, Pylon::Cleanup_Delete );

    // Только в демонстрационных целях зарегистрируйте другой обработчик событий изображения.
    baslerCamera_->RegisterImageEventHandler(this, Pylon::RegistrationMode_Append, Pylon::Cleanup_Delete );

    // Откройте устройство камеры.
    baslerCamera_->Open();

    if (!baslerCamera_->UserSetSelector.IsWritable())
    {
        throw RUNTIME_EXCEPTION( "The device doesn't support user sets." );
    }

    // Used for USB cameras
    oldDefaultUserSet = Basler_UniversalCameraParams::UserSetDefault_Default;

    // Used for GigE cameras
    oldDefaultUserSetSelector = Basler_UniversalCameraParams::UserSetDefaultSelector_Default;

    if (baslerCamera_->UserSetDefault.IsReadable()) // Cameras based on SFNC 2.0 or later, e.g., USB cameras
    {
        qDebug()<<"true";
        oldDefaultUserSet = baslerCamera_->UserSetDefault.GetValue();
    }else
    {
        qDebug()<<"false";
        oldDefaultUserSetSelector = baslerCamera_->UserSetDefaultSelector.GetValue();
    }

    qDebug() << "Loading default settings";
    baslerCamera_->UserSetSelector.SetValue( Basler_UniversalCameraParams::UserSetSelector_Default );
    baslerCamera_->UserSetLoad.Execute();

    //отключение камеры для её настройки
    if (baslerCamera_->Gain.IsWritable()) // Cameras based on SFNC 2.0 or later, e.g., USB cameras
    {
        baslerCamera_->GainAuto.TrySetValue(  Basler_UniversalCameraParams::GainAuto_Off );
        baslerCamera_->Gain.SetValue( baslerCamera_->Gain.GetMin() );
        baslerCamera_->ExposureAuto.TrySetValue(  Basler_UniversalCameraParams::ExposureAuto_Off );
        baslerCamera_->ExposureTime.SetValue( baslerCamera_->ExposureTime.GetMin() );
    }
    else{
        baslerCamera_->GainAuto.TrySetValue(Basler_UniversalCameraParams::GainAuto_Off);
        baslerCamera_->GainRaw.SetValue( baslerCamera_->GainRaw.GetMin() );
        baslerCamera_->ExposureAuto.TrySetValue(Basler_UniversalCameraParams::ExposureAuto_Off);
        baslerCamera_->ExposureTimeRaw.SetValue(baslerCamera_->ExposureTimeRaw.GetMin());
    }

    baslerCamera_->UserSetSelector.SetValue(Basler_UniversalCameraParams::UserSetSelector_UserSet1);
    baslerCamera_->UserSetSave.Execute();

    // Show default settings.
    qDebug()<<"Loading default settings.";
    baslerCamera_->UserSetSelector.SetValue(Basler_UniversalCameraParams::UserSetSelector_Default);
    baslerCamera_->UserSetLoad.Execute();
    qDebug()<<"Default settings";
    qDebug()<<"================";
    if (baslerCamera_->Gain.IsReadable()) // Cameras based on SFNC 2.0 or later, e.g., USB cameras
    {
        qDebug()<< "Gain          : " << baslerCamera_->Gain.GetValue();
        qDebug()<< "Exposure time : " << baslerCamera_->ExposureTime.GetValue();
    }
    else
    {
        qDebug()<< "Gain          : " << baslerCamera_->GainRaw.GetValue();
        qDebug()<< "Exposure time : " << baslerCamera_->ExposureTimeRaw.GetValue();
    }

    // Show user set 1 settings.
    qDebug()<< "Loading user set 1 settings.";
    baslerCamera_->UserSetSelector.SetValue( Basler_UniversalCameraParams::UserSetSelector_UserSet1 );
    baslerCamera_->UserSetLoad.Execute();
    qDebug()<< "User set 1 settings";
    qDebug()<< "===================";

    if (baslerCamera_->Gain.IsReadable()) // Cameras based on SFNC 2.0 or later, e.g., USB cameras
    {
        qDebug()<< "Gain          : " << baslerCamera_->Gain.GetValue();
        qDebug()<< "Exposure time : " << baslerCamera_->ExposureTime.GetValue();
    }
    else
    {
        qDebug()<< "Gain          : " << baslerCamera_->GainRaw.GetValue();
        qDebug()<< "Exposure time : " << baslerCamera_->ExposureTimeRaw.GetValue();
    }

    if (baslerCamera_->UserSetDefault.IsWritable()) // Cameras based on SFNC 2.0 or later, e.g., USB cameras
    {
        baslerCamera_->UserSetDefault.SetValue( Basler_UniversalCameraParams::UserSetDefault_UserSet1 );

        // Restore the default user set selector.
        baslerCamera_->UserSetDefault.SetValue( oldDefaultUserSet );
    }
    else
    {
        // Set user set 1 as default user set:
        // When the camera wakes up it will be configured
        // with the settings from user set 1.
        baslerCamera_->UserSetDefaultSelector.SetValue( Basler_UniversalCameraParams::UserSetDefaultSelector_UserSet1 );

        // Restore the default user set selector.
        baslerCamera_->UserSetDefaultSelector.SetValue( oldDefaultUserSetSelector );
    }
    baslerCamera_->Close();
}

double MyBaslerCamera::getExposureTime()
{
    double exposureTime;
    baslerCamera_->Open();

    if (baslerCamera_->Gain.IsReadable()) // Cameras based on SFNC 2.0 or later, e.g., USB cameras
    {
        exposureTime = baslerCamera_->ExposureTime.GetValue();
    }
    else
    {
        exposureTime = baslerCamera_->ExposureTimeRaw.GetValue();
    }

    baslerCamera_->Close();
    return exposureTime;
}

double MyBaslerCamera::getGain()
{
    double gain;
    baslerCamera_->Open();

    if (baslerCamera_->Gain.IsReadable()) // Cameras based on SFNC 2.0 or later, e.g., USB cameras
    {
        gain = baslerCamera_->Gain.GetValue();
    }
    else
    {
        gain = baslerCamera_->GainRaw.GetValue();
    }

    baslerCamera_->Close();
    return gain;
}

void MyBaslerCamera::setSetting(double exposure, double gain)
{
    baslerCamera_->Open();

    baslerCamera_->UserSetSelector.SetValue( Basler_UniversalCameraParams::UserSetSelector_UserSet1 );
    baslerCamera_->UserSetLoad.Execute();

    baslerCamera_->ExposureTime.SetValue(exposure);
    baslerCamera_->Gain.SetValue(gain);

    if (baslerCamera_->UserSetDefault.IsWritable()) // Cameras based on SFNC 2.0 or later, e.g., USB cameras
    {
        baslerCamera_->UserSetDefault.SetValue( Basler_UniversalCameraParams::UserSetDefault_UserSet1 );

        // Restore the default user set selector.
        baslerCamera_->UserSetDefault.SetValue( oldDefaultUserSet );
    }
    else
    {
        // Set user set 1 as default user set:
        // When the camera wakes up it will be configured
        // with the settings from user set 1.
        baslerCamera_->UserSetDefaultSelector.SetValue( Basler_UniversalCameraParams::UserSetDefaultSelector_UserSet1 );

        // Restore the default user set selector.
        baslerCamera_->UserSetDefaultSelector.SetValue(oldDefaultUserSetSelector);
    }
    baslerCamera_->Close();
}

void MyBaslerCamera::run()
{
    baslerCamera_->Open();
    qDebug()<<"run";
    qDebug()<< "===================";

    if (baslerCamera_->Gain.IsReadable()) // Cameras based on SFNC 2.0 or later, e.g., USB cameras
    {
        qDebug()<< "Gain          : " << baslerCamera_->Gain.GetValue();
        qDebug()<< "Exposure time : " << baslerCamera_->ExposureTime.GetValue();
    }
    else
    {
        qDebug()<< "Gain          : " << baslerCamera_->GainRaw.GetValue();
        qDebug()<< "Exposure time : " << baslerCamera_->ExposureTimeRaw.GetValue();
    }
    // Можно ли запросить устройство камеры, готово ли оно принять триггер следующего кадра?
    if (baslerCamera_->CanWaitForFrameTriggerReady())
    {
        // Запустите захват с помощью потока цикла захвата, установив параметр grabLoopType
        // к GrabLoop_ProvidedByInstantCamera. Результаты захвата передаются обработчикам событий изображений.
        // Используется стратегия захвата GrabStrategy_OneByOne по умолчанию.
        baslerCamera_->StartGrabbing( Pylon::GrabStrategy_OneByOne, Pylon::GrabLoop_ProvidedByInstantCamera );

        // Дождитесь ввода пользователем, чтобы запустить камеру или выйти из программы.
        // Захват прекращается, устройство закрывается и автоматически уничтожается, когда объект камеры выходит из поля зрения.

        bool runLoop = true;
        while (runLoop)
        {
            if (baslerCamera_->WaitForFrameTriggerReady(1000, Pylon::TimeoutHandling_ThrowException))
            {
                baslerCamera_->ExecuteSoftwareTrigger();
            }
            // Подождите некоторое время, чтобы разрешить обработчику OnImageGrabbed распечатать свои выходные данные,
            // чтобы напечатанный текст на консоли был в ожидаемом порядке.
            Pylon::WaitObject::Sleep( 250 );
        }
    }else
    {
        try{
            baslerCamera_->TriggerSelector.SetValue(Basler_UniversalCameraParams::TriggerSelector_FrameStart);
            baslerCamera_->TriggerMode.SetValue(Basler_UniversalCameraParams::TriggerMode_On);
            baslerCamera_->TriggerSelector.SetValue(Basler_UniversalCameraParams::TriggerSelector_FrameBurstStart);
            baslerCamera_->TriggerMode.SetValue(Basler_UniversalCameraParams::TriggerMode_On);

            baslerCamera_->AcquisitionBurstFrameCount.SetValue(3);

        qDebug()<<"else";
        Pylon::CGrabResultPtr ptrGrabResult;
        qDebug()<<"CGrabResultPtr";
        baslerCamera_->StartGrabbing();
        qDebug()<<"StartGrabbing";
        while(baslerCamera_->IsGrabbing()){
            qDebug()<<"IsGrabbing";
            baslerCamera_->RetrieveResult(5000, ptrGrabResult, Pylon::TimeoutHandling_ThrowException);
            if (ptrGrabResult->GrabSucceeded()){
                qDebug()<<"GrabSucceeded";
                Pylon::CPylonImage image;
                image.AttachGrabResultBuffer(ptrGrabResult);
                QImage img((uchar *)image.GetBuffer(),image.GetWidth(),image.GetHeight(),QImage::Format_Grayscale8);
                emit sendGrabb(img);
            }
        }
        }catch (GenICam::GenericException &e)
        {
            // Error handling.
            qDebug() << "An exception occurred.\n"
            << e.GetDescription();
        }

        //См. Документацию CInstantCamera::CanWaitForFrameTriggerReady() для получения дополнительной информации.
        qDebug()<<"This sample can only be used with cameras that can be queried whether they are ready to accept the next frame trigger.";
    }
    baslerCamera_->Close();
}


void MyBaslerCamera::OnImageGrabbed(Pylon::CInstantCamera& camera, const Pylon::CGrabResultPtr& ptrGrab)
{
    Pylon::CGrabResultPtr m_ptrGrabResult = ptrGrab;
    Pylon::CPylonImage image;
    image.AttachGrabResultBuffer(ptrGrab);
    QImage img((uchar *)image.GetBuffer(),image.GetWidth(),image.GetHeight(),QImage::Format_Grayscale8);
    emit sendGrabb(img);
}


