#include "rgbled.h"

#ifdef __arm__
#include <pigpio.h>

RGBLed::RGBLed(uint8_t red_pin, uint8_t green_pin, uint8_t blue_pin) : m_redPin(red_pin), m_greenPin(green_pin), m_bluePin(blue_pin)
{
    if (gpioInitialise() < 0) {
        return;
    }

    gpioSetMode(red_pin, PI_OUTPUT);
    gpioSetPWMfrequency(red_pin, 1000);
    gpioSetPWMrange(red_pin, 255);

    gpioSetMode(green_pin, PI_OUTPUT);
    gpioSetPWMfrequency(green_pin, 1000);
    gpioSetPWMrange(green_pin, 255);

    gpioSetMode(blue_pin, PI_OUTPUT);
    gpioSetPWMfrequency(blue_pin, 1000);
    gpioSetPWMrange(blue_pin, 255);
}

RGBLed::~RGBLed()
{
    gpioPWM(m_redPin, 0);
    gpioPWM(m_greenPin, 0);
    gpioPWM(m_bluePin, 0);
}

void RGBLed::setColor(uint8_t r, uint8_t g, uint8_t b)
{
    gpioPWM(m_redPin, r);
    gpioPWM(m_greenPin, g);
    gpioPWM(m_bluePin, b);
}

#else

RGBLed::RGBLed(uint8_t, uint8_t, uint8_t) { }

RGBLed::~RGBLed() { }

void RGBLed::setColor(uint8_t, uint8_t, uint8_t) { }

#endif
