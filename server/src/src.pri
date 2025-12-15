INCLUDEPATH += \
    $$PWD \
    $$PWD/MAVLink \
    $$PWD/pigpio

HEADERS += \
    $$PWD/arucodetector.h \
    $$PWD/httpserver.h \
    $$PWD/mavlinkinterface.h \
    $$PWD/sonar.h

SOURCES += \
    $$PWD/arucodetector.cpp \
    $$PWD/main.cpp \
    $$PWD/httpserver.cpp \
    $$PWD/mavlinkinterface.cpp \
    $$PWD/sonar.cpp

unix {
    HEADERS += \
        $$PWD/pigpio/pigpio.h

    SOURCES += \
        $$PWD/pigpio/pigpio.c
}
