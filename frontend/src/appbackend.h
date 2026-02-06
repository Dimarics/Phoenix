#ifndef APPBACKEND_H
#define APPBACKEND_H

#pragma once
//#include <QObject>
#include <QtQmlIntegration>
#include <QQmlEngine>

#ifdef __EMSCRIPTEN__
#include <QHostAddress>
#else
#include <QtSerialPort/QSerialPortInfo>
#endif

class AppBackend : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    Q_PROPERTY(QString tempLocation READ tempLocation CONSTANT)
    Q_PROPERTY(QString appDataLocation READ appDataLocation CONSTANT)
    Q_PROPERTY(QString deviceName READ deviceName WRITE setDeviceName NOTIFY deviceNameChanged)
    Q_PROPERTY(QStringList availableDevices READ availableDevices WRITE setAvailableDevices NOTIFY availableDevicesChanged)
    Q_PROPERTY(QObject *device READ device WRITE setDevice NOTIFY deviceChanged)
#ifndef __EMSCRIPTEN__
    Q_PROPERTY(QStringList availablePorts READ availablePorts NOTIFY availablePortsChanged)
#endif

public:
    AppBackend();
    static inline QString tempLocation() { return QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/Hobots/"; }
    static inline QString appDataLocation() { return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/"; }
    static inline AppBackend* instance() { static AppBackend *instance = new AppBackend; return instance; };
    static inline AppBackend* qmlInstance(QQmlEngine*, QJSEngine*) { return instance(); }
    inline QString deviceName() { return m_deviceName; }
    inline void setDeviceName(const QString &deviceName) { m_deviceName = deviceName; emit deviceNameChanged(); }
    void setAvailableDevices(const QStringList &availableDevices);
    inline QStringList availableDevices() const { return m_availableDevices; }
    void setDevice(QObject *device);
    inline QObject *device() const { return m_device; }
    Q_INVOKABLE void saveFile(const QByteArray &data, const QString &path) const;
    Q_INVOKABLE void saveFile(const QByteArray &data, const QUrl &url) const;
    Q_INVOKABLE QByteArray readFile(const QString &path) const;
    Q_INVOKABLE QByteArray readFile(const QUrl &url) const;
    Q_INVOKABLE QVariant readJSON(const QString &path) const;
    Q_INVOKABLE QVariant readJSON(const QUrl &url) const;
#ifdef __EMSCRIPTEN__
    inline void setWebServerPort(quint16 port) { m_webServerPort = port; }
    inline void setWebServerAddress(QString address) { m_webServerAddress = address; }
    inline quint16 webServerPort() const { return m_webServerPort; }
    inline QString webServerAddress() const { return m_webServerAddress; }
    static void setWebServerUrl(const std::string &href);
    static void setWebServerAddress(const std::string &address);
    inline QUrl webServerUrl() { return m_webServerUrl; }
#else
    inline QStringList availablePorts() const { return m_serialPorts; }
    Q_INVOKABLE bool loadModel(const QString &lib_name) const;
#endif

private:
    QString m_deviceName;
    QStringList m_availableDevices;
    QObject *m_device;
#ifdef __EMSCRIPTEN__
    quint16 m_webServerPort;
    QString m_webServerAddress;
    QUrl m_webServerUrl;
#else
    QStringList m_serialPorts;
    QLibrary *m_model;
    void timerEvent(QTimerEvent*) override;
#endif

signals:
    void deviceNameChanged();
    void deviceChanged();
    void availableDevicesChanged();
#ifdef __EMSCRIPTEN__
    void webServerAddressChanged(const QString &address);
#else
    void availablePortsChanged();
#endif
};

#endif // APPBACKEND_H
