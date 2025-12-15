import QtQuick
import Hobots
import Components
import App

QtObject {
    id: root
    property real sonarDistance
    property list<int> arucoIds
    readonly property Connections connections: Connections {
        target: protocol
        ignoreUnknownSignals: true
        function onSonarDistanceChanged(value: real) { sonarDistance = value; }
        function onArucoIdsChanged(list: list<int>) { arucoIds = list
            /*protocol.log("list begin")
            for (let l of list) {
                protocol.log(l)
            }
            protocol.log("list end")*/
        }
    }
    property QtObject protocol: NetworkInterface {}
}
