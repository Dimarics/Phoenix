import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import ".."

ValueBlock {
    property alias name: text.text
    objectName: "Variable"
    borderColor: "#DB6E00"
    fillColor: "#FF8C1A"
    Component.onCompleted: {
        workspace.removeVariable.connect(deletedName => { if (deletedName === name) destruct() })
        workspace.renameVariable.connect((currentName, newName) => { if (currentName === name) name = newName })
    }
    ScratchText { id: text; horizontalAlignment: Text.AlignHCenter; Layout.fillWidth: true }
    PointHandler {
        acceptedButtons: Qt.RightButton
        onActiveChanged: if (active) contextMenu.popup()
        //onGrabChanged: (transition, point) => console.log(transition, point.grabPosition)
        //onGrabChanged: console.log("aaa")
    }
    Menu {
        id: contextMenu
        font.pointSize: 12
        MenuItem {
            text: "Переименовать"
            onTriggered: workspace.variableRenameRequest(name)
        }
        MenuItem {
            text: "Удалить"
            onTriggered: workspace.removeVariable(name)
        }
    }
    //function copyData(target) { target.name = name }
    function value() { return workspace.variables.get(name).value }
    function getData() { return { "name": name } }
    function setData(data) { name = data.name }
    function cppCode(): string { return name }
}
