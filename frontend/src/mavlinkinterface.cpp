#include "mavlinkinterface.h"
#include <QHostAddress>

MAVLinkInterface::MAVLinkInterface() : m_networkManager(new QNetworkAccessManager(this))
{
    m_networkManager->connectToHost(QHostAddress(QHostAddress::Any).toString(), 80);
}

void MAVLinkInterface::arm()
{
    mavlink_message_t msg;
    bool arm = true;
    mavlink_msg_command_long_pack(255, 0, &msg, 1, 1, MAV_CMD_COMPONENT_ARM_DISARM, 0, arm, 0, 0, 0, 0, 0, 0);
    sendMavlinkMessage(msg);
}

void MAVLinkInterface::sendMavlinkMessage(mavlink_message_t message)
{
    QByteArray buffer(MAVLINK_MAX_PACKET_LEN, 0);
    uint16_t length = mavlink_msg_to_send_buffer(reinterpret_cast<uint8_t*>(buffer.data()), &message);
    buffer.resize(length);
    QNetworkRequest request(QUrl("device"));
    m_networkManager->post(request, buffer);
}
