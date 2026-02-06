import QtQuick
import Components
import Hobots

Item {
    signal exit
    StreamViewer {
        anchors.fill: parent
    }
    Button {
        id: exitButton
        toolTip: "Выход"
        //icon.color: Style.buttonHoveredBorderColor
        icon.source: "qrc:/images/exit.svg"
        icon.width: 26; icon.height: 26
        //opacity: hovered ? 1 : 0.4
        width: 50; height: 50
        x: parent.width - width - 10; y: 10
        //anchors { verticalCenter: parent.verticalCenter; right: parent.right; margins: 15}
        onClicked: exit()
    }
}
