import QtQuick
import QtQuick.Controls.Basic as T
import QtQuick.Layouts
import App
import Hobots
import Components as C

Window {
    id: window
    width: 900
    height: 480
    color: C.Style.lightWidgetColor
    visibility: Qt.platform.os === "wasm" ? Window.FullScreen : Window.Maximized
    //visibility: Window.FullScreen
    Component.onCompleted: {
        App.deviceName = "Phoenix"
        App.device = device
    }
    Device {
        id: device
        protocolName: "MAVLink"
    }
    /*C.JsonSettings {
        id: settings
        path: App.appDataLocation + "settings.json"
        property alias windowX: window.x
        property alias windowY: window.y
        property alias windowWidth: window.width
        property alias windowHeight: window.height
        property alias windowVisibility: window.visibility
        property string currentDevice
        onLoaded: deviceBox.currentIndex = deviceBox.find(currentDevice)
    }*/
    Rectangle {
        //id: controlPanelBackground
        color: C.Style.headerBackgroundColor
        width: parent.width; height: 28
    }
    T.Button {
        id: hideControlPanel
        width: 28; height: 28
        anchors.right: parent.right
        checkable: true
        icon.source: checked ? "qrc:/images/left-arrow.svg" : "qrc:/images/right-arrow.svg"
        icon.width: 7; icon.height: 12
        icon.color: hovered ? "#00FFFF" : "white"
        background: Rectangle {
            color: hideControlPanel.pressed ? "#32ffffff" : hideControlPanel.hovered ? "#1effffff" : "transparent"
            Behavior on color { ColorAnimation { duration: 100 } }
        }
        // Анимация
        onClicked: {
            controlPanelAnimation.from = checked ? controlPanel.implicitWidth : 0
            controlPanelAnimation.to = checked ? 0 : controlPanel.implicitWidth
            controlPanelAnimation.start()
        }
        NumberAnimation {
            id: controlPanelAnimation
            target: controlPanel
            property: "width"
            duration: 300
        }
        Behavior on icon.color { ColorAnimation { duration: 100 } }
    }
    Item {
        id: controlPanel
        implicitWidth: controlPanelModel.implicitWidth
        width: hideControlPanel.checked ? 0 : implicitWidth; height: parent.height
        anchors.right: parent.right
        C.Text {
            text: "Панель управления"
            width: parent.width - 28; height: 28
            horizontalAlignment: Text.AlignHCenter
        }
        Rectangle { width: 2; height: parent.height; color: C.Style.darkBorderColor }
        C.Flickable {
            id: controlPanelFlickable
            anchors { fill: parent; topMargin: 28; leftMargin: 2}
            contentWidth: width
            contentHeight: Math.max(height, controlPanelModel.implicitHeight)
            ControlPanel { id: controlPanelModel }
        }
    }
    C.Text {
        id: appsHeader
        text: "Приложения"
        horizontalAlignment: Text.AlignHCenter
        height: 28
        anchors {
            left: parent.left; right: controlPanel.left
            rightMargin: hideControlPanel.checked ? hideControlPanel.width : 0
        }
        Behavior on anchors.rightMargin { NumberAnimation { duration: 300 } }
    }
    C.Flickable {
        id: apps
        clip: true
        contentWidth: phoenix_apps.width; contentHeight: phoenix_apps.height
        boundsMovement: Flickable.StopAtBounds
        boundsBehavior: Flickable.DragAndOvershootBounds
        anchors { top: appsHeader.bottom; left: parent.left; right: controlPanel.left; bottom: parent.bottom }
        PhoenixApps { id: phoenix_apps }
    }
}
