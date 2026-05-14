#ifndef I2C_H
#define I2C_H

#include <cstdint>

#ifdef __arm__

#include "fcntl.h"
#include "unistd.h"
#include "linux/i2c-dev.h"
#include "sys/ioctl.h"
#include <QDebug>

class I2C
{
public:
    I2C(const char *device = "/dev/i2c-1") {
        m_fileDescriptor = open(device, O_RDWR);
        if (m_fileDescriptor < 0) {
            qDebug() << "file not open";
        }
    }
    void writeBytes(uint8_t address, uint8_t *data, size_t size) {
        if (ioctl(m_fileDescriptor, I2C_SLAVE, address) < 0) {
            qDebug() << "ioctl error";
        }
        write(m_fileDescriptor, data, size);
    }
    void writeByte(uint8_t address, uint8_t byte) {
        writeBytes(address, &byte, 1);
    }
    void readBytes(uint8_t address, uint8_t *data, size_t size) {
        if (ioctl(m_fileDescriptor, I2C_SLAVE, address) < 0) {
            qDebug() << "ioctl error";
        }
        read(m_fileDescriptor, data, size);
    }

private:
    int m_fileDescriptor;
};

#else

class I2C
{
public:
    I2C(const char *device = "") {}
    void writeBytes(uint8_t address, uint8_t *data, size_t size) { }
    void writeByte(uint8_t address, uint8_t byte) { }
    void readBytes(uint8_t address, uint8_t *data, size_t size) { }
};

#endif // __arm__

#endif // I2C_H
