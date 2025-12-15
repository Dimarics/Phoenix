#include "networkinterface.h"
#include <QHostAddress>
#include <QNetworkReply>

NetworkInterface::NetworkInterface() : m_networkManager(new QNetworkAccessManager(this))
{
    m_networkManager->connectToHost(QHostAddress(QHostAddress::Any).toString(), 80);

    //getRequest("device/getArucoIds", [this](const QByteArray &data) {
    //    postRequest("client/debug", data);
    //});
    QTimer *timer = new QTimer(this);
    timer->setInterval(20);
    connect(timer, &QTimer::timeout, this, [this] {
        getRequest("device/data", [this](const QByteArray &data) {
            double sonarDistance;
            QList<int> arucoIds;
            QDataStream ds(data);
            ds >> sonarDistance >> arucoIds;
            emit sonarDistanceChanged(sonarDistance);
            emit arucoIdsChanged(arucoIds);
        });
    });
    timer->start();
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
