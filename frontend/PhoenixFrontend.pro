include(src/src.pri)

QT += quick network

CONFIG += qmltypes c++17

QML_IMPORT_PATH += $$PWD
QML_IMPORT_NAME = Hobots
QML_IMPORT_MAJOR_VERSION = 1
QML_IMPORT_MINOR_VERSION = 0

RESOURCES += resources \
    resources.qrc

wasm {
} else {
    QT += serialport
}

win32:  {
    LIBS += -lgdi32 -ldwmapi

    target_path = $$PWD
    target_path ~= s,/,\\,g
    resourse_path = $$target_path\\..\\..\\Hobots_v2
    #system(xcopy /Y /E "$$resourse_path\\src\\Apps" "$$target_path"\\Apps)
    #system(xcopy /Y /E "$$resourse_path\\src\\Components" "$$target_path"\\Components)
    #system(xcopy /Y /E "$$resourse_path\\src\\interfaces" "$$target_path"\\interfaces)
    #system(xcopy /Y /E "$$resourse_path\\src\\Hobot_2" "$$target_path"\\Hobot_2)
    #system(xcopy /Y /E "$$resourse_path\\src\\interfaces" "$$target_path"\\interfaces)
    #system(xcopy /Y /E "$$resourse_path\\images" "$$target_path"\\images)
    #system(xcopy /Y /E "$$resourse_path\\fonts" "$$target_path"\\fonts)
}

macos {
    ICON = mac_icon.icns
}

qnx: target.path = /tmp/$$TARGET/bin
else: unix:!android: target.path = /opt/$$TARGET/bin
!isEmpty(target.path): INSTALLS += target
