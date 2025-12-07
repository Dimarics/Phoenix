#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QObject>
#include <QtHttpServer/QHttpServer>
#include "mavlinkinterface.h"

class HttpServer : public QObject
{
    Q_OBJECT
public:
    HttpServer();

private:
    static const QHash<QString, QString> mimeTypes;
    QHttpServer *m_server;
    QList<QWebSocket*> m_clients;
    MAVLinkInterface *m_mavlinkInterface;
};

#endif // HTTPSERVER_H
