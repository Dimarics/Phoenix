#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include "arucodetector.h"
#include "sonar.h"
#include <QtHttpServer/QHttpServer>

class HttpServer : public QObject
{
    Q_OBJECT
public:
    HttpServer();
    ~HttpServer();

private:
    static const QHash<QString, QString> mimeTypes;
    QHttpServer *m_server;
    ArucoDetector *m_arucoDetector;
    Sonar *m_sonar;
    QList<QTcpSocket*> m_clients;
};

#endif // HTTPSERVER_H
