#ifndef ARUCODETECTOR_H
#define ARUCODETECTOR_H

#include <opencv2/opencv.hpp>
#include <QTimer>

class ArucoDetector : QObject
{
public:
    ArucoDetector(QObject *parent = nullptr);
    ~ArucoDetector();

private:
    int m_fps;
    QTimer *m_timer;
    cv::VideoCapture *m_videoCapture;
};

#endif // ARUCODETECTOR_H
