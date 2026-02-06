#include "httpserver.h"
#include "mavlinkinterface.h"
#include <QFile>
#include <QFileInfo>
#include <QSslServer>
#include <QSslCertificate>
#include <QSslKey>
#include <QTimer>
#ifdef __arm__
#include <pigpio.h>
#endif

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

HttpServer::HttpServer() :
    m_server(new QHttpServer(this)),
    m_webSocketServer(new QWebSocketServer("Phoenix", QWebSocketServer::NonSecureMode)),
    m_arucoDetector(new ArucoDetector(this)),
    m_sonar(new Sonar(17, 27))
{
#ifdef __arm__
    gpioSetMode(17, PI_INPUT);
#endif

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
#ifdef __arm__
        m_buttonState = gpioRead(17);
        //qDebug() << m_buttonState;
#endif
    });
    pollTimer->start(20);

    m_server->route("/", QHttpServerRequest::Method::Get, [] {
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
    m_server->route("/<arg>", [](QString fileName) {
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
    m_server->route("/client/debug", QHttpServerRequest::Method::Post, [](const QHttpServerRequest &request) {
        qDebug() << "client debug:" << request.body();
        return QHttpServerResponse(QHttpServerResponder::StatusCode::Accepted);
    });

    m_server->route("/device/arm", QHttpServerRequest::Method::Post, [](const QHttpServerRequest &request) {
        //qDebug() << "arm";
        mavlinkInterface->armDisarm(request.body().toInt());
        return QHttpServerResponse(QHttpServerResponder::StatusCode::Accepted);
    });
    m_server->route("/device/setThrottle", QHttpServerRequest::Method::Post, [](const QHttpServerRequest &request) {
        mavlinkInterface->setThrottlePWM(request.body().toFloat());
        //qDebug() << "Throttle:" << mavlinkInterface->throttle() << "Voltage:" << mavlinkInterface->voltageBattery();
        return QHttpServerResponse(QHttpServerResponder::StatusCode::Accepted);
    });
    m_server->route("/device/setRoll", QHttpServerRequest::Method::Post, [](const QHttpServerRequest &request) {
        mavlinkInterface->setRollPWM(request.body().toFloat());
        return QHttpServerResponse(QHttpServerResponder::StatusCode::Accepted);
    });
    m_server->route("/device/setPitch", QHttpServerRequest::Method::Post, [](const QHttpServerRequest &request) {
        mavlinkInterface->setPitchPWM(request.body().toFloat());
        return QHttpServerResponse(QHttpServerResponder::StatusCode::Accepted);
    });
    m_server->route("/device/setYaw", QHttpServerRequest::Method::Post, [](const QHttpServerRequest &request) {
        mavlinkInterface->setYawPWM(request.body().toFloat());
        return QHttpServerResponse(QHttpServerResponder::StatusCode::Accepted);
    });
    m_server->route("/device/align", QHttpServerRequest::Method::Post, [] {
        mavlinkInterface->align();
        return QHttpServerResponse(QHttpServerResponder::StatusCode::Accepted);
    });
    m_server->route("/device/holdOnMarker", QHttpServerRequest::Method::Post, [this](const QHttpServerRequest &request) {
        m_arucoDetector->holdOnMarker(request.body().toInt());
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
        stream << mavlinkInterface->armed();
        stream << m_buttonState;
        stream << m_sonar->distance();
        stream << m_arucoDetector->arucoIds();
        return QHttpServerResponse(data);
    });

    m_webSocketServer->listen(QHostAddress::Any, 8081);
    connect(m_webSocketServer, &QWebSocketServer::newConnection, this, [this] {
        if (!m_webSocketServer->hasPendingConnections()) return;
        QWebSocket *socket = m_webSocketServer->nextPendingConnection();
        qDebug() << "new connection:" << socket;
        if (socket->requestUrl().path() == "/followStream") { // setup video stream
            m_streamFollowers << socket;
            connect(socket, &QWebSocket::disconnected, this, [socket, this] {
                m_streamFollowers.removeOne(socket);
                m_streamFollowers.squeeze();
                qDebug() << socket << "disconnected";
            });
        } else { // setup client
            m_clients << socket;
            connect(socket, &QWebSocket::binaryMessageReceived, this, [](const QByteArray &msg) {
                quint8 msg_id;
                QDataStream msg_data(msg);
                msg_data >> msg_id;
                switch(msg_id) {
                case MSG_SET_MODE: {
                    uint8_t mode;
                    msg_data >> mode;
                    mavlinkInterface->setMode(mode);
                    qDebug() << "setMode:" << mode;
                }
                    break;
                case MSG_TAKEOFF: {
                    float z;
                    msg_data >> z;
                    mavlinkInterface->takeoff(z);
                    qDebug() << "takeoff:" << z;
                }
                    break;
                case MSG_LAND:
                    mavlinkInterface->land();
                    qDebug() << "land";
                    break;
                case MSG_DROP:
                    mavlinkInterface->drop();
                    qDebug() << "drop";
                    break;
                case MSG_ARM_DISARM: {
                    bool arm;
                    msg_data >> arm;
                    mavlinkInterface->armDisarm(arm);
                    qDebug() << "arm:" << arm;
                }
                    break;
                case MSG_SET_THROTTLE_PWM: {
                    float throttle;
                    msg_data >> throttle;
                    mavlinkInterface->setThrottlePWM(throttle);
                    qDebug() << "set throttle pwm:" << throttle;
                }
                    break;
                case MSG_SET_ROLL_PWM: {
                    float roll;
                    msg_data >> roll;
                    mavlinkInterface->setRollPWM(roll);
                    qDebug() << "set roll pwm:" << roll;
                }
                    break;
                case MSG_SET_PITCH_PWM: {
                    float pitch;
                    msg_data >> pitch;
                    mavlinkInterface->setPitchPWM(pitch);
                    qDebug() << "set pitch pwm:" << pitch;
                }
                    break;
                case MSG_ALIGN:
                    mavlinkInterface->align();
                    qDebug() << "align";
                    break;
                case MSG_SET_X: {
                    float x;
                    msg_data >> x;
                    mavlinkInterface->setTargetX(x);
                    qDebug() << "set x:" << x;
                }
                    break;
                case MSG_SET_Y: {
                    float y;
                    msg_data >> y;
                    mavlinkInterface->setTargetY(y);
                    qDebug() << "set y:" << y;
                }
                    break;
                case MSG_SET_Z: {
                    float z;
                    msg_data >> z;
                    mavlinkInterface->setTargetZ(z);
                    qDebug() << "set z:" << z;
                }
                    break;
                case MSG_SET_YAW: {
                    float yaw;
                    msg_data >> yaw;
                    mavlinkInterface->setTargetYaw(yaw);
                    qDebug() << "set yaw:" << yaw;
                }
                    break;
                }
            });
            connect(socket, &QWebSocket::disconnected, this, [socket, this] {
                m_clients.removeOne(socket);
                m_clients.squeeze();
                qDebug() << socket << "disconnected";
            });
        }
    });
    /*
    connect(m_arucoDetector, &ArucoDetector::coordsChanged, this, [](float x, float y, float z, float yaw) {
        mavlinkInterface->setX(x);
        mavlinkInterface->setY(y);
        mavlinkInterface->setZ(z);
        mavlinkInterface->setYaw(yaw);
    });
    connect(m_arucoDetector, &ArucoDetector::frameChanged, this, [this](const QByteArray &frame) {
        for (QWebSocket *&socket : m_streamFollowers) {
            socket->sendBinaryMessage(frame);
        }
    });
    */
    QTimer *pingTimer = new QTimer(this);
    pingTimer->setInterval(200);
    connect(pingTimer, &QTimer::timeout, this, [this] {
        //qDebug() << mavlinkInterface->x() << mavlinkInterface->y() << mavlinkInterface->z()
        //         << mavlinkInterface->roll() << mavlinkInterface->yaw() << mavlinkInterface->pitch();
        for (QWebSocket *&socket : m_clients) {
            QByteArray data;
            QDataStream stream(&data, QIODevice::WriteOnly);
            stream << mavlinkInterface->systemStatus();
            stream << mavlinkInterface->batteryRemaining();
            stream << mavlinkInterface->mode();
            stream << mavlinkInterface->armed();
            stream << mavlinkInterface->x();
            stream << mavlinkInterface->y();
            stream << mavlinkInterface->z();
            stream << mavlinkInterface->roll();
            stream << mavlinkInterface->pitch();
            stream << mavlinkInterface->yaw();
            stream << m_arucoDetector->arucoIds();
            stream << m_buttonState;
            stream << m_sonar->distance();
            socket->sendBinaryMessage(data);
        }
    });
    pingTimer->start();
}

HttpServer::~HttpServer()
{
    delete m_sonar;
}
