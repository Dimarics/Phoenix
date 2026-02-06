import QtQuick
import QtQuick.Layouts
//import Hobots
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
        Script {}
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
