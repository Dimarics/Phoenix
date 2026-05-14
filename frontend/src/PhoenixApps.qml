import QtQuick
import QtQuick.Layouts
//import Hobots
import App
import Components
import Apps.Scratch
import Apps.Scratch.Blocks
import Apps.Script
import "ScratchBlocks"

pragma ComponentBehavior: Bound

Item {
    id: root
    property Item currentPage: setPage(appsMenu)
    //implicitWidth: currentPage.implicitWidth
    //implicitHeight: currentPage.implicitHeight
    width: {
        var implicitWidth = currentPage.implicitWidth
        if (currentPage.anchors) implicitWidth += currentPage.anchors.leftMargin + currentPage.anchors.rightMargin
        return Math.max(implicitWidth, apps.width)
    }
    height: {
        var implicitHeight = currentPage.implicitHeight
        if (currentPage.anchors) implicitHeight += currentPage.anchors.topMargin + currentPage.anchors.bottomMargin
        return Math.max(implicitHeight, apps.height)
    }
    //width: Math.max(currentPage.implicitWidth, apps.width)
    //height: Math.max(currentPage.implicitHeight, apps.height)
    Component {
        id: scratch
        Scratch {
            availableModes: Scratch.Debug
            movement: [
                Arm {},
                Disarm {},
                Takeoff {},
                Land {},
                SetPoint {},
                SetYaw {},
                //SetThrottle {},
                //SetPitch {},
                //SetRoll {},
                Align {}
            ]
            controls: [
                Delay {}
            ]
            sensors: [
                PhxSonarDistance {},
                PhxButtonState {},
                ArucoId {}
            ]
        }
    }
    Component {
        id: script
        Script {
            templates: [
                //{ "tip": "значение x", "template": "x()" },
                //{ "tip": "значение y", "template": "y()" },
                //{ "tip": "значение z", "template": "z()" },
                //{ "tip": "двигаться по x", "template": "moveX(value)" },
                //{ "tip": "двигаться по x", "template": "moveX(value)" },
                //{ "tip": "двигаться по y", "template": "moveY(value)" },
                //{ "tip": "двигаться по z", "template": "moveZ(value)" },
                //{ "tip": "перейти в точку x, y", "template": "moveXY(x, y)"},
                { "tip": "включить моторы", "template": "arm()"},
                { "tip": "выключить моторы", "template": "disarm()"},
                { "tip": "взлёт", "template": "takeoff(значение в метрах)"},
                { "tip": "посадка", "template": "land()"},
                { "tip": "перейти в точку x, y, z", "template": "setPoint(x, y, z в миллиметрах)" },
                { "tip": "установить рысканье", "template": "setYaw(значение в градусах)"},
                { "tip": "выровняться", "template": ""},
                { "tip": "ждать", "template": "delay(значение в миллисекундах)"}
            ]
            function commandsHandler(command, args) {
                switch(command) {
                    /*
                case "moveX":
                    App.device.protocol.setX(args[1])
                    next()
                    break;
                case "moveY":
                    App.device.protocol.setY(args[1])
                    break;
                case "moveZ":
                    App.device.protocol.setZ(args[1])
                    break;
                case "moveXY":
                    App.device.protocol.setX(args[1])
                    App.device.protocol.setY(args[2])
                    break;
                case "setPoint":
                    App.device.protocol.setX(args[1])
                    App.device.protocol.setY(args[1])
                    App.device.protocol.setZ(args[1])
                    break;
                    */
                case "arm":
                    App.device.protocol.armDisarm(true)
                    break;
                case "disarm":
                    App.device.protocol.armDisarm(false)
                    break;
                case "takeoff":
                    App.device.protocol.takeoff(args[1] / 1000)
                    break;
                case "land":
                    App.device.protocol.land()
                    break;
                case "setPoint":
                    App.device.protocol.setX(args[1] / 1000)
                    App.device.protocol.setY(args[2] / 1000)
                    App.device.protocol.setZ(args[3] / 1000)
                    break;
                case "setYaw":
                    App.device.protocol.setYaw(args[1])
                    break;
                case "align":
                    App.device.protocol.align()
                    break;
                case "delay":
                    delay(args[1])
                    break;
                }
                if (command != "delay") next()
            }
        }
    }
    Component {
        id: fpv
        FPV {}
    }
    Component {
        id: appsMenu
        RowLayout {
            spacing: 1
            anchors.margins: 16
            //Layout.margins: 16
            //Layout.preferredWidth: apps.width; Layout.preferredHeight: apps.height
            ModeButton {
                text: "Scratch"
                icon.source: "qrc:/images/scratch.svg"
                icon.width: 160; icon.height: 160
                onClicked: root.setPage(scratch)
            }
            ModeButton {
                //visible: false
                text: "Скрипт"
                icon.source: "qrc:/images/script.svg"
                icon.width: 160; icon.height: 160
                onClicked: root.setPage(script)
            }
            ModeButton {
                //visible: false
                text: "FPV"
                icon.source: "qrc:/images/fpv_mode.svg"
                icon.width: 160; icon.height: 160
                onClicked: root.setPage(fpv)
            }
        }
    }
    function setPage(page: Component): Item {
        if (currentPage) currentPage.destroy()
        currentPage = page.createObject(this, { 'anchors.fill': this })
        if (currentPage.exit) currentPage.exit.connect(() => setPage(appsMenu))
    }
}
