include(Apps/Apps.pri)
include(Components/Components.pri)
include(ScratchBlocks/ScratchBlocks.pri)

INCLUDEPATH += $$PWD
QML_IMPORT_PATH += $$PWD

INCLUDEPATH += \
    $$PWD \
    $$PWD/mavlink

HEADERS += \
    $$PWD/appbackend.h \
    $$PWD/mavlinkinterface.h

SOURCES += \
    $$PWD/main.cpp \
    $$PWD/appbackend.cpp \
    $$PWD/mavlinkinterface.cpp

resources.files += \
    $$PWD/main.qml \
    $$PWD/PhoenixApps.qml \
    $$PWD/ControlPanel.qml \
    $$PWD/Device.qml
