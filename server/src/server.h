#ifndef SERVER_H
#define SERVER_H

#include <QtHttpServer/QHttpServer>
#include <QWebSocketServer>

class Server : public QObject
{
    Q_OBJECT
public:
    Server(QObject *parent = nullptr);
    ~Server();
    //void sendDataToClients(const QByteArray &data);
    //void sendVideoFrame(const QByteArray &frame);

private:
    static const QHash<QString, QString> mimeTypes;
    QHttpServer *m_httpServer;
    QWebSocketServer *m_webSocketServer;
    QList<QWebSocket*> m_clients;
    QList<QWebSocket*> m_streamFollowers;

signals:
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

    void messageReceived(const QByteArray &msg);
    void sendDataToClients(const QByteArray &data);

    void sendVideoFrame(const QByteArray &frame);

    void scriptMessageReceived(const QByteArray &msg);
    void sendScriptMessage(const QByteArray &msg);
};

#endif // SERVER_H
