#ifndef ARUCODETECTOR_H
#define ARUCODETECTOR_H

#include <opencv2/opencv.hpp>
#include <QTimer>

class ArucoDetector : QObject
{
public:
    ArucoDetector(QObject *parent = nullptr);
    ~ArucoDetector();
    inline QList<int> arucoIds() const { return m_arucoIds; }

private:
    int m_fps;
    QTimer *m_timer;
    cv::VideoCapture *m_videoCapture;
    QList<int> m_arucoIds;
};

#endif // ARUCODETECTOR_H
