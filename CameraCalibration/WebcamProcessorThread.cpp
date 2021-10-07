//#include "WebcamProcessorThread.h"

//WebcamProcessorThread::WebcamProcessorThread()
//{
//    web_cam.open(1);
//}

//void WebcamProcessorThread::run()
//{
//    cv::Mat outFrame;

//    while(web_cam.isOpened() && !isInterruptionRequested())
//        {
//        web_cam>>outFrame;
//            emit outDisplay(
//                        QPixmap::fromImage(
//                            QImage(
//                                outFrame.data,
//                                outFrame.cols,
//                                outFrame.rows,
//                                outFrame.step,
//                                QImage::Format_RGB888).rgbSwapped()));
//        }
//}
