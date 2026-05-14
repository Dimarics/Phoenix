#ifndef MAVLINKINTERFACE_H
#define MAVLINKINTERFACE_H

#include <common/mavlink.h>
#include <QtSerialPort/QSerialPort>
#include <QTimer>

class MAVLinkInterface : public QObject
{
public:
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
        PX4_SUB_MODE_AUTO_TAKEOFF = 2,
        PX4_SUB_MODE_AUTO_LOITER,
        PX4_SUB_MODE_AUTO_MISSION,
        PX4_SUB_MODE_AUTO_RTL,
        PX4_SUB_MODE_AUTO_LAND,
        PX4_SUB_MODE_AUTO_FOLLOW_TARGET,
        PX4_SUB_MODE_AUTO_PRECLAND
    };
    enum CONTROL_MODE {
        MAV_CONTROL_SPEED = 1,
        MAV_CONTROL_COORDS
    };
    MAVLinkInterface(QObject *parent = nullptr);
    void sendMavlinkMessage(const QByteArray &message);
    void sendMavlinkMessage(mavlink_message_t message);

    inline uint8_t systemStatus() const { return m_systemStatus; }
    inline int8_t batteryRemaining() const { return m_batteryRemaining; }
    inline uint8_t mode() const { return m_mainMode; }
    inline bool armed() const { return m_armed; }
    inline float x() const { return m_visionPos.y; }
    inline float y() const { return m_visionPos.x; }
    inline float z() const { return -m_visionPos.z; }
    inline float vx() const { return m_vx; }
    inline float vy() const { return m_vy; }
    inline float vz() const { return m_vz; }
    inline float roll() const { return m_roll; }
    inline float pitch() const { return m_pitch; }
    inline float yaw() const { return -m_visionPos.yaw; }
    inline float targetX() const { return m_targetX; }
    inline float targetY() const { return m_targetY; }
    inline float targetZ() const { return m_targetZ; }
    inline float targetVX() const { return m_targetVX; }
    inline float targetVY() const { return m_targetVY; }
    inline float targetVZ() const { return m_targetVZ; }
    inline float targetYaw() const { return m_targetYaw; }
    inline float distanceToPoint() const {
        return sqrt(pow(m_targetX - m_x, 2) + pow(m_targetY - m_y, 2) + pow(m_targetZ - m_z, 2));
    }
    inline float throttlePWM() const { return m_throttlePWM; }
    inline float rollPWM() const { return m_rollPWM; }
    inline float pitchPWM() const { return m_pitchPWM; }
    inline float yawPWM() const { return m_yawPWM; }
    inline uint16_t voltageBattery() const { return m_voltageBattery; }
    inline mavlink_odometry_t odometry() const { return m_odometry; }

public slots:
    void setMode(uint8_t mode);
    void armDisarm(bool arm);
    void takeoff(float z);
    void land();
    void drop();

    inline void forward() { m_pitchPWM = 1700; }
    inline void backward() { m_pitchPWM = 1300; }
    inline void left() { m_rollPWM = 1300; }
    inline void right() { m_rollPWM = 1700; }
    inline void align() { m_rollPWM = 1500; m_pitchPWM = 1500; }

    void setX(float value);
    void setY(float value);
    void setZ(float value);
    void setYaw(float value);
    void setPos(float x, float y, float z, float yaw, uint64_t timestamp = 0);
    void setTargetX(float value);
    void setTargetY(float value);
    void setTargetZ(float value);
    void setTargetYaw(float value);
    void setTargetPos(float x, float y, float z, float yaw);
    void setVelocities(float vx, float vy, float vz);
    void setPositionTargetTypeMask(uint16_t type_mask);
    void setThrottlePWM(float value);
    void setRollPWM(float value);
    void setPitchPWM(float value);
    void setYawPWM(float value);
    inline void setTimestamp(uint64_t value) { m_visionPos.usec = value; }
    inline void resetVisionPos() { ++m_visionPos.reset_counter; }

private:
    uint8_t m_systemId = 2;
    uint8_t m_componentId = 191;
    uint8_t m_targetSystemId = 1;
    uint8_t m_targetComponentId = 1;
    uint8_t m_systemStatus = 0;
    int8_t m_batteryRemaining = 0;
    uint8_t m_mainMode = 0;
    uint8_t m_subMode = 0;
    uint8_t m_targetMode = PX4_MAIN_MODE_OFFBOARD;
    uint8_t m_landedState = MAV_LANDED_STATE_UNDEFINED;
    bool m_autoModeEnabled = false;
    bool m_armed = false;
    bool m_initializeSystem = false;

    // Примечание: координаты X, Y камеры - это координаты Y, X коптера
    float m_x = 0;
    float m_y = 0;
    float m_z = 0;
    float m_vx = 0;
    float m_vy = 0;
    float m_vz = 0;
    float m_roll = 0;
    float m_pitch = 0;
    float m_yaw = 0;

    /*
     * covariance - степень доверия
     * размерность - метры возведённые в квадрат
     * 0 - точность по X коптера (Y камеры)
     * 2 - точность по Y коптера (X камеры)
     * 5 - точность по Z
     * значения в координатах коптера
     */
    float m_covariance[21];

    float m_targetX = 0;
    float m_targetY = 0;
    float m_targetZ = 0;
    float m_targetVX = 0;
    float m_targetVY = 0;
    float m_targetVZ = 0;
    float m_targetRoll = 0;
    float m_targetPitch = 0;
    float m_targetYaw = 0;
    float m_throttlePWM = 1000;
    float m_rollPWM = 1500;
    float m_pitchPWM = 1500;
    float m_yawPWM = 1500;
    mavlink_odometry_t m_odometry = {};
    mavlink_vision_position_estimate_t m_visionPos = {};

    int m_heartbeatTimerId;
    int m_positionTimerId;
    int m_targetPosTimerId;
    uint16_t m_positionTargetTypeMask = 0;
    uint16_t m_voltageBattery;

    mavlink_message_t m_message;
    mavlink_status_t m_status;
    QSerialPort *m_serialPort;
    QTimer *m_disconnectTimer;

    void sendHeartbeat();
    void timerEvent(QTimerEvent *event) override;
};

#endif // MAVLINKINTERFACE_H
