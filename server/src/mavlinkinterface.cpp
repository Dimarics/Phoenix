#include "mavlinkinterface.h"
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QTimerEvent>
#include <QTimer>

MAVLinkInterface *mavlinkInterface = nullptr;

MAVLinkInterface::MAVLinkInterface(QObject *parent) : QObject(parent),
    m_systemStatus(MAV_STATE_UNINIT),
    m_armed(false),
    m_serialPort(new QSerialPort(this)),
    m_disconnectTimer(new QTimer(this))
{
    m_visionPos.x = 0;
    m_visionPos.y = 0;
    m_visionPos.z = 0;
    m_visionPos.roll = 0;
    m_visionPos.pitch = 0;
    m_visionPos.yaw = 0;
    std::memset(m_visionPos.covariance, 0, sizeof(m_covariance));
    m_visionPos.covariance[0] = 1e6f; // x (y)
    m_visionPos.covariance[6] = 1e6f; // y (x)
    m_visionPos.covariance[11] = 1e6f; // z
    //m_visionPos.covariance[15] = 1e6f; // roll
    //m_visionPos.covariance[18] = 1e6f; // pitch
    //m_visionPos.covariance[20] = 1e6f; // yaw
    m_visionPos.reset_counter = 0;

    m_serialPort->setBaudRate(921600);

    connect(m_serialPort, &QSerialPort::readyRead, this, [this]() {
        QByteArray data = m_serialPort->readAll();
        //qDebug() << data;
        for (uint8_t byte : data) {
            if (mavlink_parse_char(MAVLINK_COMM_0, byte, &m_message, &m_status)) {
                //if (m_message.msgid != MAVLINK_MSG_ID_HEARTBEAT) qDebug() << m_message.msgid;
                switch (m_message.msgid) {
                case MAVLINK_MSG_ID_HEARTBEAT: {
                    mavlink_heartbeat_t heartbeat;
                    mavlink_msg_heartbeat_decode(&m_message, &heartbeat);

                    m_systemStatus = heartbeat.system_status;
                    m_mainMode = (heartbeat.custom_mode >> 16) & 0xFF;
                    m_subMode = (heartbeat.custom_mode >> 24) & 0xFF;
                    m_armed = heartbeat.base_mode & MAV_MODE_FLAG_SAFETY_ARMED;
                    m_autoModeEnabled = heartbeat.base_mode & MAV_MODE_FLAG_AUTO_ENABLED;

                    //qDebug() << "=== PIXHAWK HEARTBEAT RECEIVED ===";
                    //qDebug() << "System ID:" << (int)m_message.sysid;
                    //qDebug() << "Component ID:" << (int)m_message.compid;
                    //qDebug() << "Autopilot:" << heartbeat.autopilot;
                    //qDebug() << "Type:" << heartbeat.type;
                    //qDebug() << "Base mode:";
                    //qDebug() << "    Custom mode enabled:" << bool(heartbeat.base_mode & MAV_MODE_FLAG_CUSTOM_MODE_ENABLED);
                    //qDebug() << "    Armed:" << bool(heartbeat.base_mode & MAV_MODE_FLAG_SAFETY_ARMED);

                    //qDebug() << "Main mode:" << main_mode;
                    //qDebug() << "Sub mode:" << sub_mode;
                    //qDebug() << "State:" << heartbeat.system_status;

                    m_disconnectTimer->start();

                    // инициализирует систему, если был принят первый heartbeat
                    if (m_initializeSystem) {
                        m_initializeSystem = false;
                        qDebug() << "initialize system";
                        sendHeartbeat();
                        setMode(m_targetMode);

                        mavlink_message_t msg;
                        mavlink_msg_command_long_pack(m_systemId, m_componentId, &msg, m_targetSystemId, m_targetComponentId,
                                                      MAV_CMD_SET_MESSAGE_INTERVAL, 0, MAVLINK_MSG_ID_ODOMETRY, 30'000, 0, 0, 0, 0, 0);
                        sendMavlinkMessage(msg);
                    }
                }   break;
                case MAVLINK_MSG_ID_COMMAND_ACK: {
                    mavlink_command_ack_t ack;
                    mavlink_msg_command_ack_decode(&m_message, &ack);
                    qDebug() << "=== PIXHAWK COMMAND ASK RECEIVED ===";
                    qDebug() << "Command:" << ack.command;
                    qDebug() << "Result:" << ack.result;
                    switch (ack.command) {
                    case MAV_CMD_DO_SET_MODE:
                        if (ack.result == MAV_RESULT_ACCEPTED) m_mainMode = m_targetMode;
                        break;
                    case MAV_CMD_COMPONENT_ARM_DISARM:
                        if (ack.result == MAV_RESULT_ACCEPTED) {
                            m_systemStatus = MAV_STATE_ACTIVE;
                            m_armed = !m_armed;
                        }
                        break;
                    default:
                        break;
                    }
                }   break;
                case MAVLINK_MSG_ID_SYS_STATUS: {
                    mavlink_sys_status_t sys_status;
                    mavlink_msg_sys_status_decode(&m_message, &sys_status);
                    m_voltageBattery = sys_status.voltage_battery;
                    m_batteryRemaining = sys_status.battery_remaining;
                    /*
                    qDebug() << "=== BATTERY STATUS RECEIVED ===";
                    qDebug() << "Battery level" << sys_status.battery_remaining;
                    qDebug() << "Voltage" << sys_status.voltage_battery;
                    qDebug() << "Amperage" << sys_status.current_battery;
                    */
                }   break;
                case MAVLINK_MSG_ID_ATTITUDE: {
                    mavlink_attitude_t attitude;
                    mavlink_msg_attitude_decode(&m_message, &attitude);
                    m_roll = attitude.roll;
                    m_pitch = attitude.pitch;
                    //m_yaw = attitude.yaw;
                    //qDebug() << attitude.roll * 180 / M_PI << attitude.pitch * 180 / M_PI;
                }   break;
                    /*
                case MAVLINK_MSG_ID_LOCAL_POSITION_NED: {
                    mavlink_local_position_ned_t lpn;
                    mavlink_msg_local_position_ned_decode(&m_message, &lpn);
                    qDebug() << "LOCAL POSITION NED:";
                    qDebug() << "x:" << QString::number(lpn.x, 'f', 5);
                    qDebug() << "y:" << QString::number(lpn.y, 'f', 5);
                    qDebug() << "z:" << QString::number(lpn.z, 'f', 5);
                    qDebug() << "vx:" << QString::number(lpn.vx, 'f', 5);
                    qDebug() << "vy:" << QString::number(lpn.vy, 'f', 5);
                    qDebug() << "vz:" << QString::number(lpn.vz, 'f', 5);
                }   break;
                    */
                case MAVLINK_MSG_ID_ODOMETRY: {
                    mavlink_odometry_t odometry;
                    mavlink_msg_odometry_decode(&m_message, &odometry);
                    m_vx = odometry.vy;
                    m_vy = odometry.vx;
                    m_vz = -odometry.vz;
                    m_odometry.x = odometry.y;
                    m_odometry.y = odometry.x;
                    m_odometry.z = -odometry.z;
                    /*
                    qDebug() << "ODOMETRY:";
                    //qDebug() << "estimator_type:"<< odometry.estimator_type << MAV_FRAME_LOCAL_NED;
                    qDebug() << "x:" << QString::number(odometry.y, 'f', 5);
                    qDebug() << "y:" << QString::number(odometry.x, 'f', 5);
                    qDebug() << "z:" << QString::number(odometry.z, 'f', 5);
                    qDebug() << "vx:" << QString::number(odometry.vy, 'f', 5);
                    qDebug() << "vy:" << QString::number(odometry.vx, 'f', 5);
                    qDebug() << "vz:" << QString::number(odometry.vz, 'f', 5);
                    */
                }   break;
                case MAVLINK_MSG_ID_EXTENDED_SYS_STATE: {
                    mavlink_extended_sys_state_t state;
                    mavlink_msg_extended_sys_state_decode(&m_message, &state);
                    /*
                    if (m_landedState == MAV_LANDED_STATE_LANDING && state.landed_state == MAV_LANDED_STATE_ON_GROUND) {
                        setMode(m_mainMode);
                    }
                    */
                    //qDebug() << m_subMode;
                    if (m_subMode == PX4_SUB_MODE_AUTO_TAKEOFF && state.landed_state != MAV_LANDED_STATE_TAKEOFF) {
                        setMode(m_targetMode);
                    }
                    if (m_subMode == PX4_SUB_MODE_AUTO_LAND && state.landed_state != MAV_LANDED_STATE_LANDING && !m_armed ) {
                        setMode(m_targetMode);
                    }
                    m_landedState = state.landed_state;
                }   break;
                default:
                    break;
                }
            }
        }
    });
    connect(m_serialPort, &QSerialPort::errorOccurred, this, [this](QSerialPort::SerialPortError error) {
        if (error == QSerialPort::NoError) return;
        switch (error) {
        case QSerialPort::DeviceNotFoundError:
        case QSerialPort::PermissionError:
        case QSerialPort::ResourceError:
            m_serialPort->close();
            break;
        default: break;
        }
        qDebug() << error;
    });
    m_heartbeatTimerId = startTimer(1000);
    m_positionTimerId = startTimer(30);
    m_targetPosTimerId = startTimer(50);

    m_disconnectTimer->setInterval(3000);
    m_disconnectTimer->setSingleShot(true);
    m_disconnectTimer->callOnTimeout(this, [this] {
        m_armed = false;
        m_systemStatus = MAV_STATE_UNINIT;
        m_mainMode = 0;
        m_targetMode = PX4_MAIN_MODE_OFFBOARD;
        m_batteryRemaining = 0;
        m_serialPort->close();
        qDebug() << "Heartbeat timeout error. Port closed";
    });
}

void MAVLinkInterface::sendMavlinkMessage(const QByteArray &message) {
    m_serialPort->write(message);
}

void MAVLinkInterface::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_heartbeatTimerId) {
        if (!m_serialPort->isOpen()) {
#ifndef __arm__
            QList<QSerialPortInfo> port_list = QSerialPortInfo::availablePorts();
            for (QSerialPortInfo &serialPortInfo : port_list) {
                qDebug() << "check port:" << serialPortInfo.portName();
                if (serialPortInfo.vendorIdentifier() == 9900 &&
                        (serialPortInfo.productIdentifier() == 18 || // pixracer R15
                         serialPortInfo.productIdentifier() == 17))  // pixhawk 2.4.8
                {
                    qDebug() << "set port...";
                    m_serialPort->setPort(serialPortInfo);
                    qDebug() << "success!";
                    if (m_serialPort->open(QIODevice::ReadWrite)) {
                        //m_serialPort->setDataTerminalReady(false);
                        qDebug() << "connected:" << m_serialPort->portName();
                        break;
                    } else {
                        qDebug() << "connection error";
                    }
                }
            }
#else
            m_serialPort->setPortName("/dev/serial0");
            if (m_serialPort->open(QIODevice::ReadWrite)) {
                qDebug() << "connected:" << m_serialPort->portName();
                m_initializeSystem = true;
            } else {
                qDebug() << "connection error";
            }
#endif
        } else {
            sendHeartbeat();
        }
    } else if (event->timerId() == m_positionTimerId && m_serialPort->isOpen()) {
        /*
        mavlink_set_attitude_target_t m_attitudeTarget = {
            .time_boot_ms = 0,
            .q = { 1.f, 0.f, 0.f, 0.f },
            .body_roll_rate = 0,
            .body_pitch_rate = 0,
            .body_yaw_rate = 0,
            .thrust = 0,
            .target_system = 1,
            .target_component = 1,
            .type_mask = 0b00000111,
            .thrust_body = {0, 0, 0}
        };
        mavlink_msg_set_attitude_target_encode(m_systemId, m_componentId, &msg, &m_attitudeTarget);
        */
        /*
        mavlink_msg_set_attitude_target_pack(
                    m_systemId, m_componentId, &msg,
                    0, 1, 1,
                    0b00000111,
                    NULL,
                    0, 0, 0,
                    m_thrust, NULL
                    );
        */
        /*
        float thrust_body[] = {0, 0, m_thrust};
        mavlink_msg_set_attitude_target_pack(
                    m_systemId, m_componentId, &msg,
                    0, 1, 1,
                    0b10000111,
                    NULL,
                    0, 0, 0, 0, thrust_body
                    );
        */
        /*
        mavlink_msg_set_position_target_local_ned_pack(
                    m_systemId, m_componentId, &msg, 0, 1, 1,
                    MAV_FRAME_LOCAL_NED,
                    0b0000111111000111,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                    );
        */
        // manual control
        switch (m_mainMode) {
        case PX4_MAIN_MODE_STABILIZED: {
            mavlink_message_t msg;
            mavlink_msg_rc_channels_override_pack_chan(
                        m_systemId, m_componentId, MAVLINK_COMM_0, &msg, m_targetSystemId, m_targetComponentId,
                        m_rollPWM, m_pitchPWM, m_throttlePWM, m_yawPWM,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                        );
            sendMavlinkMessage(msg);
        }
            break;
        case PX4_MAIN_MODE_POSCTL:
        case PX4_MAIN_MODE_OFFBOARD: {
            /*
            // odometry
            mavlink_message_t msg_odom;
            mavlink_odometry_t odometry;
            odometry.time_usec = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
            odometry.x = 0;
            odometry.y = 0;
            odometry.z = 0;
            odometry.vx = 0;
            odometry.vy = 0;
            odometry.vz = 0;
            odometry.rollspeed = 0;
            odometry.pitchspeed = 0;
            odometry.yawspeed = 0;
            odometry.frame_id = MAV_FRAME_LOCAL_NED;
            odometry.child_frame_id = MAV_FRAME_LOCAL_NED;
            odometry.reset_counter = 0;
            odometry.estimator_type = MAV_ESTIMATOR_TYPE_VISION;
            odometry.quality = 0;
            mavlink_msg_odometry_encode(m_systemId, m_componentId, &msg_odom, &odometry);
            sendMavlinkMessage(msg_odom);
            */
        }
            break;
        default:
            break;
        }
        // visual pos

        /*
        mavlink_message_t msg_vio;
        mavlink_vision_position_estimate_t pos_vio;
        pos_vio.usec = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
        // здесь x, y ставятся наоборот
        pos_vio.x = m_y;
        pos_vio.y = m_x;
        pos_vio.z = -m_z;
        pos_vio.roll = 0;
        pos_vio.pitch = 0;
        pos_vio.yaw = -m_yaw;
        //pos_vio.covariance[0] = NAN;

        std::memcpy(pos_vio.covariance, m_covariance, sizeof(m_covariance));
        m_covariance[0] = 1e6f; // copter x (camera y)
        m_covariance[1] = 1e6f; // copter y (camera x)
        m_covariance[2] = 1e6f; // z
        m_covariance[5] = 1e6f; // yaw

        pos_vio.reset_counter = 0;
        mavlink_msg_vision_position_estimate_encode(m_systemId, m_componentId, &msg_vio, &pos_vio);
        sendMavlinkMessage(msg_vio);
        */

        mavlink_message_t msg_vio;
        if (m_mainMode == PX4_MAIN_MODE_OFFBOARD && !m_armed) {
            m_visionPos.x = 0;
            m_visionPos.y = 0;
            m_visionPos.z = 0;
            m_visionPos.covariance[0] = 0;
            m_visionPos.covariance[6] = 0;
            m_visionPos.covariance[11] = 0;
            m_visionPos.usec = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
        }
        //m_visionPos.covariance[0] = NAN;
        mavlink_msg_vision_position_estimate_encode(m_systemId, m_componentId, &msg_vio, &m_visionPos);
        sendMavlinkMessage(msg_vio);
        // устанавливаем координаты vision pos равными координатам одометрии (костыль)
        //m_visionPos.x = m_odometry.y;
        //m_visionPos.y = m_odometry.x;
        //m_visionPos.z = m_odometry.z;
        // устанавливаем ковариацию большой до получения новых значений
        m_visionPos.covariance[0] = 1e6f;
        m_visionPos.covariance[6] = 1e6f;
        m_visionPos.covariance[11] = 1e6f;
        //m_visionPos.covariance[20] = 1e6f;

    } else if (event->timerId() == m_targetPosTimerId && m_serialPort->isOpen()) {
        // target pos
        mavlink_message_t msg_ned;
        mavlink_set_position_target_local_ned_t pos_ned;
        pos_ned.target_system = m_targetSystemId;
        pos_ned.target_component = m_targetComponentId;
        pos_ned.time_boot_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
        pos_ned.coordinate_frame = MAV_FRAME_LOCAL_NED;
        pos_ned.type_mask = m_positionTargetTypeMask; // игнорировать скорости и ускорения - 0b0000111111111000;
        // здесь x, y ставятся наоборот
        pos_ned.x = m_targetY;
        pos_ned.y = m_targetX;
        pos_ned.z = -m_targetZ;
        pos_ned.vx = m_targetVX;
        pos_ned.vy = m_targetVY;
        pos_ned.vz = -m_targetVZ;
        pos_ned.afx = 0;
        pos_ned.afy = 0;
        pos_ned.afz = 0;
        pos_ned.yaw = -m_targetYaw;
        pos_ned.yaw_rate = 0;
        mavlink_msg_set_position_target_local_ned_encode(m_systemId, m_componentId, &msg_ned, &pos_ned);
        sendMavlinkMessage(msg_ned);
    }
}

void MAVLinkInterface::sendMavlinkMessage(mavlink_message_t message)
{
    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    uint16_t length = mavlink_msg_to_send_buffer(buffer, &message);
    m_serialPort->write(reinterpret_cast<char*>(&buffer), length);
}

void MAVLinkInterface::setMode(uint8_t mode) {
    m_targetMode = mode;
    mavlink_message_t msg;
    uint32_t sub_mode = PX4_SUB_MODE_AUTO_READY;
    uint32_t custom_mode = (mode << 16) | (sub_mode << 24);
    mavlink_msg_set_mode_pack(m_systemId, m_componentId, &msg, m_targetSystemId, MAV_MODE_FLAG_CUSTOM_MODE_ENABLED, custom_mode);
    sendMavlinkMessage(msg);
}

void MAVLinkInterface::armDisarm(bool arm) {
    mavlink_message_t msg;
    mavlink_msg_command_long_pack(m_systemId, m_componentId, &msg, m_targetSystemId, m_targetComponentId,
                                  MAV_CMD_COMPONENT_ARM_DISARM, 0, arm, 0, 0, 0, 0, 0, 0);
    sendMavlinkMessage(msg);
}

void MAVLinkInterface::takeoff(float z)
{
    mavlink_message_t msg;
    mavlink_msg_command_long_pack(m_systemId, m_componentId, &msg, m_targetSystemId, m_targetComponentId,
                                  MAV_CMD_NAV_TAKEOFF, 0, 0, 0, 0, 0, 0, 0, z); // coords
    sendMavlinkMessage(msg);
}

void MAVLinkInterface::land()
{
    mavlink_message_t msg;
    mavlink_msg_command_long_pack(m_systemId, m_componentId, &msg, m_targetSystemId, m_targetComponentId,
                                  MAV_CMD_NAV_LAND, 0, 0, 0, 0, 0, 0, 0, 0);
    sendMavlinkMessage(msg);
}

void MAVLinkInterface::drop()
{
    mavlink_message_t msg;
    mavlink_msg_command_long_pack(m_systemId, m_componentId, &msg, m_targetSystemId, m_targetComponentId,
                                  MAV_CMD_COMPONENT_ARM_DISARM, 0, 0, 21196, 0, 0, 0, 0, 0);
    sendMavlinkMessage(msg);
}

void MAVLinkInterface::setX(float value) {
    //m_x = value;
    //m_covariance[1] = 0.001; // copter y (camera x)
    m_visionPos.y = value;
    m_visionPos.covariance[6] = 0.00; // copter y (camera x)
}

void MAVLinkInterface::setY(float value) {
    //m_y = value;
    //m_covariance[0] = 0.001; // copter x (camera y)
    m_visionPos.x = value;
    m_visionPos.covariance[0] = 0.00; // copter x (camera y)
}

void MAVLinkInterface::setZ(float value) {
    //m_z = value;
    //m_covariance[2] = 0.001;
    m_visionPos.z = -value;
    m_visionPos.covariance[11] = 0.00;
    /*
     * (    x, x), (    x, y), (    x, z), (    x, roll), (    x, pitch), (    x, yaw)
     * (    y, x), (    y, y), (    y, z), (    y, roll), (    y, pitch), (    y, yaw)
     * (    z, x), (    z, y), (    z, z), (    z, roll), (    z, pitch), (    z, yaw)
     * ( roll, x), ( roll, y), ( roll, z), ( roll, roll), ( roll, pitch), ( roll, yaw)
     * (pitch, x), (pitch, y), (pitch, z), (pitch, roll), (pitch, pitch), (pitch, yaw)
     * (  yaw, x), (  yaw, y), (  yaw, z), (  yaw, roll), (  yaw, pitch), (  yaw, yaw)
     */
}

void MAVLinkInterface::setYaw(float value) {
    //m_yaw = value;
    //m_covariance[5] = 0.001;
    m_visionPos.yaw = -value;
    m_visionPos.covariance[20] = 0.00;
}

void MAVLinkInterface::setPos(float x, float y, float z, float yaw, uint64_t timestamp) {
    //m_x = x; m_y = y; m_z = z; m_yaw = yaw;
    setX(x); setY(y); setZ(z); setYaw(yaw);
    m_visionPos.usec = timestamp;
}

void MAVLinkInterface::setTargetX(float value) {
    if (value < 0) value = 0;
    if (value > 3000) value = 3000;
    m_targetX = value;
}

void MAVLinkInterface::setTargetY(float value) {
    if (value < 0) value = 0;
    if (value > 3000) value = 3000;
    m_targetY = value;
}

void MAVLinkInterface::setTargetZ(float value) { m_targetZ = value; }

void MAVLinkInterface::setTargetYaw(float value) { m_targetYaw = value; }

void MAVLinkInterface::setTargetPos(float x, float y, float z, float yaw) {
    m_targetX = x; m_targetY = y; m_targetZ = z; m_targetYaw = yaw;
}

void MAVLinkInterface::setVelocities(float vx, float vy, float vz) {
    m_targetVX = vx; m_targetVY = vy; m_targetVZ = vz;
}

void MAVLinkInterface::setPositionTargetTypeMask(uint16_t type_mask) {
    m_positionTargetTypeMask = type_mask;
}

void MAVLinkInterface::setThrottlePWM(float value) {
    value = value * 10.f + 1000;
    m_throttlePWM = value < 1000 ? 1000 : value > 2000 ? 2000 : value;
}

void MAVLinkInterface::setRollPWM(float value) {
    value = value * 5.f + 1500;
    m_rollPWM = value < 1000 ? 1000 : value > 2000 ? 2000 : value;
}

void MAVLinkInterface::setPitchPWM(float value) {
    value = value * 5.f + 1500;
    m_pitchPWM = value < 1000 ? 1000 : value > 2000 ? 2000 : value;
}

void MAVLinkInterface::setYawPWM(float value) {
    value = value * 5.f + 1500;
    m_yawPWM = value < 1000 ? 1000 : value > 2000 ? 2000 : value;
}

void MAVLinkInterface::sendHeartbeat()
{
    mavlink_message_t msg;
    mavlink_msg_heartbeat_pack(m_systemId, m_componentId, &msg, MAV_TYPE_ONBOARD_CONTROLLER, MAV_AUTOPILOT_INVALID, MAV_MODE_FLAG_CUSTOM_MODE_ENABLED, 0, MAV_STATE_ACTIVE);
    sendMavlinkMessage(msg);
}
