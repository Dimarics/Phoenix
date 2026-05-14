#include "videocapture.h"
#include "opencv2/imgcodecs.hpp"

#include "qdebug.h"
#include "qelapsedtimer.h"

VideoCapture::VideoCapture(QObject *parent) : QObject(parent),
    m_fps(30),
    m_emptyFrameCount(0),
    //m_cameraThread(&VideoCapture::cameraLoop, this),
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
        //qDebug() << "====================";
        QElapsedTimer frame_timer;
        frame_timer.start();
        if (!m_videoCapture->read(frame)) {
            if (++m_emptyFrameCount >= 50) {
                m_videoCapture->release();
                tryConnect();
            }
            return;
        }
        //qDebug() << "время получения кадра" << frame_timer.elapsed() << "мс";
        m_emptyFrameCount = 0;
        emit frameChanged(frame, std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
    });
    m_timer->start();
}

VideoCapture::~VideoCapture()
{
    //qDebug() << m_videoCapture;
    m_timer->stop();
    m_videoCapture->release();
    delete m_videoCapture;
    m_cameraThread.join();
}

bool VideoCapture::tryConnect() {
    //if (m_videoCapture->isOpened())
    if (!m_videoCapture->open(0)) {
        m_timer->setInterval(1000);
        return false;
    }
    m_videoCapture->set(cv::CAP_PROP_FRAME_WIDTH, 640);   // Ширина кадра
    m_videoCapture->set(cv::CAP_PROP_FRAME_HEIGHT, 480);  // Высота кадра
    m_videoCapture->set(cv::CAP_PROP_FPS, 30);            // Кадров в секунду
    //m_videoCapture->set(cv::CAP_PROP_BUFFERSIZE, 1);      // Размер буффера
    //m_timer->setInterval(1000.f / float(m_fps));
    //qDebug() << m_videoCapture->get(cv::CAP_PROP_FPS);
    m_timer->setInterval(5);
    return true;
}

QByteArray VideoCapture::matToJpeg(const cv::Mat &mat)
{
    std::vector<uchar> buffer;
    cv::imencode(".jpg", mat, buffer, {cv::IMWRITE_JPEG_QUALITY, 80});
    return QByteArray(reinterpret_cast<char*>(buffer.data()), buffer.size());
}

void VideoCapture::cameraLoop()
{
    cv::VideoCapture videoCapture(0);
    videoCapture.set(cv::CAP_PROP_FRAME_WIDTH, 640);   // Ширина кадра
    videoCapture.set(cv::CAP_PROP_FRAME_HEIGHT, 480);  // Высота кадра
    videoCapture.set(cv::CAP_PROP_FPS, 30);            // Кадров в секунду
    //m_videoCapture->set(cv::CAP_PROP_BUFFERSIZE, 1);      // Размер буффера
    //qDebug() << "buffer size:" << videoCapture.get(cv::CAP_PROP_BUFFERSIZE);
    QElapsedTimer tm;
    tm.start();
    while (true) {
        if (!videoCapture.isOpened()) break;
        if (videoCapture.grab()) {
            cv::Mat frame;
            std::mutex mutex;
            videoCapture.retrieve(frame);
            //qDebug() << tm.elapsed();
            tm.restart();
            emit frameChanged(frame, std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
        }
    }
    videoCapture.release();
}
