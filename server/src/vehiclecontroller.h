#ifndef VEHICLECONTROLLER_H
#define VEHICLECONTROLLER_H

#include "mavlinkinterface.h"
#include "videocapture.h"
#include "arucodetector.h"
#include "server.h"
#include <QObject>

class VehicleController : public QObject
{
    Q_OBJECT
public:
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
    enum State: quint8 {
        ON_GROUND = 1,
        TAKEOFF,
        IN_AIR,
        LANDING,
        MISSING,
    };
    VehicleController(QObject *parent = nullptr);
    void takeoff(float z);
    void land();
    void setPoint(float x, float y, float z, float yaw);
    bool checkPoint();

private:
    bool m_movingToPoint;
    quint64 m_losslessTime;
    quint64 m_takeoffStartTime;
    quint64 m_restTime;
    float m_takeoffTarget = 0;
    State m_state;
    MAVLinkInterface *m_mavlinkInterface;
    VideoCapture *m_videoCapture;
    ArucoDetector *m_arucoDetector;
    Server *m_server;
};

#endif // VEHICLECONTROLLER_H
