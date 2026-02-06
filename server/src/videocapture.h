#ifndef VIDEOCAPTURE_H
#define VIDEOCAPTURE_H

#include "opencv2/videoio.hpp"
#include <QObject>
#include <QTimer>

class VideoCapture : public QObject
{
    Q_OBJECT
public:
    VideoCapture(QObject *parent = nullptr);
    ~VideoCapture();
    bool tryConnect();
    QByteArray matToJpeg(const cv::Mat &mat);

private:
    float m_fps;
    int m_emptyFrameCount;
    QTimer *m_timer;
    cv::VideoCapture *m_videoCapture;

signals:
    void frameChanged(const cv::Mat &frame);
};

#endif // VIDEOCAPTURE_H
