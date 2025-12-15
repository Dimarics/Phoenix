#include "httpserver.h"
#include "mavlinkinterface.h"
#include <QFile>
#include <QFileInfo>
#include <QSslServer>
#include <QSslCertificate>
#include <QSslKey>

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

HttpServer::HttpServer() : m_server(new QHttpServer(this)), m_arucoDetector(new ArucoDetector(this))
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
    m_server->bind(tcp);

    QTimer *pollTimer = new QTimer(this);
    connect(pollTimer, &QTimer::timeout, this, [this] {
        m_sonar->tick();
    });
    pollTimer->start(20);

    m_server->route("/", QHttpServerRequest::Method::Get, []() {
        //QFile file(":/frontend/PhoenixFrontend.html");
#ifdef QT_OS_LINUX
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
    m_server->route("/<arg>", [](QString fileName) {
        //qDebug() << "arg:" << fileName;
        if (fileName.startsWith("/")) fileName = fileName.mid(1);
#ifdef QT_OS_LINUX
        QFile file("frontend/" + fileName);
#else
        QFile file("D:/Development/Phoenix/build/frontend/WebAssembly/debug/" + fileName);
#endif
        if (!file.open(QIODevice::ReadOnly)) {
            return QHttpServerResponse("File not found", QHttpServerResponse::StatusCode::NotFound);
        }
        return QHttpServerResponse(mimeTypes.value(QFileInfo(fileName).suffix(), "text/plain").toUtf8(), file.readAll());
    });
    m_server->route("/client/debug", QHttpServerRequest::Method::Post, [](const QHttpServerRequest &request) {
        qDebug() << "client debug:" << request.body();
        return QHttpServerResponse(QHttpServerResponder::StatusCode::Accepted);
    });

    m_server->route("/device/arm", QHttpServerRequest::Method::Post, [] {
        //qDebug() << "arm";
        mavlinkInterface->arm();
        return QHttpServerResponse(QHttpServerResponder::StatusCode::Accepted);
    });
    m_server->route("/device/setThrottle", QHttpServerRequest::Method::Post, [](const QHttpServerRequest &request) {
        mavlinkInterface->setThrottle(request.body().toFloat());
        return QHttpServerResponse(QHttpServerResponder::StatusCode::Accepted);
    });
    m_server->route("/device/setRoll", QHttpServerRequest::Method::Post, [](const QHttpServerRequest &request) {
        mavlinkInterface->setRoll(request.body().toFloat());
        return QHttpServerResponse(QHttpServerResponder::StatusCode::Accepted);
    });
    m_server->route("/device/setPitch", QHttpServerRequest::Method::Post, [](const QHttpServerRequest &request) {
        mavlinkInterface->setPitch(request.body().toFloat());
        return QHttpServerResponse(QHttpServerResponder::StatusCode::Accepted);
    });
    m_server->route("/device/setYaw", QHttpServerRequest::Method::Post, [](const QHttpServerRequest &request) {
        mavlinkInterface->setYaw(request.body().toFloat());
        return QHttpServerResponse(QHttpServerResponder::StatusCode::Accepted);
    });
    m_server->route("/device/align", QHttpServerRequest::Method::Post, [] {
        mavlinkInterface->align();
        return QHttpServerResponse(QHttpServerResponder::StatusCode::Accepted);
    });

    m_server->route("/device/getSonarDistance", QHttpServerRequest::Method::Get, [this] {
        return QHttpServerResponse(QByteArray::number(m_sonar->distance()));
    });
    m_server->route("/device/getArucoIds", QHttpServerRequest::Method::Get, [this] {
        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);
        stream << m_arucoDetector->arucoIds();
        return QHttpServerResponse(data);
    });
    m_server->route("/device/data", QHttpServerRequest::Method::Get, [this] {
        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);
        stream << m_sonar->distance();
        stream << m_arucoDetector->arucoIds();
        return QHttpServerResponse(data);
    });
}

HttpServer::~HttpServer()
{
    delete m_sonar;
}
