unix: {
    include(rpi_ws281x/rpi_ws281x.pri)
}

INCLUDEPATH += \
    $$PWD \
    $$PWD/mavlink

HEADERS += \
    $$PWD/arucodetector.h \
    $$PWD/i2c.h \
    $$PWD/mavlinkinterface.h \
    $$PWD/pidcontroller.h \
    $$PWD/pointtracker.h \
    $$PWD/rgbled.h \
    $$PWD/scriptworker.h \
    $$PWD/server.h \
    $$PWD/sonar.h \
    $$PWD/vehiclecontroller.h \
    $$PWD/videocapture.h \
    $$PWD/vl53l0x.h

SOURCES += \
    $$PWD/arucodetector.cpp \
    $$PWD/i2c.cpp \
    $$PWD/main.cpp \
    $$PWD/mavlinkinterface.cpp \
    $$PWD/pidcontroller.cpp \
    $$PWD/pointtracker.cpp \
    $$PWD/rgbled.cpp \
    $$PWD/scriptworker.cpp \
    $$PWD/server.cpp \
    $$PWD/sonar.cpp \
    $$PWD/vehiclecontroller.cpp \
    $$PWD/videocapture.cpp \
    $$PWD/vl53l0x.cpp
