import QtQuick
import QtQuick.Layouts
import App
import Hobots
import Components

Item {
    id: root
    anchors.verticalCenter: parent.verticalCenter
    implicitHeight: layout.implicitHeight + layout.anchors.topMargin + layout.anchors.bottomMargin
    implicitWidth: 450
    //width: parent.width
    component CPText: Text {
        horizontalAlignment: Qt.AlignRight
        Layout.alignment: Qt.AlignVCenter
    }
    ColumnLayout {
        id: layout
        spacing: 32
        anchors.fill: parent; anchors.margins: 16
        Text {
            id: battery_status
            property int remaining: 0
            text: remaining < 0 ? "Аккумулятор отсутствует" : `Аккумулятор: ${remaining}%`
            color: remaining > 50 ? "#00FF00" : remaining > 20 ? "#FF8000" : "red"
            Layout.alignment: Qt.AlignHCenter
            //Layout.topMargin: 12
        }
        Text {
            id: system_status
            property string status: "не готов"
            text: "Статус: " + status
            color: "red"
            Layout.topMargin: -12
            Layout.alignment: Qt.AlignHCenter
        }
        RowLayout {
            spacing: 16
            Layout.alignment: Qt.AlignHCenter
            Text { text: "Режим" }
            ComboBox {
                id: modeBox
                currentIndex: -1
                model: [ "Stabilized", "Offboard" ]
                Layout.preferredWidth: 120
                onActivated: index => {
                                 console.log(index)
                                 App.device.protocol.setMode(textAt(index))
                             }
            }
        }
        RowLayout {
            spacing: 16
            Layout.alignment: Qt.AlignHCenter
            Button {
                contentItem: Text { text: App.device.armed ? "disarm" : "arm"; horizontalAlignment: Text.AlignHCenter }
                Layout.preferredWidth: 100; Layout.preferredHeight: 40
                onClicked: App.device.protocol.armDisarm(!App.device.armed)
            }
            Button {
                visible: false
                checked: false
                checkable: true
                contentItem: Text { text: parent.checked ? "hold off" : "hold on"; horizontalAlignment: Text.AlignHCenter }
                Layout.preferredWidth: 100; Layout.preferredHeight: 40
                onClicked: App.device.protocol.holdOnMarker(checked)
            }
            Button {
                contentItem: Text { text: "takeoff"; horizontalAlignment: Text.AlignHCenter }
                Layout.preferredWidth: 100; Layout.preferredHeight: 40
                onClicked: App.device.protocol.takeoff(1)
            }
            Button {
                contentItem: Text { text: "land"; horizontalAlignment: Text.AlignHCenter }
                Layout.preferredWidth: 100; Layout.preferredHeight: 40
                onClicked: App.device.protocol.land()
            }
        }
        Button {
            contentItem: Text { text: "drop"; horizontalAlignment: Text.AlignHCenter }
            Layout.preferredWidth: 100; Layout.preferredHeight: 40
            onClicked: App.device.protocol.drop()
        }
        GridLayout {
            id: coordinates
            rows: 3
            flow: GridLayout.TopToBottom
            rowSpacing: 16; columnSpacing: 16
            Layout.alignment: Qt.AlignCenter
            CPText { text: "X, мм" }
            CPText { text: "Y, мм" }
            CPText { text: "Z, мм" }
            NumberField {
                id: coord_x
                decimals: 0
                min: -50000; max: 50000;
                onValueModified: App.device.protocol.setX(value / 1000)
            }
            NumberField {
                id: coord_y
                decimals: 0
                min: -50000; max: 50000;
                onValueModified: App.device.protocol.setY(value / 1000)
            }
            NumberField {
                id: coord_z
                decimals: 0
                min: -50000; max: 50000;
                onValueModified: App.device.protocol.setZ(value / 1000)
            }
            CPText { text: "Крен, °"; Layout.preferredWidth: yaw_label.width }
            CPText { text: "Тангаж, °"; Layout.preferredWidth: yaw_label.width }
            CPText { id: yaw_label; text: "Рысканье, °" }
            NumberField {
                id: roll
                decimals: 1
                min: -360; max: 360
            }
            NumberField {
                id: pitch
                decimals: 1
                min: -360; max: 360
            }
            NumberField {
                id: yaw
                decimals: 1
                min: -360; max: 360
                onValueModified: App.device.protocol.setYaw(value * Math.PI / 180)
            }
        }
        RowLayout {
            //width: parent.width
            //Layout.preferredWidth: parent.width
            Layout.alignment: Qt.AlignHCenter
            spacing: 32
            Joystick {
                leftText: "↺"; rightText: "↻"; topText: "up"; bottomText: "down"
                /*
                onTopPressed: App.device.protocol.setPitch(40)
                onBottomPressed: App.device.protocol.setPitch(-40)
                onLeftPressed: App.device.protocol.setRoll(-40)
                onRightPressed: App.device.protocol.setRoll(40)
                onTopReleased: App.device.protocol.setPitch(0)
                onBottomReleased: App.device.protocol.setPitch(0)
                onLeftReleased: App.device.protocol.setRoll(0)
                onRightReleased: App.device.protocol.setRoll(0)
                */
            }
            Joystick {
                //leftText: "🡰"; rightText: "🡲"; topText: "🡱"; bottomText: "🡳"
                leftText: "◄"; rightText: "►"; topText: "▲"; bottomText: "▼"
                onTopPressed: App.device.protocol.setPitchPWM(40)
                onBottomPressed: App.device.protocol.setPitchPWM(-40)
                onLeftPressed: App.device.protocol.setRollPWM(-40)
                onRightPressed: App.device.protocol.setRollPWM(40)
                onTopReleased: App.device.protocol.setPitchPWM(0)
                onBottomReleased: App.device.protocol.setPitchPWM(0)
                onLeftReleased: App.device.protocol.setRollPWM(0)
                onRightReleased: App.device.protocol.setRollPWM(0)
            }
        }
        RowLayout {
            visible: modeBox.currentText === "Stabilized"
            spacing: 16
            Text { text: "Тяга:" }
            Volume {
                min: 0; max: 100; value: 0
                decimals: 0
                spacing: parent.spacing
                Layout.fillWidth: true
                onValueModified: App.device.protocol.setThrottlePWM(value)
            }
        }
    }
    Connections {
        target: App.device.protocol
        ignoreUnknownSignals: true
        function onSystemStatusChanged(value: int) {
            var label, color
            switch(value) {
            case 3: label = "готов"; color = "#00FF00"; break
            case 4: label = "моторы запущены"; color = "#00FFFF"; break
            default: label = "не готов"; color = "red"; break
            }
            system_status.status = label
            system_status.color = color
        }
        function onBatteryRemainingChanged(value: int) {
            battery_status.remaining = value
        }
        function onModeChanged(mode: int) {
            var modeName
            switch(mode) {
            case NetworkInterface.PX4_MAIN_MODE_STABILIZED: modeName = "Stabilized"; break
            case NetworkInterface.PX4_MAIN_MODE_POSCTL: modeName = "Position"; break
            case NetworkInterface.PX4_MAIN_MODE_OFFBOARD: modeName = "Offboard"; break
            default: modeBox.currentIndex = -1; return
            }
            modeBox.currentIndex = modeBox.find(modeName)
        }
        function onPositionChanged(x: real, y: real, z: real, _roll: real, _pitch: real, _yaw: real) {
            coord_x.setValue(x * 1000); coord_y.setValue(y * 1000); coord_z.setValue(z * 1000)
            roll.setValue(_roll * 180 / Math.PI); pitch.setValue(_pitch * 180 / Math.PI); yaw.setValue(_yaw * 180 / Math.PI)
        }
    }
}
