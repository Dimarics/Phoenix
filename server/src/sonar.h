#ifndef SONAR_H
#define SONAR_H

#ifdef QT_OS_LINUX
#include <pigpio.h>
#include <thread>

class Sonar
{
public:
    Sonar(uint8_t trigPin, uint8_t echoPin) : m_trigPin(trigPin), m_echoPin(echoPin) {}
    void tick() {
        if (m_threadRunning) return;
        m_threadRunning = true;
        std::thread([this] {
            const double SOUND_SPEED = 343000.0; // мм/с
            uint32_t start, end;
            int timeout = 0;

            gpioWrite(m_trigPin, 1);
            gpioDelay(10);
            gpioWrite(m_trigPin, 0);

            // Ждем LOW->HIGH переход
            while (gpioRead(m_echoPin) == 0) {
                if (++timeout > 10'000) {
                    m_distance = -1;
                    m_threadRunning = false;
                    return;
                }
                gpioDelay(10);
            }
            start = gpioTick();

            // Ждем HIGH->LOW переход
            timeout = 0;
            while (gpioRead(m_echoPin) == 1) {
                if (++timeout > 10'000) {
                    m_distance = -1;
                    m_threadRunning = false;
                    return;
                }
                gpioDelay(10);
            }
            end = gpioTick();

            double pulseDuration = (end - start) / 1'000'000.0;
            m_distance = (pulseDuration * SOUND_SPEED) / 2.0;
            m_threadRunning = false;

            if (m_distance < 20 || m_distance > 10'000) {
                m_distance = -1;
                return;
            }
        });
    }
    double distance() const { return m_distance; }

private:
    bool m_threadRunning = false;
    uint8_t m_trigPin;
    uint8_t m_echoPin;
    double m_distance;
};

#else

class Sonar
{
public:
    Sonar(unsigned char, unsigned char) {}
    void tick() {}
    double distance() const { return -1; }
};

#endif

#endif // SONAR_H
