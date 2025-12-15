import QtQuick
import QtQuick.Controls.Basic as T
import QtQuick.Layouts

T.Button {
    id: root
    property alias borderWidth: backgroundRect.border.width
    property alias backgroundColor: backgroundRect.color
    property alias borderRadius: backgroundRect.radius
    property alias borderColor: backgroundRect.border.color
    property alias toolTip: toolTip.text
    //backgroundColor: Style.darkWidgetColor
    borderWidth: 2
    borderRadius: 8
    backgroundColor: pressed ? "#7346BE" : hovered ? "#400080" : "#400080"
    borderColor:     pressed ? "#7346BE" : hovered ? "#7346BE" : "#5A3597"
    width: 56; height: 56
    hoverEnabled: Qt.platform.os !== "android"
    scale: pressed ? 0.95 : 1
    display: T.AbstractButton.TextUnderIcon
    icon.width: 40; icon.height: 40
    icon.color: pressed ? "#FF80FF" : hovered ? "#FF80FF" : "#FFFFFF"
    Layout.preferredWidth: width; Layout.preferredHeight: height
    Layout.alignment: Qt.AlignCenter
    background: Rectangle {
        id: backgroundRect
        border.width: 2
        Behavior on color { ColorAnimation { duration: 200 } }
        Behavior on border.color { ColorAnimation { duration: 100 } }
    }
    containmentMask: RoundedRectMask { radius: borderRadius; width: root.width; height: root.height }
    Behavior on icon.color { ColorAnimation { duration: 100 } }
    Behavior on scale { NumberAnimation { duration: 70 } }
    ToolTip { id: toolTip; visible: root.hovered && text }
}
