#ifndef MAVLINKINTERFACE_H
#define MAVLINKINTERFACE_H

#pragma once

#include <common/mavlink.h>
#include <QtSerialPort/QSerialPort>

class MAVLinkInterface : public QObject
{
public:
    enum PX4_CUSTOM_MAIN_MODE {
        PX4_CUSTOM_MAIN_MODE_MANUAL = 1,
        PX4_CUSTOM_MAIN_MODE_ALTCTL,
        PX4_CUSTOM_MAIN_MODE_POSCTL,
        PX4_CUSTOM_MAIN_MODE_AUTO,
        PX4_CUSTOM_MAIN_MODE_ACRO,
        PX4_CUSTOM_MAIN_MODE_OFFBOARD,
        PX4_CUSTOM_MAIN_MODE_STABILIZED,
        PX4_CUSTOM_MAIN_MODE_RATTITUDE,
        PX4_CUSTOM_MAIN_MODE_SIMPLE
    };
    enum PX4_CUSTOM_SUB_MODE_AUTO {
        PX4_CUSTOM_SUB_MODE_AUTO_READY = 1,
        PX4_CUSTOM_SUB_MODE_AUTO_TAKEOFF,
        PX4_CUSTOM_SUB_MODE_AUTO_LOITER,
        PX4_CUSTOM_SUB_MODE_AUTO_MISSION,
        PX4_CUSTOM_SUB_MODE_AUTO_RTL,
        PX4_CUSTOM_SUB_MODE_AUTO_LAND,
        PX4_CUSTOM_SUB_MODE_AUTO_FOLLOW_TARGET,
        PX4_CUSTOM_SUB_MODE_AUTO_PRECLAND
    };
    MAVLinkInterface(QObject *parent = nullptr);
    void sendMavlinkMessage(const QByteArray &message);
    void sendMavlinkMessage(mavlink_message_t message);

    void arm();
    inline void forward() { m_pitch = 1700; }
    inline void backward() { m_pitch = 1300; }
    inline void left() { m_roll = 1300; }
    inline void right() { m_roll = 1700; }
    inline void align() { m_roll = 1500; m_pitch = 1500; }

    inline void setThrottle(float value) { m_throttle = value; }
    inline void setRoll(float value) { m_roll = value; }
    inline void setPitch(float value) { m_pitch = value; }
    inline void setYaw(float value) { m_yaw = value; }
    inline bool armed() { return m_systemStatus == MAV_STATE_ACTIVE; }

private:
    int m_heartbeatTimerId;
    int m_attitudeTimerId;

    uint8_t m_systemStatus;
    float m_throttle = 0;
    float m_roll = 1500;
    float m_pitch = 1500;
    float m_yaw = 1500;
    mavlink_message_t m_message;
    mavlink_status_t m_status;
    QSerialPort *m_serialPort;

    void timerEvent(QTimerEvent *event) override;
};

extern MAVLinkInterface *mavlinkInterface;

#endif // MAVLINKINTERFACE_H
