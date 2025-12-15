#include "mavlinkinterface.h"
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QTimerEvent>
#include <QTimer>

MAVLinkInterface *mavlinkInterface = nullptr;

MAVLinkInterface::MAVLinkInterface(QObject *parent) : QObject(parent),
    m_systemStatus(MAV_STATE_UNINIT),
    m_serialPort(new QSerialPort(this))
{
    m_serialPort->setBaudRate(57600);
    connect(m_serialPort, &QSerialPort::readyRead, this, [this]() {
        QByteArray data = m_serialPort->readAll();
        for (uint8_t byte : data) {
            if (mavlink_parse_char(MAVLINK_COMM_0, byte, &m_message, &m_status)) {
                //if (m_message.msgid != MAVLINK_MSG_ID_HEARTBEAT) qDebug() << m_message.msgid;
                switch (m_message.msgid) {
                case MAVLINK_MSG_ID_HEARTBEAT: {
                    mavlink_heartbeat_t heartbeat;
                    mavlink_msg_heartbeat_decode(&m_message, &heartbeat);

                    m_systemStatus = heartbeat.system_status;
                    //uint8_t main_mode = (heartbeat.custom_mode >> 16) & 0xFF;
                    //uint8_t sub_mode = (heartbeat.custom_mode >> 24) & 0xFF;

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

                }
                    break;
                case MAVLINK_MSG_ID_COMMAND_ACK: {
                    mavlink_command_ack_t ack;
                    mavlink_msg_command_ack_decode(&m_message, &ack);
                    qDebug() << "=== PIXHAWK COMMAND ASK RECEIVED ===";
                    qDebug() << "Command:" << ack.command;
                    qDebug() << "Result:" << ack.result;
                    switch (ack.command) {
                    case MAV_CMD_COMPONENT_ARM_DISARM:
                        if (ack.result == 0) m_systemStatus = MAV_STATE_ACTIVE;
                        break;
                    default:
                        break;
                    }
                }
                    break;
                default:
                    break;
                }
            }
        }
    });
    connect(m_serialPort, &QSerialPort::errorOccurred, this, [this](QSerialPort::SerialPortError error) {
        switch (error) {
        case QSerialPort::DeviceNotFoundError:
        case QSerialPort::PermissionError:
        case QSerialPort::ResourceError:
            m_serialPort->close();
            break;
        default: break;
        }
        //qDebug() << error;
    });
    m_heartbeatTimerId = startTimer(1000);
    m_attitudeTimerId = startTimer(20);

    QTimer::singleShot(5000, this, [this] {
        mavlink_message_t msg;

        /*
        // Определяем base_mode

        bool armed = false;
        uint8_t base_mode =
                MAV_MODE_FLAG_CUSTOM_MODE_ENABLED |
                MAV_MODE_FLAG_STABILIZE_ENABLED |
                MAV_MODE_FLAG_GUIDED_ENABLED |
                MAV_MODE_FLAG_MANUAL_INPUT_ENABLED;
        if (armed) {
            base_mode |= MAV_MODE_FLAG_SAFETY_ARMED;
        }
        */

        uint32_t main_mode = PX4_CUSTOM_MAIN_MODE_STABILIZED;
        uint32_t sub_mode = PX4_CUSTOM_SUB_MODE_AUTO_READY;
        uint32_t custom_mode = (main_mode << 16) | (sub_mode << 24);

        mavlink_msg_set_mode_pack(255, 0, &msg, 1, MAV_MODE_FLAG_CUSTOM_MODE_ENABLED, custom_mode);
        //sendMavlinkMessage(msg);
    });
}

void MAVLinkInterface::sendMavlinkMessage(const QByteArray &message) {
    m_serialPort->write(message);
}

void MAVLinkInterface::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_heartbeatTimerId) {
        if (!m_serialPort->isOpen()) {
            QList<QSerialPortInfo> port_list = QSerialPortInfo::availablePorts();
            for (QSerialPortInfo &serialPortInfo : port_list) {
                if (serialPortInfo.vendorIdentifier() == 9900 &&
                        (serialPortInfo.productIdentifier() == 18 || // pixracer R15
                         serialPortInfo.productIdentifier() == 17))  // pixhawk 2.4.8
                {
                    m_serialPort->setPort(serialPortInfo);
                    if (m_serialPort->open(QIODevice::ReadWrite)) {
                        //m_serialPort->setDataTerminalReady(false);
                        qDebug() << "connected:" << m_serialPort->portName();
                        break;
                    } else {
                        qDebug() << "connection error";
                    }
                }
            }
        } else {
            mavlink_message_t msg;
            mavlink_msg_heartbeat_pack(255, 0, &msg, MAV_TYPE_ONBOARD_CONTROLLER, MAV_AUTOPILOT_INVALID, MAV_MODE_FLAG_CUSTOM_MODE_ENABLED, 0, MAV_STATE_ACTIVE);
            sendMavlinkMessage(msg);
        }
    } else if (event->timerId() == m_attitudeTimerId && m_serialPort->isOpen()) {
        mavlink_message_t msg;

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
        mavlink_msg_set_attitude_target_encode(255, 0, &msg, &m_attitudeTarget);
        */
        /*
        mavlink_manual_control_t manual_control;
        manual_control.target = 1;
        manual_control.x = 0;
        manual_control.y = 0;
        manual_control.z = 0;
        manual_control.r = 1000 + m_thrust * 1000;
        manual_control.buttons = 0;
        mavlink_msg_manual_control_encode(255, 0, &msg, &manual_control);
        */
        /// new
        /*
        mavlink_msg_rc_channels_override_pack_chan(
                    255, 0, MAVLINK_COMM_0, &msg, 1, 1,
                    0, 0, 1000 + m_thrust * 1000, 0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0
                    );
        */
        /*
        mavlink_msg_set_attitude_target_pack(
                    255, 0, &msg,
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
                    255, 0, &msg,
                    0, 1, 1,
                    0b10000111,
                    NULL,
                    0, 0, 0, 0, thrust_body
                    );
        */
        /*
        mavlink_msg_set_position_target_local_ned_pack(
                    255, 0, &msg, 0, 1, 1,
                    MAV_FRAME_LOCAL_NED,
                    0b0000111111000111,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                    );
        */

        // manual control

        //m_systemStatus = MAV_STATE_ACTIVE
        mavlink_msg_rc_channels_override_pack_chan(
                    255, 0, MAVLINK_COMM_0, &msg, 1, 1,
                    m_roll, m_pitch, m_throttle, m_yaw,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                    );

        //mavlink_msg_manual_control_pack_chan(255, 0, MAVLINK_COMM_0, &msg, 1,
        //                                     0, 0, -m_thrust, 0,
        //                                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

        sendMavlinkMessage(msg);
    }
}

void MAVLinkInterface::sendMavlinkMessage(mavlink_message_t message)
{
    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    uint16_t length = mavlink_msg_to_send_buffer(buffer, &message);
    m_serialPort->write(reinterpret_cast<char*>(&buffer), length);
}

void MAVLinkInterface::arm() {
    mavlink_message_t msg;
    bool arm = true;
    mavlink_msg_command_long_pack(255, 0, &msg, 1, 1, MAV_CMD_COMPONENT_ARM_DISARM, 0, arm, 0, 0, 0, 0, 0, 0);
    sendMavlinkMessage(msg);
}
