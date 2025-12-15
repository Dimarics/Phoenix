#ifndef NETWORKINTERFACE_H
#define NETWORKINTERFACE_H

#include <QtQmlIntegration>
#include <QNetworkAccessManager>

class NetworkInterface : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    enum PX4_CUSTOM_MODE {
        PX4_MODE_MANUAL = 1,
        PX4_MODE_ALTCTL = 2,
        PX4_MODE_POSCTL = 3,
        PX4_MODE_AUTO = 4,
        PX4_MODE_ACRO = 5,
        PX4_MODE_OFFBOARD = 6,
        PX4_MODE_STABILIZED = 7,
        PX4_MODE_RATTITUDE = 8,
        PX4_MODE_SIMPLE = 9,
        PX4_MODE_AUTO_RTL = 10,
        PX4_MODE_AUTO_TAKEOFF = 11,
        PX4_MODE_AUTO_LAND = 12,
        PX4_MODE_AUTO_MISSION = 13,
        PX4_MODE_AUTO_LOITER = 14,
        PX4_MODE_AUTO_FOLLOW = 15,
    };
    NetworkInterface();
    inline Q_INVOKABLE void arm() { postRequest("device/arm"); }
    inline Q_INVOKABLE void setThrottle(float value) { postRequest("device/setThrottle", QByteArray::number(value * 10.f + 1000)); }
    inline Q_INVOKABLE void setRoll(float value) { postRequest("device/setRoll", QByteArray::number(value * 5.f + 1500.f)); }
    inline Q_INVOKABLE void setPitch(float value) { postRequest("device/setPitch", QByteArray::number(value * 5.f + 1500.f)); }
    inline Q_INVOKABLE void setYaw(float value) { postRequest("device/setYaw", QByteArray::number(value * 5.f + 1500.f)); }
    inline Q_INVOKABLE void align() { postRequest("device/align"); }

    inline Q_INVOKABLE void log(const QString &text) { postRequest("client/debug", text.toUtf8()); }

private:
    QNetworkAccessManager *m_networkManager;
    void postRequest(const QString &url, const QByteArray &data = {});
    template <typename Function>
    void getRequest(const QString &url, Function &&func, const QByteArray &data = {});

signals:
    void sonarDistanceChanged(double value);
    void arucoIdsChanged(const QList<int> &);

};

#endif // NETWORKINTERFACE_H
