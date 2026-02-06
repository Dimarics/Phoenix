#include "streamviewer.h"

#ifdef __EMSCRIPTEN__
#include "appbackend.h"

StreamViewer::StreamViewer(QQuickItem *parent) : ImageViewer(parent),
    m_webSocket(new QWebSocket(QString(), QWebSocketProtocol::VersionLatest, this))
{
    m_webSocket->open(QUrl(QString("ws://%1:%2/followStream").arg(AppBackend::instance()->webServerAddress()).arg(8081)));
    connect(m_webSocket, &QWebSocket::binaryMessageReceived, this, [this](const QByteArray &frame) {
        //qDebug() << frame;
        setImageFromData(frame);
    });
    connect(m_webSocket, &QWebSocket::errorOccurred, this, [this] {
        QTimer::singleShot(1000, this, [this] {
            if (m_webSocket->state() == QAbstractSocket::UnconnectedState) {
                m_webSocket->open(m_webSocket->requestUrl());
            }
        });
    });
}
#else
StreamViewer::StreamViewer(QQuickItem *parent) : ImageViewer(parent){}
#endif
