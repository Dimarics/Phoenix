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
    $$PWD/appeventfilter.h \
    $$PWD/networkinterface.h \
    $$PWD/streamviewer.h

SOURCES += \
    $$PWD/appeventfilter.cpp \
    $$PWD/main.cpp \
    $$PWD/appbackend.cpp \
    $$PWD/networkinterface.cpp \
    $$PWD/streamviewer.cpp

resources.files += \
    $$PWD/main.qml \
    $$PWD/PhoenixApps.qml \
    $$PWD/ControlPanel.qml \
    $$PWD/Device.qml \
    $$PWD/FPV.qml
