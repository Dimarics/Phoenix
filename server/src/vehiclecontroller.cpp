#include "vehiclecontroller.h"
#include <QTimer>
#include <QFile>
#include <QElapsedTimer>
#include <QCoreApplication>

#ifdef __arm__
#include "ws2811.h"
//#include <pigpio.h>

ws2811_t ledstring = {};
#endif

struct GradientPoint {
    GradientPoint(float position, uint8_t red, uint8_t green, uint8_t blue) :
        pos(position), r(red), g(green), b(blue) { }
    float pos;
    uint8_t r;
    uint8_t g;
    uint8_t b;
};


uint32_t colorInPos(float pos) {
    int size = 3;
    GradientPoint points[] {
        GradientPoint(  0, 255,   0,   0),
        GradientPoint(0.5,   0,   0, 255),
        GradientPoint(1.0,   0, 255,   0),
    };
    if (pos < 0) {
        return uint32_t(points[0].g) << 16 | uint16_t(points[0].r) << 8 | points[0].b;
    } else if (pos > 1) {
        return uint32_t(points[size-1].g) << 16 | uint16_t(points[size-1].r) << 8 | points[size-1].b;
    } else {
        for (int i = 0; i < size - 1; ++i) {
            GradientPoint point_a = points[i];
            GradientPoint point_b = points[i+1];
            if (pos < point_a.pos || pos > point_b.pos) continue;
            float k = (pos - point_a.pos) / (point_b.pos - point_a.pos);
            float r = point_a.r + float(point_b.r - point_a.r) * k;
            float g = point_a.g + float(point_b.g - point_a.g) * k;
            float b = point_a.b + float(point_b.b - point_a.b) * k;
            if (r < 0) r = 0;
            else if (r > 255) r = 255;
            if (g < 0) g = 0;
            else if (g > 255) g = 255;
            if (b < 0) b = 0;
            else if (b > 255) b = 255;
            return uint32_t(g) << 16 | uint16_t(r) << 8 | uint8_t(b);
        }
    }
    return 0;
}

VehicleController::VehicleController(QObject *parent) : QObject(parent),
    m_movingToPoint(false),
    m_losslessTime(0),
    m_state(ON_GROUND),
    m_RGBLed(27, 17, 22),
    m_mavlinkInterface(new MAVLinkInterface(this)),
    m_videoCapture(new VideoCapture(this)),
    m_arucoDetector(new ArucoDetector(this)),
    m_server(new Server(this)),
    m_scriptWorker(new ScriptWorker(this))
{
    /*
    const int pin = 18;
    gpioSetMode(pin, PI_OUTPUT);
    //show
    gpioWaveClear();
    gpioWaveAddNew();
    for (int i = 0; i < 30; ++i) {
        quint8 r = 255, g = 0, b = 255;
        uint32_t color = (g << 16) | (r << 8) | b;
        // send pixel
        for (int bit = 23; bit >= 0; --bit) {
            gpioPulse_t pulse[2];

            // Упрощенные тайминги: все импульсы по 1 микросекунде
            // Но меняем порядок HIGH/LOW для кодирования битов
            pulse[0].gpioOn = (1 << pin);
            pulse[0].gpioOff = 0;
            pulse[0].usDelay = 1;

            pulse[1].gpioOn = 0;
            pulse[1].gpioOff = (1 << pin);
            pulse[1].usDelay = 1;

            // Для бита 1 оставляем как есть (HIGH, LOW)
            // Для бита 0 меняем порядок на (LOW, HIGH)
            if (!(color & (1 << bit))) {
                // Меняем местами для бита 0
                pulse[0].gpioOn = 0;
                pulse[0].gpioOff = (1 << pin);
                pulse[1].gpioOn = (1 << pin);
                pulse[1].gpioOff = 0;
            }
            gpioWaveAddGeneric(2, pulse);
        }
        //end send pixel
    }
    gpioPulse_t reset_pulse;
    reset_pulse.gpioOn = 0;
    reset_pulse.gpioOff = (1 << pin);
    reset_pulse.usDelay = 50;
    gpioWaveAddGeneric(1, &reset_pulse);

    // создаём волну
    int wave_id = gpioWaveCreate();
    if (wave_id >= 0) {
        gpioWaveTxSend(wave_id, PI_WAVE_MODE_ONE_SHOT);
        while(gpioWaveTxBusy()) {
            usleep(100);
        }
        gpioWaveDelete(wave_id);
    }
    // end show
*/

#ifdef __arm__
    ledstring.freq = WS2811_TARGET_FREQ;
    ledstring.dmanum = 10;
    ledstring.channel[0].gpionum = 18;
    ledstring.channel[0].count = 90;
    ledstring.channel[0].brightness = 128;
    ws2811_return_t ws_init_state = ws2811_init(&ledstring);
    if (ws_init_state != WS2811_SUCCESS) {
        qDebug() << "error" << ws_init_state;
    } else {
        qDebug() << "ledstrip init success";
        /*
        ws2811_led_t color = 0x00FF00FF;
        for (int i = 0; i < ledstring.channel[0].count; ++i) {
            ledstring.channel[0].leds[i] = color;
        }
        ws2811_render(&ledstring);
        */
    }
#endif

    // Дальномер
    m_dalnomer.setBus(&m_i2c);
    m_dalnomer.setTimeout(100);
    m_dalnomer.init();
    qDebug() << "dalnomer init:" << m_dalnomer.isValid();

    QTimer *tm = new QTimer(this);
    tm->setInterval(50);
    connect(tm, &QTimer::timeout, this, [this] {
        QElapsedTimer elapsed_timer;
        elapsed_timer.start();
        if (m_dalnomer.isValid()) {
            m_dalnomer.poll();
            float distance = m_dalnomer.distance();
            //qDebug() << "z:" << distance;
            if (distance <= 1.2) {
                m_mavlinkInterface->setZ(distance);
                m_mavlinkInterface->setTimestamp(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
            }
        }
        //qDebug() << "time:" << elapsed_timer.elapsed();
        m_i2c.writeByte(0x08, I2C_REQUEST_GET_SONARS_DATA);
        uint8_t data[8] {};
        m_i2c.readBytes(0x08, data, 8);
        float front_sonar_distance = float(uint16_t(data[0] << 8) | data[1]) / 1000.f;
        float back_sonar_distance = float(uint16_t(data[2] << 8) | data[3]) / 1000.f;
        float left_sonar_distance = float(uint16_t(data[4] << 8) | data[5]) / 1000.f;
        float right_sonar_distance = float(uint16_t(data[6] << 8) | data[7]) / 1000.f;
        if (m_state == IN_AIR) {
            float limit = 0.3;
            float k = 2;
            if (front_sonar_distance <= limit) {
                m_mavlinkInterface->setTargetY(m_mavlinkInterface->odometry().y - (limit - front_sonar_distance) * k);
            }
            if (back_sonar_distance <= limit) {
                m_mavlinkInterface->setTargetY(m_mavlinkInterface->odometry().y + (limit - back_sonar_distance) * k);
            }
            if (left_sonar_distance <= limit) {
                m_mavlinkInterface->setTargetX(m_mavlinkInterface->x() + (limit - left_sonar_distance) * k);
            }
            if (right_sonar_distance <= limit) {
                m_mavlinkInterface->setTargetX(m_mavlinkInterface->x() - (limit - right_sonar_distance) * k);
            }
        }
        /*
        qDebug() << "=========================";
        qDebug() << "front sonar distance:" << front_sonar_distance;
        qDebug() << "back sonar distance:" << back_sonar_distance;
        qDebug() << "left sonar distance:" << left_sonar_distance;
        qDebug() << "right sonar distance:" << right_sonar_distance;
        */
#ifdef __arm__
        float pos = (m_mavlinkInterface->odometry().z - 0.1) / 1.5;
        uint32_t color = colorInPos(pos);
        for (int i = 0; i < ledstring.channel[0].count; ++i) {
            ledstring.channel[0].leds[i] = color;
        }
        ws2811_render(&ledstring);
#endif
    });
    tm->start();

    uint16_t type_mask = 0;
    type_mask |= POSITION_TARGET_TYPEMASK_AX_IGNORE;
    type_mask |= POSITION_TARGET_TYPEMASK_AY_IGNORE;
    type_mask |= POSITION_TARGET_TYPEMASK_AZ_IGNORE;
    m_mavlinkInterface->setPositionTargetTypeMask(type_mask);
    // video capture
    connect(m_videoCapture, &VideoCapture::frameChanged, this, [this](cv::Mat frame, uint64_t timestamp) {
        //QElapsedTimer shared_timer;
        //shared_timer.start();
        bool markers_in_view = m_arucoDetector->markersInView();
        //qDebug() << timestamp / 1000;

        //QElapsedTimer detected_timer;
        //detected_timer.start();
        m_arucoDetector->detectMarkers(frame);
        //qDebug() << "время обработки изображения:" << detected_timer.elapsed() << "мс";
        //qDebug() << "маркеров:" << m_arucoDetector->arucoIds().size();

        bool marker_was_detected = !markers_in_view && m_arucoDetector->markersInView();
        //bool marker_was_lost = marker_on_vision && !m_arucoDetector->arucoIds().size();
        if (m_arucoDetector->arucoIds().size()) {
            m_RGBLed.setColor(0, 255, 0);
            //
            m_losslessTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
            //
            //m_mavlinkInterface->setVelocities(0, 0, 0);
            m_mavlinkInterface->setX(m_arucoDetector->x());
            m_mavlinkInterface->setY(m_arucoDetector->y());
            if (!m_dalnomer.isValid() || m_dalnomer.distance() > 1.2) {
                m_mavlinkInterface->setZ(m_arucoDetector->z());
            }
            m_mavlinkInterface->setYaw(m_arucoDetector->yaw());
            m_mavlinkInterface->setTimestamp(timestamp);
        } else {
            m_RGBLed.setColor(0, 0, 255);
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
            //m_mavlinkInterface->setPos(0, 0, 0, 0);
            if (m_state == IN_AIR && std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count() - m_losslessTime >= 200) {
                //land();
                //qDebug() << "markers lost";
            }
        }
        if (marker_was_detected) {
            if (m_state == ORIENTATION) {
                qDebug() << "markers detected";
                m_state = IN_AIR;
                uint16_t type_mask = 0;
                type_mask |= POSITION_TARGET_TYPEMASK_AX_IGNORE;
                type_mask |= POSITION_TARGET_TYPEMASK_AY_IGNORE;
                type_mask |= POSITION_TARGET_TYPEMASK_AZ_IGNORE;
                m_mavlinkInterface->setPositionTargetTypeMask(type_mask);
                m_mavlinkInterface->setVelocities(0, 0, 0);

                m_mavlinkInterface->resetVisionPos();
                m_mavlinkInterface->setTargetX(m_arucoDetector->x());
                m_mavlinkInterface->setTargetY(m_arucoDetector->y());
                //m_mavlinkInterface->setTargetX(1.5);
                //m_mavlinkInterface->setTargetY(1.5);

                //m_mavlinkInterface->setTargetZ(m_takeoffTarget);

                m_mavlinkInterface->setTargetYaw(m_mavlinkInterface->yaw());
                //m_mavlinkInterface->setTargetYaw(0);
                qDebug() << "new target - x:" << m_arucoDetector->x() << "y:" << m_arucoDetector->y();
            }
        }
        /*
        m_arucoDetector->drawDetectMarkers(frame);

        QElapsedTimer send_frame_timer;
        send_frame_timer.start();
        emit m_server->sendVideoFrame(m_videoCapture->matToJpeg(frame)); //----------------------
        qDebug() << "время отправки изображения:" << send_frame_timer.elapsed() << "мс";
        qDebug() << "время работы с кадром:" << shared_timer.elapsed() << "мс";
        */
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
        stream << m_mavlinkInterface->odometry().x;
        stream << m_mavlinkInterface->odometry().y;
        stream << m_mavlinkInterface->odometry().z;
        stream << m_mavlinkInterface->roll();
        stream << m_mavlinkInterface->pitch();
        stream << m_mavlinkInterface->yaw();
        stream << m_arucoDetector->arucoIds();
        stream << button_state;
        stream << sonar_distance;
        emit m_server->sendDataToClients(data);
    });
    pingTimer->start();
    connect(m_server, &Server::messageReceived, this, [this](const QByteArray &msg) {
        quint8 msg_id;
        QDataStream msg_data(msg);
        msg_data >> msg_id;
        switch(msg_id) {
        case MSG_CLIENT_SET_MODE: {
            uint8_t mode; msg_data >> mode; m_mavlinkInterface->setMode(mode);
            qDebug() << "setMode:" << mode;
        }   break;
        case MSG_CLIENT_ARM_DISARM: {
            bool arm; msg_data >> arm; m_mavlinkInterface->armDisarm(arm);
            qDebug() << "arm:" << arm;
        }   break;
        case MSG_CLIENT_TAKEOFF: {
            float z; msg_data >> z; takeoff(z);
            qDebug() << "takeoff:" << z;
        }   break;
        case MSG_CLIENT_LAND:
            land();
            qDebug() << "land";
            break;
        case MSG_CLIENT_DROP:
            drop();
            qDebug() << "drop";
            break;
        case MSG_CLIENT_SET_THROTTLE_PWM: {
            float throttle; msg_data >> throttle; m_mavlinkInterface->setThrottlePWM(throttle);
            qDebug() << "set throttle pwm:" << throttle;
        }   break;
        case MSG_CLIENT_SET_ROLL_PWM: {
            float roll; msg_data >> roll; m_mavlinkInterface->setRollPWM(roll);
            qDebug() << "set roll pwm:" << roll;
        }   break;
        case MSG_CLIENT_SET_PITCH_PWM: {
            float pitch; msg_data >> pitch; m_mavlinkInterface->setPitchPWM(pitch);
            qDebug() << "set pitch pwm:" << pitch;
        }   break;
        case MSG_CLIENT_SET_YAW_PWM: {
            float yaw; msg_data >> yaw; m_mavlinkInterface->setYawPWM(yaw);
            qDebug() << "set yaw pwm:" << yaw;
        }   break;
        case MSG_CLIENT_ALIGN:
            m_mavlinkInterface->align();
            qDebug() << "align";
            break;
        case MSG_CLIENT_SET_X: {
            float x; msg_data >> x; m_mavlinkInterface->setTargetX(x);
            qDebug() << "set x:" << x;
        }   break;
        case MSG_CLIENT_SET_Y: {
            float y; msg_data >> y; m_mavlinkInterface->setTargetY(y);
            qDebug() << "set y:" << y;
        }   break;
        case MSG_CLIENT_SET_Z: {
            float z; msg_data >> z; m_mavlinkInterface->setTargetZ(z);
            qDebug() << "set z:" << z;
        }   break;
        case MSG_CLIENT_SET_YAW: {
            float yaw; msg_data >> yaw; m_mavlinkInterface->setTargetYaw(yaw);
            qDebug() << "set yaw:" << yaw;
        }   break;
        }
    });
    connect(m_server, &Server::scriptMessageReceived, m_scriptWorker, &ScriptWorker::messageHandler);
    connect(m_scriptWorker, &ScriptWorker::sendMessage, m_server, &Server::sendScriptMessage);
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
        case ON_GROUND:
            //if (!m_mavlinkInterface->armed()) m_mavlinkInterface->setPos(0, 0, 0, 0);
            break;
        case TAKEOFF:
            if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count() - m_takeoffStartTime > 8000) {
                //land();
            }
            if (m_mavlinkInterface->odometry().z >= 0.3) {
                qDebug() << "orientation";
                m_state = ORIENTATION;
                uint16_t type_mask = 0;
                type_mask |= POSITION_TARGET_TYPEMASK_AX_IGNORE;
                type_mask |= POSITION_TARGET_TYPEMASK_AY_IGNORE;
                type_mask |= POSITION_TARGET_TYPEMASK_AZ_IGNORE;
                m_mavlinkInterface->setPositionTargetTypeMask(type_mask);
                m_mavlinkInterface->setVelocities(0, 0, 0);
            }
            break;
        case LANDING:
            //if (m_mavlinkInterface->z() == 0) {
            //    m_mavlinkInterface->setVelocities(0, 0, -2);
            //}
            if (m_mavlinkInterface->odometry().z <= 0.2 && qAbs(m_mavlinkInterface->vz()) <= 0.01) {
                m_restTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
            }
            /*
            if (m_mavlinkInterface->z() == 0 &&
                std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count() - m_restTime > 1000) {
                m_mavlinkInterface->setVelocities(0, 0, -2);
            }
            */
            if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count() - m_restTime > 4000) {
                drop();
            }
            break;
        default:
            break;
        }
    });
    moveControlTimer->start();
}

VehicleController::~VehicleController()
{
#ifdef __arm__
    ws2811_fini(&ledstring);
#endif
}

void VehicleController::takeoff(float z)
{
    if (m_state == ON_GROUND) {
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
        //type_mask |= POSITION_TARGET_TYPEMASK_YAW_RATE_IGNORE;
        m_mavlinkInterface->setPositionTargetTypeMask(type_mask);

        //m_state = TAKEOFF;
        m_state = ORIENTATION;
        m_mavlinkInterface->setTargetPos(0, 0, z, 0);
        m_mavlinkInterface->setVelocities(0, 0, 1.6);
        //m_takeoffTarget = z;
        if (!m_mavlinkInterface->armed()) m_mavlinkInterface->armDisarm(true);
    } else {
        uint16_t type_mask = 0;
        type_mask |= POSITION_TARGET_TYPEMASK_AX_IGNORE;
        type_mask |= POSITION_TARGET_TYPEMASK_AY_IGNORE;
        type_mask |= POSITION_TARGET_TYPEMASK_AZ_IGNORE;
        m_mavlinkInterface->setPositionTargetTypeMask(type_mask);
        m_mavlinkInterface->setVelocities(0, 0, 0);
        m_mavlinkInterface->setTargetX(m_arucoDetector->x());
        m_mavlinkInterface->setTargetY(m_arucoDetector->y());
        m_mavlinkInterface->setTargetZ(z);
    }
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
    //type_mask |= POSITION_TARGET_TYPEMASK_YAW_RATE_IGNORE;
    m_mavlinkInterface->setPositionTargetTypeMask(type_mask);

    m_state = LANDING;
    m_restTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    m_mavlinkInterface->setTargetPos(0, 0, 0, 0);
    //m_mavlinkInterface->land();
    m_mavlinkInterface->setVelocities(0, 0, -0.35);
}

void VehicleController::drop()
{
    m_state = ON_GROUND;
    m_mavlinkInterface->drop();
    m_mavlinkInterface->setPos(0, 0, 0, 0);
}

void VehicleController::setPoint(float x, float y, float z, float yaw)
{
    m_mavlinkInterface->setTargetPos(x, y, z, yaw);
}

bool VehicleController::checkPoint()
{
    return m_mavlinkInterface->distanceToPoint() <= 0.30;
}
