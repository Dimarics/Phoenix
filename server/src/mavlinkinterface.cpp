#include "mavlinkinterface.h"
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QTimer>

MAVLinkInterface::MAVLinkInterface(QObject *parent) : QObject(parent), m_serialPort(new QSerialPort(this))
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
                    /*
                    qDebug() << "=== PIXHAWK HEARTBEAT RECEIVED ===";
                    qDebug() << "System ID:" << (int)m_message.sysid;
                    qDebug() << "Component ID:" << (int)m_message.compid;
                    //qDebug() << "Autopilot:" << heartbeat.autopilot;
                    qDebug() << "Type:" << heartbeat.type;
                    qDebug() << "Base mode:";
                    qDebug() << "    Custom mode enabled:" << bool(heartbeat.base_mode & MAV_MODE_FLAG_CUSTOM_MODE_ENABLED);
                    qDebug() << "    Armed:" << bool(heartbeat.base_mode & MAV_MODE_FLAG_SAFETY_ARMED);

                    qDebug() << "Custom mode:" << heartbeat.custom_mode;
                    qDebug() << "State:" << heartbeat.system_status;
                    */
                }
                    break;
                case MAVLINK_MSG_ID_COMMAND_ACK: {
                    mavlink_command_ack_t ack;
                    mavlink_msg_command_ack_decode(&m_message, &ack);
                    qDebug() << "Command:" << ack.command;
                    qDebug() << "Result:" << ack.result;
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
    startTimer(1000);

    /*QTimer::singleShot(5000, this, [this] {
        mavlink_message_t msg;

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

        //mavlink_msg_set_mode_pack(255, 0, &msg, 1, base_mode, PX4_MODE_POSCTL);
        //qDebug() << base_mode;

        //mavlink_msg_set_mode_pack(255, 0, &msg, 1, MAV_MODE_FLAG_CUSTOM_MODE_ENABLED, PX4_MODE_OFFBOARD);
    });*/
}

void MAVLinkInterface::sendMavlinkMessage(const QByteArray &message) {
    m_serialPort->write(message);
}

void MAVLinkInterface::timerEvent(QTimerEvent *)
{
    if (!m_serialPort->isOpen()) {
        QList<QSerialPortInfo> port_list = QSerialPortInfo::availablePorts();
        if (!port_list.isEmpty()) {
            m_serialPort->setPort(port_list.at(0));
            if (m_serialPort->open(QIODevice::ReadWrite)) {
                m_serialPort->setDataTerminalReady(false);
                qDebug() << "connected:" << m_serialPort->portName();
            } else {
                qDebug() << "connection error";
            }
        }
    } else {
        mavlink_message_t msg;
        mavlink_msg_heartbeat_pack(255, 0, &msg, MAV_TYPE_ONBOARD_CONTROLLER, MAV_AUTOPILOT_INVALID, 0, 0, MAV_STATE_ACTIVE);
        sendMavlinkMessage(msg);
    }
}

void MAVLinkInterface::sendMavlinkMessage(mavlink_message_t message)
{
    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    uint16_t length = mavlink_msg_to_send_buffer(buffer, &message);
    m_serialPort->write(reinterpret_cast<char*>(&buffer), length);
}
