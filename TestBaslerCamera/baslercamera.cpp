#include "baslercamera.h"
#include <QDateTime>
#include <QDebug>

BaslerCamera::BaslerCamera(QObject *parent) : QObject(parent)
{

}

BaslerCamera::~BaslerCamera()
{
    deleteAll();
}

void BaslerCamera::initSome()
{
    qDebug() << "BaslerCamera: PylonInitialize initSome" ;
    Pylon::PylonInitialize();
    GenICam_3_1_Basler_pylon::SetGenICamLogConfig("D:/PRoG/CLib/baslerCamera/Development/Assemblies/Basler.Pylon/x86/Basler.Pylon.xml");
    Pylon::CTlFactory &TlFactory = Pylon::CTlFactory::GetInstance();
    Pylon::TlInfoList_t lstInfo;
    int n = TlFactory.EnumerateTls(lstInfo);

    Pylon::TlInfoList_t::const_iterator it;
    for ( it = lstInfo.begin(); it != lstInfo.end(); ++it )
    {
        qDebug() << "FriendlyName: " << it->GetFriendlyName() << "FullName: " << it->GetFullName();
        qDebug() << "VendorName: " << it->GetVendorName() << "DeviceClass: " << it->GetDeviceClass() ;
    }
    qDebug() << "before  UpdateCameraList";
    UpdateCameraList();
    qDebug() << "after  UpdateCameraList";
    //emit sigCameraCount(n);
    qDebug() << "BaslerCamera Count: " << n;
}

void BaslerCamera::deleteAll()
{
    //Прекратить сбор
    if(m_isOpenAcquire) {
        StopAcquire();
    }
    //Выключите камеру
    CloseCamera();
    //Закрыть библиотеку
    qDebug() << "SBaslerCameraControl deleteAll: PylonTerminate" ;
    Pylon::PylonTerminate();
    qDebug() << "SBaslerCameraControl deleteAll: Close" ;
}

int BaslerCamera::CloseCamera()
{
    if(!m_isOpen) {
        return -1;
    }
    try {
        if(m_basler.IsOpen()) {
            m_basler.DetachDevice();
            m_basler.Close();
        }
    } catch (GenICam::GenericException &e) {
//        OutputDebugString(LPCWSTR(e.GetDescription()));
        qDebug() << "CloseCamera Error:" << QString(e.GetDescription());
        return -2;
    }
    return 0;
}

int BaslerCamera::OpenCamera(QString cameraSN)
{
    try {
        Pylon::CDeviceInfo cInfo;
        Pylon::String_t str = Pylon::String_t(cameraSN.toStdString().c_str());
        cInfo.SetSerialNumber(str);
        m_basler.Attach(Pylon::CTlFactory::GetInstance().CreateDevice(cInfo));
        m_basler.Open();
        //Получить режим запуска
        getFeatureTriggerSourceType();
        m_isOpen = true;
    } catch (GenICam::GenericException &e) {
//        OutputDebugString(L"OpenCamera Error\n");
        qDebug() << "OpenCamera Error" << QString(e.GetDescription());
        m_isOpen = false;
        return -2;
    }
    return 0;
}

QStringList BaslerCamera::cameras()
{
    return m_cameralist;
}

void BaslerCamera::setExposureTime(double time)
{
    SetCamera(Type_Basler_ExposureTimeAbs, time);
}

int BaslerCamera::getExposureTime()
{
    return QString::number(GetCamera(BaslerCamera_Type::Type_Basler_ExposureTimeAbs)).toInt();
}

int BaslerCamera::getExposureTimeMin()
{
    return DOUBLE_MIN;
}

int BaslerCamera::getExposureTimeMax()
{
    return DOUBLE_MAX;
}

void BaslerCamera::SetCamera(BaslerCamera_Type index, double tmpValue)
{
    GenApi::INodeMap &cameraNodeMap = m_basler.GetNodeMap();
       switch (index) {
       case Type_Basler_Freerun: {
           GenApi::CEnumerationPtr  ptrTriggerSel = cameraNodeMap.GetNode ("TriggerSelector");
           ptrTriggerSel->FromString("FrameStart");
            GenApi::CEnumerationPtr  ptrTrigger  = cameraNodeMap.GetNode ("TriggerMode");
   #ifdef Real_Freerun
           ptrTrigger->SetIntValue(0);
   #else //Software
           ptrTrigger->SetIntValue(1);
            GenApi::CEnumerationPtr  ptrTriggerSource = cameraNodeMap.GetNode ("TriggerSource");
           ptrTriggerSource->FromString("Software");
   #endif
       } break;
       case Type_Basler_Line1: {
            GenApi::CEnumerationPtr  ptrTriggerSel = cameraNodeMap.GetNode ("TriggerSelector");
           ptrTriggerSel->FromString("FrameStart");
            GenApi::CEnumerationPtr  ptrTrigger  = cameraNodeMap.GetNode ("TriggerMode");
           ptrTrigger->SetIntValue(1);
            GenApi::CEnumerationPtr  ptrTriggerSource = cameraNodeMap.GetNode ("TriggerSource");
           ptrTriggerSource->FromString("Line1");
       } break;
       case Type_Basler_ExposureTimeAbs: {
           const GenApi::CFloatPtr exposureTime = cameraNodeMap.GetNode("ExposureTimeAbs");
           exposureTime->SetValue(tmpValue);
       } break;
       case Type_Basler_GainRaw: {
           const GenApi::CIntegerPtr cameraGen = cameraNodeMap.GetNode("GainRaw");
           cameraGen->SetValue(tmpValue);
       } break;
       case Type_Basler_AcquisitionFrameRateAbs: {
           const GenApi::CBooleanPtr frameRate = cameraNodeMap.GetNode("AcquisitionFrameRateEnable");
           frameRate->SetValue(TRUE);
           const GenApi::CFloatPtr frameRateABS = cameraNodeMap.GetNode("AcquisitionFrameRateAbs");
           frameRateABS->SetValue(tmpValue);
       } break;
       case Type_Basler_Width: {
           const GenApi::CIntegerPtr widthPic = cameraNodeMap.GetNode("Width");
           widthPic->SetValue(tmpValue);
       } break;
       case Type_Basler_Height: {
           const GenApi::CIntegerPtr heightPic = cameraNodeMap.GetNode("Height");
           heightPic->SetValue(tmpValue);
       } break;
       case Type_Basler_LineSource: {
           GenApi::CEnumerationPtr  ptrLineSource = cameraNodeMap.GetNode ("LineSource");
           ptrLineSource->SetIntValue(2);
       } break;
       default:
           break;
       }
}

double BaslerCamera::GetCamera(BaslerCamera_Type index)
{
    GenApi::INodeMap &cameraNodeMap = m_basler.GetNodeMap();
    switch (index) {
    case Type_Basler_ExposureTimeAbs: {
        const GenApi::CFloatPtr exposureTime = cameraNodeMap.GetNode("ExposureTimeAbs");
        return exposureTime->GetValue();
    } break;
    case Type_Basler_GainRaw: {
        const GenApi::CIntegerPtr cameraGen = cameraNodeMap.GetNode("GainRaw");
        return cameraGen->GetValue();
    } break;
    case Type_Basler_AcquisitionFrameRateAbs: {
        const GenApi::CBooleanPtr frameRate = cameraNodeMap.GetNode("AcquisitionFrameRateEnable");
        frameRate->SetValue(TRUE);
        const GenApi::CFloatPtr frameRateABS = cameraNodeMap.GetNode("AcquisitionFrameRateAbs");
        return frameRateABS->GetValue();
    } break;
    case Type_Basler_Width: {
        const GenApi::CIntegerPtr widthPic = cameraNodeMap.GetNode("Width");
        return widthPic->GetValue();
    } break;
    case Type_Basler_Height: {
        const GenApi::CIntegerPtr heightPic = cameraNodeMap.GetNode("Height");
        return heightPic->GetValue();
    } break;
    default:
        return -1;
        break;
    }
}

QString BaslerCamera::getFeatureTriggerSourceType()
{
    GenApi::INodeMap &cameraNodeMap = m_basler.GetNodeMap();
    GenApi::CEnumerationPtr  ptrTriggerSel = cameraNodeMap.GetNode ("TriggerSelector");
    ptrTriggerSel->FromString("FrameStart");
    GenApi::CEnumerationPtr  ptrTrigger  = cameraNodeMap.GetNode ("TriggerMode");
    ptrTrigger->SetIntValue(1);
    GenApi::CEnumerationPtr  ptrTriggerSource = cameraNodeMap.GetNode ("TriggerSource");

    Pylon::String_t str = ptrTriggerSource->ToString();
    m_currentMode = QString::fromLocal8Bit(str.c_str());
    return m_currentMode;
}

void BaslerCamera::setFeatureTriggerModeType(bool on)
{
    GenApi::INodeMap &cameraNodeMap = m_basler.GetNodeMap();
    GenApi::CEnumerationPtr  ptrTriggerSel = cameraNodeMap.GetNode ("TriggerSelector");
    ptrTriggerSel->FromString("FrameStart");
    GenApi::CEnumerationPtr  ptrTrigger  = cameraNodeMap.GetNode ("TriggerMode");
    ptrTrigger->SetIntValue(on?1:0);
}

long BaslerCamera::GrabImage(QImage &image, int timeout)
{
    try  {
           if (!m_basler.IsGrabbing()) {
               qDebug()<<"!m_basler.IsGrabbing()";
               StartAcquire();
           }
           Pylon::CGrabResultPtr ptrGrabResult;
           if(m_currentMode == "Freerun")  {
           } else if(m_currentMode == "Software") {
               if (m_basler.WaitForFrameTriggerReady(1000, Pylon::TimeoutHandling_Return)) {
                   m_basler.ExecuteSoftwareTrigger();
                   m_basler.RetrieveResult(timeout, ptrGrabResult,Pylon::TimeoutHandling_Return);
               }
           } else if(m_currentMode == "Line1") {
               m_basler.RetrieveResult(timeout, ptrGrabResult, Pylon::TimeoutHandling_Return);
               qDebug()<<ptrGrabResult;
           } else if(m_currentMode == "Line2") {
               m_basler.RetrieveResult(timeout, ptrGrabResult, Pylon::TimeoutHandling_Return);
           }
           if(ptrGrabResult == NULL) {
               return -5;
           }
           if (ptrGrabResult->GrabSucceeded()) {
               qDebug() << "what: ptrGrabResult GrabSucceeded";
               if (!ptrGrabResult.IsValid()) { OutputDebugString(L"GrabResult not Valid Error\n"); return -1; }
               Pylon::EPixelType pixelType = ptrGrabResult->GetPixelType();
               switch (pixelType) {
               case Pylon::PixelType_Mono8: {
                   CopyToImage(ptrGrabResult, image);
               } break;
               case Pylon::PixelType_BayerRG8: { qDebug() << "what: PixelType_BayerRG8"; }  break;
               default:  qDebug() << "what: default"; break;
               }
           } else {
               qDebug() << "Grab Error!!!";
   //            OutputDebugString(L"Grab Error!!!");
               return -3;
           }
       } catch (GenICam::GenericException &e) {
   //        OutputDebugString(L"GrabImage Error\n");
           qDebug() << "GrabImage Error:" << QString(e.GetDescription());
   //        OutputDebugString((e.GetDescription()));
           return -2;
       }  catch(...)  {
           OutputDebugString(L"ZP 11 Shot GetParam Try 12 No know Error\n");
           return -1;
       }
       return 0;
}

long BaslerCamera::StartAcquire()
{
    m_isOpenAcquire = true;
    try {
        qDebug() << "SBaslerCameraControl StartAcquire" << m_currentMode;
         if(m_currentMode == "Freerun")  {
             m_basler.StartGrabbing(Pylon::GrabStrategy_LatestImageOnly,Pylon::GrabLoop_ProvidedByInstantCamera);
         } else if(m_currentMode == "Software") {
             m_basler.StartGrabbing(Pylon::GrabStrategy_LatestImageOnly);
             onTimerGrabImage();
         } else if(m_currentMode == "Line1") {
             m_basler.StartGrabbing(Pylon::GrabStrategy_OneByOne);
             onTimerGrabImage();
         } else if(m_currentMode == "Line2") {
             m_basler.StartGrabbing(Pylon::GrabStrategy_OneByOne);
         }
    } catch (GenICam::GenericException &e) {
//        OutputDebugString(L"StartAcquire error:");
        qDebug() << "StartAcquire Error:" << QString(e.GetDescription());
        return -2;
    }
    return 0;
}

long BaslerCamera::StopAcquire()
{
    m_isOpenAcquire = false;
    qDebug() << "SBaslerCameraControl StopAcquire";
    try {
        if (m_basler.IsGrabbing()) {
            m_basler.StopGrabbing();
        }
    } catch (GenICam::GenericException &e) {
//        OutputDebugString(LPCWSTR(e.GetDescription()));
        qDebug() << "StopAcquire Error:" << QString(e.GetDescription());
        return -2;
    }
    return 0;
}

void BaslerCamera::UpdateCameraList()
{
    Pylon::CTlFactory& TLFactory = Pylon::CTlFactory::GetInstance();
    Pylon::DeviceInfoList_t devices;
    int n = TLFactory.EnumerateDevices(devices);
    Pylon::CInstantCameraArray cameraArray(devices.size());
    if(n == 0) {
        qDebug() << "Cannot find Any camera!";
        return;
    }
    for (int i=0 ; i<cameraArray.GetSize() ; i++) {
        cameraArray[i].Attach(TLFactory.CreateDevice(devices[i]));
        std::string sn = cameraArray[i].GetDeviceInfo().GetSerialNumber().c_str();
        m_cameralist << QString::fromStdString(sn);
        qDebug()<<m_cameralist;
    }
    emit sigCameraUpdate(m_cameralist);
}

void BaslerCamera::CopyToImage(Pylon::CGrabResultPtr pInBuffer, QImage &OutImage)
{
    try {
            uchar* buff = (uchar*)pInBuffer->GetBuffer();
            int nHeight = pInBuffer->GetHeight();
            int nWidth = pInBuffer->GetWidth();
            if(m_size != QSize(nWidth, nHeight)) {
                m_size = QSize(nWidth, nHeight);
                emit sigSizeChange(m_size);
            }
            QImage imgBuff(buff, nWidth, nHeight, QImage::Format_Indexed8);
            OutImage = imgBuff;
            if(pInBuffer->GetPixelType() == Pylon::PixelType_Mono8) {
                uchar* pCursor = OutImage.bits();
                if ( OutImage.bytesPerLine() != nWidth ) {
                    for ( int y=0; y<nHeight; ++y ) {
                        pCursor = OutImage.scanLine( y );
                        for ( int x=0; x<nWidth; ++x ) {
                            *pCursor =* buff;
                            ++pCursor;
                            ++buff;
                        }
                    }
                } else {
                    memcpy( OutImage.bits(), buff, nWidth * nHeight );
                }
            }
        } catch (GenICam::GenericException &e) {
    //        OutputDebugString(L"CopyToImage\n");
            qDebug() << "CopyToImage Error:" << QString(e.GetDescription());
    //        OutputDebugString(LPCWSTR(e.GetDescription()));
        }
}

void BaslerCamera::onTimerGrabImage()
{
    if(m_isOpenAcquire) {
        QImage image;
        GrabImage(image, 5);
        if(!image.isNull()) {
            qDebug()<<"image not null";
            emit sigCurrentImage(image);
        }
        qDebug()<<"image null";
        QTimer::singleShot(5, this, SLOT(onTimerGrabImage()));
    }
}
