import QtQuick
import QtQuick.Layouts
import QtQuick.Shapes

BasicBlock {
    id: root
    default property alias content: layout.data
    //property var calculate
    //property var value
    objectName: "Value"
    type: "value"
    plugX: 32; plugY: height / 2
    //width: implicitWidth; height: implicitHeight
    implicitWidth: Math.max(48, layout.implicitWidth + layout.anchors.leftMargin + layout.anchors.rightMargin)
    implicitHeight: Math.max(40, layout.implicitHeight + 8)
    borderColor: "#389438"
    fillColor: "#59C059"
    shapePath: ShapePath {
        startX: height / 2
        PathLine { relativeX: root.width - root.height }
        PathArc { relativeX: 0; relativeY: height; radiusX: height / 2; radiusY: height / 2 }
        PathLine { relativeX: -width + height; relativeY: 0 }
        PathArc { relativeX: 0; relativeY: -height; radiusX: height / 2; radiusY: height / 2 }
    }
    RowLayout {
        id: layout
        spacing: 8
        anchors { fill: parent; leftMargin: 4; rightMargin: 4 }
        //children: content
        Component.onCompleted: {
            //console.log(layout.children[0] instanceof ScratchText)
            if (layout.children[0] instanceof ScratchText)
                layout.anchors.leftMargin = 12
            else if (layout.children[0] instanceof ListItem)
                layout.anchors.leftMargin = 16
            if (layout.children[layout.children.length - 1] instanceof ScratchText)
                layout.anchors.rightMargin = 12
            if (layout.children[layout.children.length - 1] instanceof ListItem)
                layout.anchors.rightMargin = 16
        }
    }
}
