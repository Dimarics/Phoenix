#include "networkinterface.h"
#include "appbackend.h"
#include <QHostAddress>
#include <QNetworkReply>

NetworkInterface::NetworkInterface() :
    m_webSocket(new QWebSocket(QString(), QWebSocketProtocol::VersionLatest, this)),
    m_networkManager(new QNetworkAccessManager(this))
{
#ifdef __EMSCRIPTEN__
    connect(AppBackend::instance(), &AppBackend::webServerAddressChanged, this, [this](const QString &address) {
        m_webSocket->open(QUrl(QString("ws://%1:%2/api").arg(address).arg(8081)));
    });
#endif
    connect(m_webSocket, &QWebSocket::binaryMessageReceived, this, [this](const QByteArray &msg) {
        double sonarDistance;
        uint8_t systemStatus, mode;
        int8_t batteryRemaining;
        bool armed, buttonState;
        float x, y, z, roll, pitch, yaw;
        QList<int> arucoIds;
        QDataStream msg_stream(msg);
        msg_stream >> systemStatus >> batteryRemaining >> mode >> armed
                   >> x >> y >> z
                   >> roll >> pitch >> yaw
                   >> arucoIds >> buttonState >> sonarDistance;
        emit systemStatusChanged(systemStatus);
        emit batteryRemainingChanged(batteryRemaining);
        emit modeChanged(mode);
        emit armChanged(armed);
        emit positionChanged(x, y, z, roll, pitch, yaw);
        emit arucoIdsChanged(arucoIds);
        emit buttonStateChanged(buttonState);
        emit sonarDistanceChanged(sonarDistance);
    });
    /*
    QTimer *reconnectTimer = new QTimer(this);
    reconnectTimer->setInterval(1000);
    connect(reconnectTimer, &QTimer::timeout, this, [this] {
        if (m_webSocket->state() != QAbstractSocket::UnconnectedState) {

        }
    });
    */
    connect(m_webSocket, &QWebSocket::errorOccurred, this, [this] {
        QTimer::singleShot(1000, this, [this] {
            if (m_webSocket->state() == QAbstractSocket::UnconnectedState) {
                m_webSocket->open(m_webSocket->requestUrl());
            }
        });
    });

    //m_networkManager->connectToHost(QHostAddress(QHostAddress::Any).toString(), 8080);

    //getRequest("device/getArucoIds", [this](const QByteArray &data) {
    //    postRequest("client/debug", data);
    //});
    /*
    QTimer *timer = new QTimer(this);
    timer->setInterval(200);
    connect(timer, &QTimer::timeout, this, [this] {
        getRequest("device/data", [this](const QByteArray &data) {
            double sonarDistance;
            bool armed, buttonState;
            float x, y, z;
            QList<int> arucoIds;
            QDataStream ds(data);
            ds >> armed >> x >> y >> z >> arucoIds >> buttonState >> sonarDistance;
            emit armChanged(armed);
            emit coordsChanged(x, y, z);
            emit arucoIdsChanged(arucoIds);
            emit buttonStateChanged(buttonState);
            emit sonarDistanceChanged(sonarDistance);
        });
    });
    timer->start();
    */
}

void NetworkInterface::setMode(const QString &modeName) {
    uint8_t mode = 0;
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    if (modeName == "Stabilized") {
        mode = PX4_MAIN_MODE_STABILIZED;
    } else if (modeName == "Position") {
        mode = PX4_MAIN_MODE_POSCTL;
    } else if (modeName == "Offboard") {
        mode = PX4_MAIN_MODE_OFFBOARD;
    }
    stream << CLIENT_MSG_SET_MODE << mode;
    m_webSocket->sendBinaryMessage(data);
}

void NetworkInterface::armDisarm(bool arm) {
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << CLIENT_MSG_ARM_DISARM << arm;
    m_webSocket->sendBinaryMessage(data);
}

void NetworkInterface::takeoff(float z) {
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << CLIENT_MSG_TAKEOFF << z;
    m_webSocket->sendBinaryMessage(data);
}

void NetworkInterface::land() {
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << CLIENT_MSG_LAND;
    m_webSocket->sendBinaryMessage(data);
}

void NetworkInterface::drop() {
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << CLIENT_MSG_DROP;
    m_webSocket->sendBinaryMessage(data);
}

void NetworkInterface::setThrottlePWM(float value) {
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << CLIENT_MSG_SET_THROTTLE_PWM << value;
    m_webSocket->sendBinaryMessage(data);
}

void NetworkInterface::setRollPWM(float value) {
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << CLIENT_MSG_SET_ROLL_PWM << value;
    m_webSocket->sendBinaryMessage(data);
}

void NetworkInterface::setPitchPWM(float value) {
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << CLIENT_MSG_SET_PITCH_PWM << value;
    m_webSocket->sendBinaryMessage(data);
}

void NetworkInterface::setYawPWM(float value) {
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << CLIENT_MSG_SET_YAW_PWM << value;
    m_webSocket->sendBinaryMessage(data);
}

void NetworkInterface::align() {
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << CLIENT_MSG_ALIGN;
    m_webSocket->sendBinaryMessage(data);
}

void NetworkInterface::setX(float value)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << CLIENT_MSG_SET_X << value;
    m_webSocket->sendBinaryMessage(data);
}

void NetworkInterface::setY(float value)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << CLIENT_MSG_SET_Y << value;
    m_webSocket->sendBinaryMessage(data);
}

void NetworkInterface::setZ(float value)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << CLIENT_MSG_SET_Z << value;
    m_webSocket->sendBinaryMessage(data);
}

void NetworkInterface::setYaw(float value)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << CLIENT_MSG_SET_YAW << value;
    m_webSocket->sendBinaryMessage(data);
}

void NetworkInterface::postRequest(const QString &url, const QByteArray &data)
{
    QNetworkReply *reply = m_networkManager->post(QNetworkRequest(url), data);
    connect(reply, &QNetworkReply::finished, reply, &QObject::deleteLater);
}

template <typename Function>
void NetworkInterface::getRequest(const QString &url, Function &&func, const QByteArray &data)
{
    QNetworkReply *reply = m_networkManager->get(QNetworkRequest(url), data);
    connect(reply, &QNetworkReply::finished, reply, [func, reply] {
        func(reply->readAll());
    });
}
