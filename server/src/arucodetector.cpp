#include "arucodetector.h"
#include "mavlinkinterface.h"

ArucoDetector::ArucoDetector(QObject *parent) : QObject(parent),
    m_fps(30),
    m_timer(new QTimer(this)),
    m_videoCapture(new cv::VideoCapture(0))
{
    m_videoCapture->set(cv::CAP_PROP_FRAME_WIDTH, 1280);  // Ширина кадра
    m_videoCapture->set(cv::CAP_PROP_FRAME_HEIGHT, 720);  // Высота кадра
    m_videoCapture->set(cv::CAP_PROP_FPS, m_fps);         // Кадров в секунду

    m_timer->setInterval(1000.f / float(m_fps));
    connect(m_timer, &QTimer::timeout, this, [this] {
        std::vector<int> arucoIds;
        std::vector<std::vector<cv::Point2f>> markerCorners, rejectedCandidates;
        cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
        cv::aruco::DetectorParameters detectorParams = cv::aruco::DetectorParameters();
        cv::aruco::ArucoDetector detector(dictionary, detectorParams);
        cv::Mat frame;
        *m_videoCapture >> frame;
        detector.detectMarkers(frame, markerCorners, arucoIds, rejectedCandidates);
        //cv::aruco::drawDetectedMarkers(frame, markerCorners, markerIds);
        m_arucoIds.clear();
        for (int marker_id : arucoIds) {
            m_arucoIds << marker_id;
            //if (marker_id == 4 && !mavlinkInterface->armed()) {
            //    mavlinkInterface->arm();
            //}
        }
    });
    m_timer->start();
}

ArucoDetector::~ArucoDetector()
{
    m_timer->stop();
    m_videoCapture->release();
}
