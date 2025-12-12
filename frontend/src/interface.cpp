#include "interface.h"
#include <QHostAddress>

Interface::Interface() : m_networkManager(new QNetworkAccessManager(this))
{
    m_networkManager->connectToHost(QHostAddress(QHostAddress::Any).toString(), 80);
}

void Interface::postRequest(const QString &url, const QByteArray &data)
{
    QNetworkRequest request(url);
    m_networkManager->post(request, data);
}
