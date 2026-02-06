#include "videocapture.h"
#include "opencv2/imgcodecs.hpp"

VideoCapture::VideoCapture(QObject *parent) : QObject(parent),
    m_fps(30),
    m_emptyFrameCount(0),
    m_timer(new QTimer(this)),
    m_videoCapture(new cv::VideoCapture())
{
    tryConnect();
    connect(m_timer, &QTimer::timeout, this, [this] {
        if (!m_videoCapture->isOpened()) {
            if (!tryConnect()) return;
        }
        cv::Mat frame;
        // проверка соединения с камерой
        if (!m_videoCapture->read(frame)) {
            if (++m_emptyFrameCount >= 50) {
                m_videoCapture->release();
                tryConnect();
            }
            return;
        }
        m_emptyFrameCount = 0;
        emit frameChanged(frame);
    });
    m_timer->start();
}

VideoCapture::~VideoCapture()
{
    m_timer->stop();
    m_videoCapture->release();
}

bool VideoCapture::tryConnect() {
    //if (m_videoCapture->isOpened())
    if (!m_videoCapture->open(0)) {
        m_timer->setInterval(1000);
        return false;
    }
    m_videoCapture->set(cv::CAP_PROP_FRAME_WIDTH, 640);   // Ширина кадра
    m_videoCapture->set(cv::CAP_PROP_FRAME_HEIGHT, 480);  // Высота кадра
    m_videoCapture->set(cv::CAP_PROP_FPS, m_fps);         // Кадров в секунду
    m_timer->setInterval(1000.f / float(m_fps));
    return true;
}

QByteArray VideoCapture::matToJpeg(const cv::Mat &mat)
{
    std::vector<uchar> buffer;
    cv::imencode(".jpg", mat, buffer, {cv::IMWRITE_JPEG_QUALITY, 80});
    return QByteArray(reinterpret_cast<char*>(buffer.data()), buffer.size());
}
