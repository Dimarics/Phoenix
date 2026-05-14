#ifndef VEHICLECONTROLLER_H
#define VEHICLECONTROLLER_H

#include "i2c.h"
#include "mavlinkinterface.h"
#include "rgbled.h"
#include "scriptworker.h"
#include "videocapture.h"
#include "arucodetector.h"
#include "server.h"
#include "vl53l0x.h"
#include <QObject>

class VehicleController : public QObject
{
    Q_OBJECT
public:
    enum MSG_CLIENT_ID: quint8 {
        MSG_CLIENT_SET_MODE = 1,
        MSG_CLIENT_ARM_DISARM,
        MSG_CLIENT_TAKEOFF,
        MSG_CLIENT_LAND,
        MSG_CLIENT_DROP,
        MSG_CLIENT_SET_THROTTLE_PWM,
        MSG_CLIENT_SET_ROLL_PWM,
        MSG_CLIENT_SET_PITCH_PWM,
        MSG_CLIENT_SET_YAW_PWM,
        MSG_CLIENT_ALIGN,
        MSG_CLIENT_SET_X,
        MSG_CLIENT_SET_Y,
        MSG_CLIENT_SET_Z,
        MSG_CLIENT_SET_YAW,
    };
    enum I2C_REQUEST_ID: uint8_t {
        I2C_REQUEST_GET_DISTANCE = 1,
        I2C_REQUEST_GET_SONARS_DATA
    };
    enum State: quint8 {
        ON_GROUND = 1,
        TAKEOFF,
        ORIENTATION,
        IN_AIR,
        LANDING,
        MISSING,
    };
    VehicleController(QObject *parent = nullptr);
    ~VehicleController();
    void takeoff(float z);
    void land();
    void drop();
    void setPoint(float x, float y, float z, float yaw);
    bool checkPoint();

private:
    bool m_movingToPoint;
    quint64 m_losslessTime;
    quint64 m_takeoffStartTime;
    quint64 m_restTime;
    float m_takeoffTarget = 0;
    State m_state;
    RGBLed m_RGBLed;
    I2C m_i2c;
    VL53L0X m_dalnomer;
    MAVLinkInterface *m_mavlinkInterface;
    VideoCapture *m_videoCapture;
    ArucoDetector *m_arucoDetector;
    Server *m_server;
    ScriptWorker *m_scriptWorker;
};

#endif // VEHICLECONTROLLER_H
