#ifndef VIDEOCAPTURE_H
#define VIDEOCAPTURE_H

#include "opencv2/videoio.hpp"
#include <QObject>
#include <QTimer>
#include <thread>

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
    std::thread m_cameraThread;
    QTimer *m_timer;
    cv::VideoCapture *m_videoCapture;
    cv::Mat m_frame;
    void cameraLoop();

signals:
    void frameChanged(cv::Mat frame, uint64_t timestamp);
};

#endif // VIDEOCAPTURE_H
