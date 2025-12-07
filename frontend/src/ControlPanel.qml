import QtQuick
import QtQuick.Layouts
import App
import Hobots
import Components

Item {
    id: root
    anchors.verticalCenter: parent.verticalCenter
    implicitHeight: layout.implicitHeight + layout.anchors.topMargin + layout.anchors.bottomMargin
    implicitWidth: 350
    //width: parent.width
    component CPText: Text {
        horizontalAlignment: Qt.AlignRight
        Layout.preferredWidth: 80; Layout.alignment: Qt.AlignVCenter
    }
    ColumnLayout {
        id: layout
        spacing: 16
        anchors.fill: parent; anchors.margins: 16
        Button {
            text: "Arm"
            Layout.bottomMargin: 20
            onClicked: App.device.protocol.arm()
        }
        RowLayout {
            id: joints
            //width: parent.width
            //Layout.preferredWidth: parent.width
            Layout.alignment: Qt.AlignHCenter
            spacing: 40
            Joystick {
                //leftText: "🡰"; rightText: "🡲"; topText: "🡱"; bottomText: "🡳"
                leftText: "◄"; rightText: "►"; topText: "▲"; bottomText: "▼"
            }
        }
        Text { text: "Скорость:"; Layout.topMargin: 20 }
        Volume {
            min: 1; max: 100; value: 50
            decimals: 0
            spacing: parent.spacing
            Layout.fillWidth: true
        }
    }
    /*
    Connections {
        target: App.device.protocol
        ignoreUnknownSignals: true
        function onJointChanged(id, value) {
            jointFields.itemAt(id - 1).setValue(value)
        }
        function onStateChanged(state) {
            if (state === HobotInterface.Unconnected) {
                for (let i = 0; i < jointFields.count; ++i) {
                    jointFields.itemAt(i).clear()
                }
            }
        }
    }
    */
}
