#ifndef SERVER_H
#define SERVER_H

#include "arucodetector.h"
#include "sonar.h"
#include <QtHttpServer/QHttpServer>
#include <QWebSocketServer>

class Server : public QObject
{
    Q_OBJECT
public:
    Server(QObject *parent = nullptr);
    void sendDataToClients(const QByteArray &data);
    void sendVideoFrame(const QByteArray &frame);

private:
    static const QHash<QString, QString> mimeTypes;
    QHttpServer *m_httpServer;
    QWebSocketServer *m_webSocketServer;
    QList<QWebSocket*> m_clients;
    QList<QWebSocket*> m_streamFollowers;

signals:
    void messageReceived(const QByteArray &msg);
    void setMode(uint8_t mode);
    void armDisarm(bool arm);
    void takeoff(float z);
    void land();
    void drop();
    void setThrottlePWM(float value);
    void setRollPWM(float value);
    void setPitchPWM(float value);
    void setYawPWM(float value);
    void align();
    void setTargetX(float value);
    void setTargetY(float value);
    void setTargetZ(float value);
    void setTargetYaw(float value);
};

#endif // SERVER_H
