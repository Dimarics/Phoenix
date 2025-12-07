#include "httpserver.h"
#include <QFile>
#include <QFileInfo>
#include <QTcpServer>

const QHash<QString, QString> HttpServer::mimeTypes = {
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

HttpServer::HttpServer() : m_server(new QHttpServer(this)), m_mavlinkInterface(new MAVLinkInterface(this))
{
    QTcpServer *tcp = new QTcpServer(this);
    tcp->listen(QHostAddress::Any, 80);
    m_server->bind(tcp);
    m_server->route("/", QHttpServerRequest::Method::Get, []() {
        //QFile file(":/frontend/PhoenixFrontend.html");
        QFile file("D:/Development/Phoenix/build/frontend/WebAssembly/debug/PhoenixFrontend.html");
        if (file.open(QIODevice::ReadOnly)) {
            return QHttpServerResponse("text/html", file.readAll());
        }
        return QHttpServerResponse(QHttpServerResponder::StatusCode::NotFound);
    });
    m_server->route("/device", QHttpServerRequest::Method::Post, [this](const QHttpServerRequest &request) {
        m_mavlinkInterface->sendMavlinkMessage(request.body());
        return QHttpServerResponse(QHttpServerResponder::StatusCode::Accepted);
    });
    // Обработчик для других статических файлов
    m_server->route("/<arg>", [](QString fileName) {
        //qDebug() << "arg:" << fileName;
        if (fileName.startsWith("/")) fileName = fileName.mid(1);
        //QFile file(":/frontend/" + fileName);
        QFile file("D:/Development/Phoenix/build/frontend/WebAssembly/debug/" + fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            return QHttpServerResponse("File not found", QHttpServerResponse::StatusCode::NotFound);
        }
        return QHttpServerResponse(mimeTypes.value(QFileInfo(fileName).suffix(), "text/plain").toUtf8(), file.readAll());
    });
}
