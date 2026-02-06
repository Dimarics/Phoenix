#ifndef ARUCODETECTOR_H
#define ARUCODETECTOR_H

#include <opencv2/core/mat.hpp>
#include <QObject>

class ArucoDetector : public QObject
{
    Q_OBJECT
public:
    ArucoDetector(QObject *parent = nullptr);
    inline float x() { return m_x; }
    inline float y() { return m_y; }
    inline float z() { return m_z; }
    inline float yaw() { return m_yaw; }

    void holdOnMarker(bool hold);
    void detectMarkers(const cv::Mat &frame);
    void drawDetectMarkers(cv::Mat &image);
    inline QList<int> arucoIds() const { return m_qListArucoIds; }

private:
    bool m_holdOnMarker = true;
    float m_x;
    float m_y;
    float m_z;
    float m_yaw;
    std::vector<int> m_arucoIds;
    std::vector<std::vector<cv::Point2f>> m_markerCorners;
    QList<int> m_qListArucoIds;

    void setPos(std::vector<cv::Point2f> markerCorners, qreal offset_x, qreal offset_y);
};

#endif // ARUCODETECTOR_H
