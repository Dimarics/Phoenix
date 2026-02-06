//#include "arucodetector.h"
//#include "httpserver.h"
//#include "mavlinkinterface.h"
#include "vehiclecontroller.h"
#include <QApplication>
#ifdef __arm__
#include <pigpio.h>
#endif
#include <opencv2/opencv.hpp>

//#include <opencv2/objdetect/aruco_detector.hpp>

int main(int argc, char *argv[])
{
/*
    // Генерация маркеров
    cv::Mat markerImage;
    cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_250);
    for (int i = 0; i <= 1; ++i) {
        cv::aruco::generateImageMarker(dictionary, i, 240, markerImage, 1);
        cv::imwrite(std::string("D:/Development/Phoenix/markers/marker_") + std::to_string(i) + ".png", markerImage);
    }
    return 0;
*/

#ifdef __arm__
    qDebug() << gpioInitialise();
#endif
    QCoreApplication app(argc, argv);
    //DomainManager domainManager("hobots-control");
    //mavlinkInterface = new MAVLinkInterface(&app);
    //HttpServer server;
    VehicleController controller;
    int exit_code = app.exec();
#ifdef __arm__
    gpioTerminate();
#endif
    return exit_code;
}
