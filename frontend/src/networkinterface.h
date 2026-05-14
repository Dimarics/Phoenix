#ifndef NETWORKINTERFACE_H
#define NETWORKINTERFACE_H

#include <QtQmlIntegration>
#include <QNetworkAccessManager>
#include <QWebSocket>

class NetworkInterface : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    typedef enum MAV_STATE
    {
       MAV_STATE_UNINIT=0, /* Uninitialized system, state is unknown. | */
       MAV_STATE_BOOT=1, /* System is booting up. | */
       MAV_STATE_CALIBRATING=2, /* System is calibrating and not flight-ready. | */
       MAV_STATE_STANDBY=3, /* System is grounded and on standby. It can be launched any time. | */
       MAV_STATE_ACTIVE=4, /* System is active and might be already airborne. Motors are engaged. | */
       MAV_STATE_CRITICAL=5, /* System is in a non-normal flight mode (failsafe). It can however still navigate. | */
       MAV_STATE_EMERGENCY=6, /* System is in a non-normal flight mode (failsafe). It lost control over parts or over the whole airframe. It is in mayday and going down. | */
       MAV_STATE_POWEROFF=7, /* System just initialized its power-down sequence, will shut down now. | */
       MAV_STATE_FLIGHT_TERMINATION=8, /* System is terminating itself (failsafe or commanded). | */
       MAV_STATE_ENUM_END=9, /*  | */
    } MAV_STATE;
    enum PX4_MAIN_MODE {
        PX4_MAIN_MODE_MANUAL = 1,
        PX4_MAIN_MODE_ALTCTL,
        PX4_MAIN_MODE_POSCTL,
        PX4_MAIN_MODE_AUTO,
        PX4_MAIN_MODE_ACRO,
        PX4_MAIN_MODE_OFFBOARD,
        PX4_MAIN_MODE_STABILIZED,
        PX4_MAIN_MODE_RATTITUDE,
        PX4_MAIN_MODE_SIMPLE
    };
    enum PX4_SUB_MODE_AUTO {
        PX4_SUB_MODE_AUTO_READY = 1,
        PX4_SUB_MODE_AUTO_TAKEOFF,
        PX4_SUB_MODE_AUTO_LOITER,
        PX4_SUB_MODE_AUTO_MISSION,
        PX4_SUB_MODE_AUTO_RTL,
        PX4_SUB_MODE_AUTO_LAND,
        PX4_SUB_MODE_AUTO_FOLLOW_TARGET,
        PX4_SUB_MODE_AUTO_PRECLAND
    };
    enum CLIENT_MSG_ID: quint8 {
        CLIENT_MSG_SET_MODE = 1,
        CLIENT_MSG_ARM_DISARM,
        CLIENT_MSG_TAKEOFF,
        CLIENT_MSG_LAND,
        CLIENT_MSG_DROP,
        CLIENT_MSG_SET_THROTTLE_PWM,
        CLIENT_MSG_SET_ROLL_PWM,
        CLIENT_MSG_SET_PITCH_PWM,
        CLIENT_MSG_SET_YAW_PWM,
        CLIENT_MSG_ALIGN,
        CLIENT_MSG_SET_X,
        CLIENT_MSG_SET_Y,
        CLIENT_MSG_SET_Z,
        CLIENT_MSG_SET_YAW
    };
    Q_ENUM(PX4_MAIN_MODE)
    NetworkInterface();
    /*
    inline Q_INVOKABLE void armDisarm(bool arm) { postRequest("device/arm", QByteArray::number(arm)); }
    inline Q_INVOKABLE void setThrottle(float value) { postRequest("device/setThrottle", QByteArray::number(value * 10.f + 1000)); }
    inline Q_INVOKABLE void setRoll(float value) { postRequest("device/setRoll", QByteArray::number(value * 5.f + 1500.f)); }
    inline Q_INVOKABLE void setPitch(float value) { postRequest("device/setPitch", QByteArray::number(value * 5.f + 1500.f)); }
    inline Q_INVOKABLE void setYaw(float value) { postRequest("device/setYaw", QByteArray::number(value * 5.f + 1500.f)); }
    inline Q_INVOKABLE void align() { postRequest("device/align"); }
    */
    Q_INVOKABLE void setMode(const QString &modeName);
    Q_INVOKABLE void armDisarm(bool arm);
    Q_INVOKABLE void takeoff(float z);
    Q_INVOKABLE void land();
    Q_INVOKABLE void drop();
    Q_INVOKABLE void setThrottlePWM(float value);
    Q_INVOKABLE void setRollPWM(float value);
    Q_INVOKABLE void setPitchPWM(float value);
    Q_INVOKABLE void setYawPWM(float value);
    Q_INVOKABLE void align();
    Q_INVOKABLE void setX(float value);
    Q_INVOKABLE void setY(float value);
    Q_INVOKABLE void setZ(float value);
    Q_INVOKABLE void setYaw(float value);

    inline Q_INVOKABLE void holdOnMarker(bool hold) { postRequest("device/holdOnMarker", QByteArray::number(hold)); }
    inline Q_INVOKABLE void log(const QString &text) { postRequest("client/debug", text.toUtf8()); }

private:
    QWebSocket *m_webSocket;
    QNetworkAccessManager *m_networkManager;
    void postRequest(const QString &url, const QByteArray &data = {});
    template <typename Function>
    void getRequest(const QString &url, Function &&func, const QByteArray &data = {});

signals:
    void systemStatusChanged(uint8_t systemStatus);
    void batteryRemainingChanged(int8_t batteryRemaining);
    void modeChanged(uint8_t mode);
    void armChanged(bool state);
    void reached();
    void positionChanged(float x, float y, float z, float roll, float pitch, float yaw);
    void arucoIdsChanged(const QList<int> &arucoIds);
    void buttonStateChanged(bool state);
    void sonarDistanceChanged(double value);
};

#endif // NETWORKINTERFACE_H
