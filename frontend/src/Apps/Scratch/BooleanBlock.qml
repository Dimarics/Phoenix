import QtQuick
import QtQuick.Layouts
import QtQuick.Shapes

BasicBlock {
    id: root
    default property alias content: layout.data
    //property bool value: false
    objectName: "Boolean"
    type: "bool"
    plugX: 32; plugY: height / 2
    //width: implicitWidth; height: implicitHeight
    implicitWidth: layout.implicitWidth + layout.anchors.leftMargin + layout.anchors.rightMargin
    implicitHeight: Math.max(40, layout.implicitHeight + 8)
    borderColor: "#389438"
    fillColor: "#59C059"
    shapePath: ShapePath {
        startY: height / 2
        PathPolyline {
            path: [
                Qt.point(0, root.height / 2), Qt.point(root.height / 2, 0), Qt.point(root.width - root.height / 2, 0),
                Qt.point(root.width, root.height / 2), Qt.point(root.width - root.height / 2, root.height),
                Qt.point(root.height / 2, root.height), Qt.point(0, root.height / 2)
            ]
        }
    }
    RowLayout {
        id: layout
        spacing: 8
        anchors { fill: parent; leftMargin: 8; rightMargin: 8 }
        Component.onCompleted: {
            //console.log(layout.children[0] instanceof ScratchText)
            if (layout.children[0] instanceof ScratchText ||
                layout.children[0] instanceof ValueInput ||
                layout.children[0] instanceof ListItem)
                layout.anchors.leftMargin = Qt.binding(() => root.implicitHeight / 2)
            if (layout.children[layout.children.length - 1] instanceof ScratchText ||
                layout.children[layout.children.length - 1] instanceof ValueInput ||
                layout.children[layout.children.length - 1] instanceof ListItem)
                layout.anchors.rightMargin = Qt.binding(() => root.implicitHeight / 2)
        }
        //children: content
        /*BooleanSocket { block: root }
        ScratchText { text: "или" }
        BooleanSocket { block: root }*/
    }
}
