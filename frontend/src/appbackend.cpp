#include "appbackend.h"

#ifndef __EMSCRIPTEN__
AppBackend::AppBackend() : m_availableDevices {
                               "Хобот 1 М1",
                               "Хобот 2",
                               "Хобот Д",
                               "Хобот L",
                               "Хобот L2",
                               "Хобот L3"
                               },
    m_device(nullptr),
    m_model(new QLibrary(this))
{
    startTimer(200);
}
#else

//#include <QNetworkAccessManager>
#include <emscripten.h>
#include <emscripten/bind.h>

EMSCRIPTEN_BINDINGS(appbackend) {
    emscripten::function("setWebServerUrl", &AppBackend::setWebServerUrl);
    emscripten::function("setWebServerAddress", &AppBackend::setWebServerAddress);
}

void AppBackend::setWebServerUrl(const std::string &href) {
    AppBackend::instance()->m_webServerUrl = QUrl(QString::fromStdString(href));
}

void AppBackend::setWebServerAddress(const std::string &address)
{
    AppBackend::instance()->m_webServerAddress = QString::fromStdString(address);
}

AppBackend::AppBackend() : m_device(nullptr) {
    /*
    QNetworkAccessManager *networkManager = new QNetworkAccessManager(this);
    networkManager->connectToHost(QHostAddress(QHostAddress::Any).toString(), 8080);
    QNetworkReply *reply = networkManager->get(QNetworkRequest(""));
    connect(reply, &QNetworkReply::finished, reply, [func, reply] {
        func(reply->readAll());
    });
    */
    QMetaObject::invokeMethod(this, [this]() {
        EM_ASM({
                   Module.setWebServerUrl(window.location.href);
                   Module.setWebServerAddress(window.location.hostname)
               });
        emit webServerAddressChanged(m_webServerAddress);
    }, Qt::QueuedConnection);
}
#endif

void AppBackend::setAvailableDevices(const QStringList &availableDevices) {
    m_availableDevices = availableDevices;
    emit availableDevicesChanged();
}

void AppBackend::setDevice(QObject *device)
{
    if (device == m_device) return;
    if (m_device) m_device->deleteLater();//QMetaObject::invokeMethod(m_device, "destroy");
    m_device = device;
    emit deviceChanged();
}

void AppBackend::saveFile(const QByteArray &data, const QString &path) const
{
    QFileInfo fileInfo(path);
    if (!fileInfo.dir().exists()) QDir().mkpath(fileInfo.path());
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Не удалось сохранить файл";
        return;
    }
    //file.write(data.toUtf8());
    file.write(data);
}

void AppBackend::saveFile(const QByteArray &data, const QUrl &url) const
{
    saveFile(data, url.toLocalFile());
}

QByteArray AppBackend::readFile(const QString &path) const
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Невозможно открыть файл";
        return {};
    }
    return file.readAll();
}

QByteArray AppBackend::readFile(const QUrl &url) const
{
    return readFile(url.toLocalFile());
}

QVariant AppBackend::readJSON(const QString &path) const
{
    QJsonParseError error;
    QByteArray rawData = readFile(path);
    if (rawData.isEmpty()) {
        qWarning() << "Файл пуст или не существует"; return {};
    }
    QJsonDocument json = QJsonDocument::fromJson(rawData, &error);
    if (error.error != QJsonParseError::NoError) {
        qWarning() << error.errorString(); return {};
    }
    return json.toVariant();
}

QVariant AppBackend::readJSON(const QUrl &url) const
{
    return readJSON(url.toLocalFile());
}

#ifndef __EMSCRIPTEN__

void AppBackend::timerEvent(QTimerEvent*) {
    const QList<QSerialPortInfo> new_list = QSerialPortInfo::availablePorts();

    bool replace = false;
    if (m_serialPorts.size() != new_list.size()) {
        replace = true;
    } else {
        for (qsizetype i = 0; i < m_serialPorts.size(); ++i)
            if (m_serialPorts.at(i) != new_list.at(i).portName())
                replace = true;
    }
    if (replace) {
        m_serialPorts.clear();
        for (const QSerialPortInfo &serialPortInfo : QSerialPortInfo::availablePorts()) {
            m_serialPorts << serialPortInfo.portName();
        }
        emit availablePortsChanged();
    }
}

bool AppBackend::loadModel(const QString &lib_name) const
{
    const QString lib_path = "models/" + lib_name;
    if (m_model->isLoaded()) {
        if (lib_path == m_model->fileName()) {
            return true;
        } else {
            m_model->unload();
        }
    }
    m_model->setFileName(lib_path);
    if (!m_model->load()) {
        qWarning() << m_model->errorString();
        return false;
    }
    return true;
}

#endif
