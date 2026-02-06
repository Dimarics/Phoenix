import QtQuick
import Hobots
import Components
import App

QtObject {
    id: root
    property int mode
    property bool armed: false
    property bool buttonState
    property real x
    property real y
    property real z
    property real roll
    property real pitch
    property real yaw
    property real sonarDistance
    property list<int> arucoIds
    readonly property Connections connections: Connections {
        target: root.protocol
        ignoreUnknownSignals: true
        function onModeChanged(mode: int) { root.mode = mode; }
        function onArmChanged(state: bool) { root.armed = state; }
        function onPositionChanged(x: real, y: real, z: real, roll: real, pitch: real, yaw: real) {
            root.x = x; root.y = y; root.z = z
            root.roll = roll; root.pitch = pitch; root.yaw = yaw
        }
        function onArucoIdsChanged(list: list<int>) { root.arucoIds = list
            /*protocol.log("list begin")
            for (let l of list) {
                protocol.log(l)
            }
            protocol.log("list end")*/
        }
        function onButtonStateChanged(state: bool) { root.buttonState = !state; }
        function onSonarDistanceChanged(value: real) { root.sonarDistance = value; }
    }
    property QtObject protocol: NetworkInterface {}
}
