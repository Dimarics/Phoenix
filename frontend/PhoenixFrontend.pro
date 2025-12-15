include(src/src.pri)

QT += quick network

win32 {
    QT += serialport
}

CONFIG += qmltypes c++17

QML_IMPORT_PATH += $$PWD
QML_IMPORT_NAME = Hobots
QML_IMPORT_MAJOR_VERSION = 1
QML_IMPORT_MINOR_VERSION = 0

RESOURCES += resources \
    resources.qrc

wasm {
    source_path = $$PWD
    source_path ~= s,/,\\,g
    install_path = $$OUT_PWD
    install_path ~= s,/,\\,g
    #QMAKE_POST_LINK += $$quote(cmd /c copy /Y "$$source_path\\$${TARGET}.dll" "$$install_path\\..\\Desktop_Qt_6_7_2_MinGW_64_bit")
}

qnx: target.path = /tmp/$$TARGET/bin
else: unix:!android: target.path = /opt/$$TARGET/bin
!isEmpty(target.path): INSTALLS += target
