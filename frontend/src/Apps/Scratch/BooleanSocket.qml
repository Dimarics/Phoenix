import QtQuick
import QtQuick.Shapes
//import QtQuick.Layouts

Socket {
    //property bool value: false
    type: "bool"
    width: next ? next.width : 48; height: next ? next.height : 32
    visible: true
    opacity: !next
    offsetX: 32; offsetY: height / 2
    implicitWidth: next ? next.width : 48; implicitHeight: next ? next.height : 32
    //Layout.preferredWidth: next ? next.width : 48
    //Layout.preferredHeight: next ? next.height : 32
    //Layout.minimumWidth: 40; Layout.minimumHeight: 32;
    path: [
        PathMove { y: 16 },
        PathPolyline {
            path: [
                Qt.point(0, 16), Qt.point(16, 0), Qt.point(32, 0), Qt.point(48, 16),
                Qt.point(32, 32), Qt.point(16, 32), Qt.point(0, 16)
            ]
        }
    ]
    Shape {
        id: shape
        visible: !next
        preferredRendererType: Shape.CurveRenderer
        ShapePath {
            strokeWidth: -1
            fillColor: block.borderColor//Qt.darker(block.fillColor, 1.2)
            Component.onCompleted: pathElements = path
        }
    }
    function value(): bool {
        return next ? next.value() : false
    }
    function copy(block): BooleanSocket {
        return Qt.createComponent("BooleanSocket.qml").createObject(null, { block: block })
    }
    function cppCode(): string {
        return next ? next.cppCode() : "false"
    }
}
