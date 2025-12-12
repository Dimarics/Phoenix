#include "arucodetector.h"
#include "httpserver.h"
#include "mavlinkinterface.h"
#include <QCoreApplication>

//#include <opencv2/objdetect/aruco_detector.hpp>

int main(int argc, char *argv[])
{
    /*
    // Генерация маркеров
    cv::Mat markerImage;
    cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
    for (int i = 1; i <= 10; ++i) {
        cv::aruco::generateImageMarker(dictionary, i, 240, markerImage, 1);
        cv::imwrite(std::string("D:/Development/Phoenix/markers/marker_") + std::to_string(i) + ".png", markerImage);
    }
    */

    QCoreApplication app(argc, argv);
    //DomainManager domainManager("hobots-control");
    mavlinkInterface = new MAVLinkInterface(&app);
    new ArucoDetector(&app);
    HttpServer server;

    return app.exec();

}
