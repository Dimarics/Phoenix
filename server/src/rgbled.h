#ifndef RGBLED_H
#define RGBLED_H

#include <cstdint>

class RGBLed
{
public:
    RGBLed(uint8_t red_pin, uint8_t green_pin, uint8_t blue_pin);
    ~RGBLed();
    void setColor(uint8_t r, uint8_t g, uint8_t b);

private:
    uint8_t m_redPin;
    uint8_t m_greenPin;
    uint8_t m_bluePin;
};

#endif // RGBLED_H
