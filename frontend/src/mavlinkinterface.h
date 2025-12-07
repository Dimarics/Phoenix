#ifndef MAVLINKINTERFACE_H
#define MAVLINKINTERFACE_H

#include <common/mavlink.h>
#include <QtQmlIntegration>
#include <QNetworkAccessManager>

class MAVLinkInterface : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    enum PX4_CUSTOM_MODE {
        PX4_MODE_MANUAL = 0,
        PX4_MODE_ALTCTL = 1,
        PX4_MODE_POSCTL = 2,
        PX4_MODE_AUTO = 3,
        PX4_MODE_ACRO = 4,
        PX4_MODE_OFFBOARD = 5,
        PX4_MODE_STABILIZED = 6,
        PX4_MODE_RATTITUDE = 7,
        PX4_MODE_SIMPLE = 8,
        PX4_MODE_AUTO_RTL = 9,
        PX4_MODE_AUTO_TAKEOFF = 10,
        PX4_MODE_AUTO_LAND = 11,
        PX4_MODE_AUTO_MISSION = 12,
        PX4_MODE_AUTO_LOITER = 13,
        PX4_MODE_AUTO_FOLLOW = 14,
    };
    MAVLinkInterface();
    Q_INVOKABLE void arm();

private:
    mavlink_message_t m_message;
    mavlink_status_t m_status;
    QNetworkAccessManager *m_networkManager;
    void sendMavlinkMessage(mavlink_message_t message);
};

#endif // MAVLINKINTERFACE_H
