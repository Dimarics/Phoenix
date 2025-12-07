#include "httpserver.h"
#include <QCoreApplication>
#include <common/mavlink.h>

#include <opencv2/opencv.hpp>
#include <iostream>
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
    /*
    cv::VideoCapture cap(0);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 1920);   // Ширина кадра
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 1280);  // Высота кадра
    cap.set(cv::CAP_PROP_FPS, 30);             // Кадров в секунду
    //std::cout << cap.get(cv::CAP_PROP_FRAME_WIDTH) << " " << cap.get(cv::CAP_PROP_FRAME_HEIGHT) << std::endl;
    cv::Mat frame, markerImage;

    while (true) {
        // Захватываем кадр
        cap >> frame;

        // Если кадр пустой, выходим
        if (frame.empty()) {
            //cout << "Ошибка: пустой кадр" << endl;
            break;
        }

        // Отображаем кадр
        std::vector<int> markerIds;
        std::vector<std::vector<cv::Point2f>> markerCorners, rejectedCandidates;
        cv::aruco::DetectorParameters detectorParams = cv::aruco::DetectorParameters();
        cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
        cv::aruco::ArucoDetector detector(dictionary, detectorParams);
        detector.detectMarkers(frame, markerCorners, markerIds, rejectedCandidates);
        markerImage = frame;
        cv::aruco::drawDetectedMarkers(markerImage, markerCorners, markerIds);

        imshow("Camera", markerImage);

        // Выход по нажатию ESC
        if (cv::waitKey(1) == 27) {
            break;
        }
    }

    // Освобождаем ресурсы
    cap.release();
    cv::destroyAllWindows();

    return 0;
    */
    QCoreApplication app(argc, argv);
    //DomainManager domainManager("hobots-control");

    HttpServer server;

    return app.exec();
}
