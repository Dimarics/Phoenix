#include "server.h"
#include <QFile>
#include <QFileInfo>
#include <QSslServer>
#include <QSslCertificate>
#include <QSslKey>

const QHash<QString, QString> Server::mimeTypes = {
    { "html", "text/html" },
    { "htm", "text/html" },
    { "js", "application/javascript" },
    { "css", "text/css" },
    { "wasm", "application/wasm" },
    { "json", "application/json" },
    { "png", "image/png" },
    { "jpg", "image/jpeg" },
    { "jpeg", "image/jpeg" },
    { "gif", "image/gif" },
    { "svg", "image/svg+xml" },
    { "ico", "image/x-icon" },
    { "txt", "text/plain" }
};

Server::Server(QObject *parent) : QObject(parent),
    m_httpServer(new QHttpServer(this)),
    m_webSocketServer(new QWebSocketServer("Phoenix", QWebSocketServer::NonSecureMode, this))
{
    QTcpServer *tcp = new QTcpServer(this);
    /*
    QSslServer *tcp = new QSslServer(this);
    QFile certFile(":/ssl/certificate.crt");
    certFile.open   (QIODevice::ReadOnly | QIODevice::Text);
    QSslCertificate sslCert(&certFile, QSsl::Pem);
    certFile.close();

    QFile keyFile(":/ssl/private.key");
    keyFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QSslKey sslKey(&keyFile, QSsl::Rsa, QSsl::Pem);
    keyFile.close();

    QSslConfiguration sslConfig = QSslConfiguration::defaultConfiguration();
    sslConfig.setCaCertificates(QSslCertificate::fromPath(":/ssl/certificate.crt"));
    sslConfig.setLocalCertificate(sslCert);
    sslConfig.setPrivateKey(sslKey);
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyPeer);
    tcp->setSslConfiguration(sslConfig);
*/

    tcp->listen(QHostAddress::Any, 8080);
    m_httpServer->bind(tcp);

    m_httpServer->route("/", QHttpServerRequest::Method::Get, [] {
        //QFile file(":/frontend/PhoenixFrontend.html");
#ifdef __arm__
        QFile file("frontend/PhoenixFrontend.html");
#else
        QFile file("D:/Development/Phoenix/build/frontend/WebAssembly/debug/PhoenixFrontend.html");
#endif
        if (file.open(QIODevice::ReadOnly)) {
            return QHttpServerResponse("text/html", file.readAll());
        }
        return QHttpServerResponse(QHttpServerResponder::StatusCode::NotFound);
    });
    // Обработчик для других статических файлов
    m_httpServer->route("/<arg>", [](QString fileName) {
        qDebug() << "arg:" << fileName;
        if (fileName.startsWith("/")) fileName = fileName.mid(1);
#ifdef __arm__
        QFile file("frontend/" + fileName);
#else
        QFile file("D:/Development/Phoenix/build/frontend/WebAssembly/debug/" + fileName);
#endif
        if (!file.open(QIODevice::ReadOnly)) {
            return QHttpServerResponse("File not found", QHttpServerResponse::StatusCode::NotFound);
        }
        return QHttpServerResponse(mimeTypes.value(QFileInfo(fileName).suffix(), "text/plain").toUtf8(), file.readAll());
    });
    m_httpServer->route("/client/debug", QHttpServerRequest::Method::Post, [](const QHttpServerRequest &request) {
        qDebug() << "client debug:" << request.body();
        return QHttpServerResponse(QHttpServerResponder::StatusCode::Accepted);
    });

    m_webSocketServer->listen(QHostAddress::Any, 8081);
    connect(m_webSocketServer, &QWebSocketServer::newConnection, this, [this] {
        if (!m_webSocketServer->hasPendingConnections()) return;
        QWebSocket *socket = m_webSocketServer->nextPendingConnection();
        socket->setParent(this);
        qDebug() << "new connection:" << socket;
        /*
        if (socket->requestUrl().path() == "/api") {
            socket->setParent(this);
            m_clients << socket;
            connect(socket, &QWebSocket::binaryMessageReceived, this, &Server::messageReceived);
            connect(socket, &QWebSocket::disconnected, this, [socket, this] {
                m_clients.removeOne(socket);
                m_clients.squeeze();
                qDebug() << socket << "disconnected";
            });
        } else if (socket->requestUrl().path() == "/followStream") { // setup video stream
            m_streamFollowers << socket;
            connect(socket, &QWebSocket::disconnected, this, [socket, this] {
                m_streamFollowers.removeOne(socket);
                m_streamFollowers.squeeze();
                qDebug() << socket << "disconnected";
            });
        }
        */
        if (socket->requestUrl().path() == "/api") {
            connect(socket, &QWebSocket::binaryMessageReceived, this, &Server::messageReceived);
            connect(this, &Server::sendDataToClients, socket, &QWebSocket::sendBinaryMessage);
        } else if (socket->requestUrl().path() == "/followStream") {
            connect(this, &Server::sendVideoFrame, socket, &QWebSocket::sendBinaryMessage);
        } else if (socket->requestUrl().path() == "/script") {
            connect(socket, &QWebSocket::binaryMessageReceived, this, &Server::scriptMessageReceived);
            connect(this, &Server::sendScriptMessage, socket, &QWebSocket::sendBinaryMessage);
        }
        connect(socket, &QWebSocket::disconnected, this, [socket] {
            qDebug() << socket << "disconnected";
            socket->deleteLater();
        });
    });
}

Server::~Server() {}

/*
void Server::sendDataToClients(const QByteArray &data)
{
    for (QWebSocket *&socket : m_clients) {
        socket->sendBinaryMessage(data);
    }
}

void Server::sendVideoFrame(const QByteArray &frame)
{
    for (QWebSocket *&socket : m_streamFollowers) {
        socket->sendBinaryMessage(frame);
    }
}
*/
