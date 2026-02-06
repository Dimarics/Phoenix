#include "vehiclecontroller.h"
#include <QTimer>

VehicleController::VehicleController(QObject *parent) : QObject(parent),
    m_movingToPoint(false),
    m_losslessTime(0),
    m_state(ON_GROUND),
    m_mavlinkInterface(new MAVLinkInterface(this)),
    m_videoCapture(new VideoCapture(this)),
    m_arucoDetector(new ArucoDetector(this)),
    m_server(new Server(this))
{
    uint16_t type_mask = 0;
    type_mask |= POSITION_TARGET_TYPEMASK_AX_IGNORE;
    type_mask |= POSITION_TARGET_TYPEMASK_AY_IGNORE;
    type_mask |= POSITION_TARGET_TYPEMASK_AZ_IGNORE;
    m_mavlinkInterface->setPositionTargetTypeMask(type_mask);
    // video capture
    connect(m_videoCapture, &VideoCapture::frameChanged, this, [this](const cv::Mat &f) {
        cv::Mat frame(f);
        bool marker_on_vision = m_arucoDetector->arucoIds().size();
        m_arucoDetector->detectMarkers(frame);
        bool marker_was_detected = !marker_on_vision && m_arucoDetector->arucoIds().size();
        //bool marker_was_lost = marker_on_vision && !m_arucoDetector->arucoIds().size();
        if (m_arucoDetector->arucoIds().size()) {
            //
            m_losslessTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
            //
            //m_mavlinkInterface->setVelocities(0, 0, 0);
            m_mavlinkInterface->setPos(m_arucoDetector->x(), m_arucoDetector->y(), m_arucoDetector->z(), m_arucoDetector->yaw());
        } else {
            //
            /*
            uint16_t type_mask = 0;
            type_mask |= POSITION_TARGET_TYPEMASK_X_IGNORE;
            type_mask |= POSITION_TARGET_TYPEMASK_Y_IGNORE;
            type_mask |= POSITION_TARGET_TYPEMASK_Z_IGNORE;
            type_mask |= POSITION_TARGET_TYPEMASK_AX_IGNORE;
            type_mask |= POSITION_TARGET_TYPEMASK_AY_IGNORE;
            type_mask |= POSITION_TARGET_TYPEMASK_AZ_IGNORE;
            type_mask |= POSITION_TARGET_TYPEMASK_YAW_IGNORE;
            m_mavlinkInterface->setPositionTargetTypeMask(type_mask);
            */
            //
            m_mavlinkInterface->setPos(0, 0, 0, 0);
            if (m_state == IN_AIR && QDateTime::currentDateTime().toMSecsSinceEpoch() - m_losslessTime >= 300) {
                land();
                qDebug() << "markers lost";
            }
        }
        if (marker_was_detected) {
            if (m_state == TAKEOFF) {
                m_state = IN_AIR;
                uint16_t type_mask = 0;
                type_mask |= POSITION_TARGET_TYPEMASK_AX_IGNORE;
                type_mask |= POSITION_TARGET_TYPEMASK_AY_IGNORE;
                type_mask |= POSITION_TARGET_TYPEMASK_AZ_IGNORE;
                m_mavlinkInterface->setPositionTargetTypeMask(type_mask);
                m_mavlinkInterface->setVelocities(0, 0, 0);
                m_mavlinkInterface->setTargetX(m_mavlinkInterface->x());
                m_mavlinkInterface->setTargetY(m_mavlinkInterface->y());
                //m_mavlinkInterface->setTargetZ(m_takeoffTarget);
                m_mavlinkInterface->setTargetYaw(m_mavlinkInterface->yaw());
                //qDebug() << "detect markers coords";
                //qDebug() << "  x:" << m_mavlinkInterface->x();
                //qDebug() << "  y:" << m_mavlinkInterface->y();
                //qDebug() << "yaw:" << m_mavlinkInterface->yaw();
            }
        }
        //qDebug() << "distance:" << m_mavlinkInterface->distanceToPoint();
        /*
        switch (m_state) {
        case TAKEOFF:
            if (checkPoint()) {
                m_state = IN_AIR;
            }
            break;
        case IN_AIR:
            break;
        default:
            break;
        }
        */
        m_arucoDetector->drawDetectMarkers(frame);
        m_server->sendVideoFrame(m_videoCapture->matToJpeg(frame));
    });

    // aruco detector

    // server
    QTimer *pingTimer = new QTimer(this);
    pingTimer->setInterval(200);
    connect(pingTimer, &QTimer::timeout, this, [this] {
        bool button_state = false;
        double sonar_distance = 0;

        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);
        stream << m_mavlinkInterface->systemStatus();
        stream << m_mavlinkInterface->batteryRemaining();
        stream << m_mavlinkInterface->mode();
        stream << m_mavlinkInterface->armed();
        stream << m_mavlinkInterface->x();
        stream << m_mavlinkInterface->y();
        stream << m_mavlinkInterface->z();
        stream << m_mavlinkInterface->roll();
        stream << m_mavlinkInterface->pitch();
        stream << m_mavlinkInterface->yaw();
        stream << m_arucoDetector->arucoIds();
        stream << button_state;
        stream << sonar_distance;
        m_server->sendDataToClients(data);
    });
    pingTimer->start();
    connect(m_server, &Server::messageReceived, this, [this](const QByteArray &msg) {
        quint8 msg_id;
        QDataStream msg_data(msg);
        msg_data >> msg_id;
        switch(msg_id) {
        case CLIENT_MSG_SET_MODE: {
            uint8_t mode; msg_data >> mode; m_mavlinkInterface->setMode(mode);
            qDebug() << "setMode:" << mode;
        }   break;
        case CLIENT_MSG_ARM_DISARM: {
            bool arm; msg_data >> arm; m_mavlinkInterface->armDisarm(arm);
            qDebug() << "arm:" << arm;
        }   break;
        case CLIENT_MSG_TAKEOFF: {
            float z; msg_data >> z; takeoff(z);
            qDebug() << "takeoff:" << z;
        }   break;
        case CLIENT_MSG_LAND:
            land();
            qDebug() << "land";
            break;
        case CLIENT_MSG_DROP:
            m_mavlinkInterface->drop();
            qDebug() << "drop";
            break;
        case CLIENT_MSG_SET_THROTTLE_PWM: {
            float throttle; msg_data >> throttle; m_mavlinkInterface->setThrottlePWM(throttle);
            qDebug() << "set throttle pwm:" << throttle;
        }   break;
        case CLIENT_MSG_SET_ROLL_PWM: {
            float roll; msg_data >> roll; m_mavlinkInterface->setRollPWM(roll);
            qDebug() << "set roll pwm:" << roll;
        }   break;
        case CLIENT_MSG_SET_PITCH_PWM: {
            float pitch; msg_data >> pitch; m_mavlinkInterface->setPitchPWM(pitch);
            qDebug() << "set pitch pwm:" << pitch;
        }   break;
        case CLIENT_MSG_ALIGN:
            m_mavlinkInterface->align();
            qDebug() << "align";
            break;
        case CLIENT_MSG_SET_X: {
            float x; msg_data >> x; m_mavlinkInterface->setTargetX(x);
            qDebug() << "set x:" << x;
        }   break;
        case CLIENT_MSG_SET_Y: {
            float y; msg_data >> y; m_mavlinkInterface->setTargetY(y);
            qDebug() << "set y:" << y;
        }   break;
        case CLIENT_MSG_SET_Z: {
            float z; msg_data >> z; m_mavlinkInterface->setTargetZ(z);
            qDebug() << "set z:" << z;
        }   break;
        case CLIENT_MSG_SET_YAW: {
            float yaw; msg_data >> yaw; m_mavlinkInterface->setTargetYaw(yaw);
            qDebug() << "set yaw:" << yaw;
        }   break;
        }
    });
    /*
    connect(m_server, &Server::setMode, m_mavlinkInterface, &MAVLinkInterface::setMode);
    connect(m_server, &Server::armDisarm, m_mavlinkInterface, &MAVLinkInterface::armDisarm);
    connect(m_server, &Server::takeoff, m_mavlinkInterface, &MAVLinkInterface::takeoff);
    connect(m_server, &Server::land, m_mavlinkInterface, &MAVLinkInterface::land);
    connect(m_server, &Server::drop, m_mavlinkInterface, &MAVLinkInterface::drop);
    connect(m_server, &Server::setThrottlePWM, m_mavlinkInterface, &MAVLinkInterface::setThrottlePWM);
    connect(m_server, &Server::setRollPWM, m_mavlinkInterface, &MAVLinkInterface::setRollPWM);
    connect(m_server, &Server::setPitchPWM, m_mavlinkInterface, &MAVLinkInterface::setPitchPWM);
    connect(m_server, &Server::setYawPWM, m_mavlinkInterface, &MAVLinkInterface::setYawPWM);
    connect(m_server, &Server::align, m_mavlinkInterface, &MAVLinkInterface::align);
    connect(m_server, &Server::setTargetX, m_mavlinkInterface, &MAVLinkInterface::setTargetX);
    connect(m_server, &Server::setTargetY, m_mavlinkInterface, &MAVLinkInterface::setTargetY);
    connect(m_server, &Server::setTargetZ, m_mavlinkInterface, &MAVLinkInterface::setTargetZ);
    connect(m_server, &Server::setTargetYaw, m_mavlinkInterface, &MAVLinkInterface::setTargetYaw);
    */
    QTimer *moveControlTimer = new QTimer(this);
    moveControlTimer->setInterval(20);
    moveControlTimer->callOnTimeout(this, [this] {
        switch (m_state) {
        case TAKEOFF:
            if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count() - m_takeoffStartTime > 8000) {
                land();
            }
            break;
        case LANDING:
            //if (m_mavlinkInterface->z() == 0) {
            //    m_mavlinkInterface->setVelocities(0, 0, -2);
            //}
            if (m_mavlinkInterface->z() != 0 || qAbs(m_mavlinkInterface->vz()) > 0.01) {
                m_restTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
            }
            /*
            if (m_mavlinkInterface->z() == 0 &&
                std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count() - m_restTime > 1000) {
                m_mavlinkInterface->setVelocities(0, 0, -2);
            }
            */
            if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count() - m_restTime > 4000) {
                m_state = ON_GROUND;
                m_mavlinkInterface->drop();
            }
            break;
        default:
            break;
        }
    });
    moveControlTimer->start();
}
void VehicleController::takeoff(float z)
{
    uint16_t type_mask = 0;
    type_mask |= POSITION_TARGET_TYPEMASK_X_IGNORE;
    type_mask |= POSITION_TARGET_TYPEMASK_Y_IGNORE;
    type_mask |= POSITION_TARGET_TYPEMASK_Z_IGNORE;
    type_mask |= POSITION_TARGET_TYPEMASK_YAW_IGNORE;
    //type_mask |= POSITION_TARGET_TYPEMASK_VX_IGNORE;
    //type_mask |= POSITION_TARGET_TYPEMASK_VY_IGNORE;
    type_mask |= POSITION_TARGET_TYPEMASK_AX_IGNORE;
    type_mask |= POSITION_TARGET_TYPEMASK_AY_IGNORE;
    type_mask |= POSITION_TARGET_TYPEMASK_AZ_IGNORE;
    type_mask |= POSITION_TARGET_TYPEMASK_YAW_RATE_IGNORE;
    m_mavlinkInterface->setPositionTargetTypeMask(type_mask);

    m_state = TAKEOFF;
    //m_mavlinkInterface->setPos(0, 0, 0, 0);
    m_takeoffStartTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    m_mavlinkInterface->setTargetPos(0, 0, z, 0);
    m_mavlinkInterface->setVelocities(0, 0, 1.5);
    //m_takeoffTarget = z;
    if (!m_mavlinkInterface->armed())
        m_mavlinkInterface->armDisarm(true);
}

void VehicleController::land()
{
    uint16_t type_mask = 0;
    type_mask |= POSITION_TARGET_TYPEMASK_X_IGNORE;
    type_mask |= POSITION_TARGET_TYPEMASK_Y_IGNORE;
    type_mask |= POSITION_TARGET_TYPEMASK_Z_IGNORE;
    type_mask |= POSITION_TARGET_TYPEMASK_YAW_IGNORE;
    //type_mask |= POSITION_TARGET_TYPEMASK_VX_IGNORE;
    //type_mask |= POSITION_TARGET_TYPEMASK_VY_IGNORE;
    type_mask |= POSITION_TARGET_TYPEMASK_AX_IGNORE;
    type_mask |= POSITION_TARGET_TYPEMASK_AY_IGNORE;
    type_mask |= POSITION_TARGET_TYPEMASK_AZ_IGNORE;
    type_mask |= POSITION_TARGET_TYPEMASK_YAW_RATE_IGNORE;
    m_mavlinkInterface->setPositionTargetTypeMask(type_mask);

    m_state = LANDING;
    m_restTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    m_mavlinkInterface->setTargetPos(0, 0, 0, 0);
    //m_mavlinkInterface->land();
    m_mavlinkInterface->setVelocities(0, 0, -0.35);
}

void VehicleController::setPoint(float x, float y, float z, float yaw)
{
    m_mavlinkInterface->setTargetPos(x, y, z, yaw);
}

bool VehicleController::checkPoint()
{
    return m_mavlinkInterface->distanceToPoint() <= 0.30;
}
