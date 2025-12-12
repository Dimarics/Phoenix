#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QtHttpServer/QHttpServer>

class HttpServer : public QObject
{
    Q_OBJECT
public:
    HttpServer();

private:
    static const QHash<QString, QString> mimeTypes;
    QHttpServer *m_server;
    QList<QWebSocket*> m_clients;
};

#endif // HTTPSERVER_H
