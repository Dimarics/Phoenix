import QtQuick
import QtQuick.Controls.Basic

ScrollBar {
    id: scrollBar
    contentItem: Rectangle {
        implicitWidth: 8
        implicitHeight: 8
        radius: 4
        color: "gray"
        opacity: scrollBar.pressed ? 0.45 : scrollBar.hovered ? 0.3 : 0.2
        Behavior on opacity { OpacityAnimator { duration: 300 } }
    }
}
