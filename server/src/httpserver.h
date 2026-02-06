#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include "arucodetector.h"
#include "sonar.h"
#include <QtHttpServer/QHttpServer>
#include <QWebSocketServer>

class HttpServer : public QObject
{
    Q_OBJECT
public:
    enum MessageID: quint8 {
        MSG_SET_MODE = 1,
        MSG_ARM_DISARM,
        MSG_TAKEOFF,
        MSG_LAND,
        MSG_DROP,
        MSG_SET_THROTTLE_PWM,
        MSG_SET_ROLL_PWM,
        MSG_SET_PITCH_PWM,
        MSG_SET_YAW_PWM,
        MSG_ALIGN,
        MSG_SET_X,
        MSG_SET_Y,
        MSG_SET_Z,
        MSG_SET_YAW,
    };
    HttpServer();
    ~HttpServer();

private:
    static const QHash<QString, QString> mimeTypes;
    QHttpServer *m_server;
    QWebSocketServer *m_webSocketServer;
    ArucoDetector *m_arucoDetector;
    Sonar *m_sonar;
    QList<QWebSocket*> m_clients;
    QList<QWebSocket*> m_streamFollowers;

    bool m_buttonState = false;
};

#endif // HTTPSERVER_H
