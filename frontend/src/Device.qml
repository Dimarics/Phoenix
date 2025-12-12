import QtQuick
import Hobots
import Components

HobotDevice {
    id: root
    joints: [
        Joint { min: -180; max: 180 },
        Joint { min:    0; max: 180 },
        Joint { min:    0; max: 360 },
        Joint { min:    0; max: 360 },
        Joint { min: -180; max: 180 },
        Joint { min: -360; max: 180 }
    ]
    availableFeatures: [
        HobotDevice.Joints,
        HobotDevice.Speed,// HobotDevice.Accel,
        HobotDevice.Grub
    ]
    availableProtocols: ["COM порт", "Точка доступа", "Локальная сеть"]
    protocol: Interface {}
    /*onProtocolNameChanged: {
        var qml
        switch(protocolName) {
        case "MAVLink": qml = "import Hobots; MAVLinkInterface{}"; break
        default: return
        }
        if (protocol) protocol.destroy()
        protocol = Qt.createQmlObject(qml, this)
    }*/
}
