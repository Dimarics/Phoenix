import QtQuick
import QtQuick.Shapes
//import QtQuick.Layouts
import ".."

ContentBlock {
    id: root
    readonly property real ifContentHeight: stackHeight(contentSocket)
    readonly property real elseContentHeight: stackHeight(elseSocket)
    objectName: "IfElse"
    implicitHeight: headerHeight + ifContentHeight + 32 + elseContentHeight + 32
    ScratchText { text: "если" }
    BooleanSocket { id: condition; block: root }
    ScratchText { text: "то" }
    data: [
        Socket { id: elseSocket; x: 40; y: headerHeight + ifContentHeight + 32; block: root },
        ScratchText { x: 8; y: headerHeight + ifContentHeight + 16 - implicitHeight / 2; text: "иначе" }
    ]
    shapePath: ShapePath {
        id: shapePath
        //strokeWidth: 0;
        startX: 0; startY: 4
        PathArc { x: 4; y: 0; radiusX: 4; radiusY: 4 }
        PathLine { relativeX: 9.51472; relativeY: 0 }
        PathArc { relativeX: 4.24264; relativeY: 1.4645; radiusX: 6; radiusY: 6 }
        PathLine { relativeX: 4.48528; relativeY: 4.48528 }
        PathArc { relativeX: 4.24264; relativeY: 1.4645; radiusX: 6; radiusY: 6; direction: PathArc.Counterclockwise }
        PathLine { relativeX: 11.02944; relativeY: 0 }
        PathArc { relativeX: 4.24264; relativeY: -1.4645; radiusX: 6; radiusY: 6; direction: PathArc.Counterclockwise }
        PathLine { relativeX: 4.48528; relativeY: -4.48528 }
        PathArc { relativeX: 4.24264; relativeY: -1.4645; radiusX: 6; radiusY: 6 }
        PathLine { x: width - 4; relativeY: 0 }
        PathArc { relativeX: 4; relativeY: 4; radiusX: 4; radiusY: 4 }
        PathLine { relativeX: 0; relativeY: headerHeight - 8 }
        PathArc { relativeX: -4; relativeY: 4; radiusX: 4; radiusY: 4 }
        PathLine { x: 50.48528 + 16; relativeY: 0 }
        PathArc { relativeX: -4.24264; relativeY: 1.4644; radiusX: 6; radiusY: 6; direction: PathArc.Counterclockwise }
        PathLine { relativeX: -4.48528; relativeY: 4.48528 }
        PathArc { relativeX: -4.24264; relativeY: 1.4645; radiusX: 6; radiusY: 6}
        PathLine { relativeX: -11.02944; relativeY: 0 }
        PathArc { relativeX: -4.24264; relativeY: -1.4645; radiusX: 6; radiusY: 6 }
        PathLine { relativeX: -4.48528; relativeY: -4.48528 }
        PathArc { relativeX: -4.24264; relativeY: -1.4644; radiusX: 6; radiusY: 6; direction: PathArc.Counterclockwise}
        PathLine { x: 4 + 16; relativeY: 0 }
        PathArc { x: 16; relativeY: 4; radiusX: 4; radiusY: 4; direction: PathArc.Counterclockwise }

        PathLine { x: 16; relativeY: ifContentHeight - 8 }
        PathArc { x: 4 + 16; relativeY: 4; radiusX: 4; radiusY: 4; direction: PathArc.Counterclockwise }
        PathLine { relativeX: 9.51472; relativeY: 0 }
        PathArc { relativeX: 4.24264; relativeY: 1.4645; radiusX: 6; radiusY: 6 }
        PathLine { relativeX: 4.48528; relativeY: 4.48528 }
        PathArc { relativeX: 4.24264; relativeY: 1.4645; radiusX: 6; radiusY: 6; direction: PathArc.Counterclockwise }
        PathLine { relativeX: 11.02944; relativeY: 0 }
        PathArc { relativeX: 4.24264; relativeY: -1.4645; radiusX: 6; radiusY: 6; direction: PathArc.Counterclockwise }
        PathLine { relativeX: 4.48528; relativeY: -4.48528 }
        PathArc { relativeX: 4.24264; relativeY: -1.4645; radiusX: 6; radiusY: 6 }
        PathLine { x: width - 4; relativeY: 0 }
        PathArc { relativeX: 4; relativeY: 4; radiusX: 4; radiusY: 4 }
        PathLine { relativeX: 0; relativeY: 32 - 8 }
        PathArc { relativeX: -4; relativeY: 4; radiusX: 4; radiusY: 4 }
        PathLine { x: 50.48528 + 16; relativeY: 0 }
        PathArc { relativeX: -4.24264; relativeY: 1.4644; radiusX: 6; radiusY: 6; direction: PathArc.Counterclockwise }
        PathLine { relativeX: -4.48528; relativeY: 4.48528 }
        PathArc { relativeX: -4.24264; relativeY: 1.4645; radiusX: 6; radiusY: 6}
        PathLine { relativeX: -11.02944; relativeY: 0 }
        PathArc { relativeX: -4.24264; relativeY: -1.4645; radiusX: 6; radiusY: 6 }
        PathLine { relativeX: -4.48528; relativeY: -4.48528 }
        PathArc { relativeX: -4.24264; relativeY: -1.4644; radiusX: 6; radiusY: 6; direction: PathArc.Counterclockwise}
        PathLine { x: 4 + 16; relativeY: 0 }
        PathArc { x: 16; relativeY: 4; radiusX: 4; radiusY: 4; direction: PathArc.Counterclockwise }

        PathLine { relativeX: 0; relativeY: elseContentHeight - 8 }
        PathArc { relativeX: 4; relativeY: 4; radiusX: 4; radiusY: 4; direction: PathArc.Counterclockwise }
        PathLine { relativeX: 9.51472; relativeY: 0 }
        PathArc { relativeX: 4.24264; relativeY: 1.4645; radiusX: 6; radiusY: 6 }
        PathLine { relativeX: 4.48528; relativeY: 4.48528 }
        PathArc { relativeX: 4.24264; relativeY: 1.4645; radiusX: 6; radiusY: 6; direction: PathArc.Counterclockwise }
        PathLine { relativeX: 11.02944; relativeY: 0 }
        PathArc { relativeX: 4.24264; relativeY: -1.4645; radiusX: 6; radiusY: 6; direction: PathArc.Counterclockwise }
        PathLine { relativeX: 4.48528; relativeY: -4.48528 }
        PathArc { relativeX: 4.24264; relativeY: -1.4645; radiusX: 6; radiusY: 6 }
        PathLine { x: width - 4; relativeY: 0 }
        PathArc { relativeX: 4; relativeY: 4; radiusX: 4; radiusY: 4 }
        PathLine { relativeX: 0; relativeY: 32 - 8 }
        PathArc { relativeX: -4; relativeY: 4; radiusX: 4; radiusY: 4 }
        PathLine { x: 50.48528; relativeY: 0 }
        PathArc { relativeX: -4.24264; relativeY: 1.4644; radiusX: 6; radiusY: 6; direction: PathArc.Counterclockwise }
        PathLine { relativeX: -4.48528; relativeY: 4.48528 }
        PathArc { relativeX: -4.24264; relativeY: 1.4645; radiusX: 6; radiusY: 6}
        PathLine { relativeX: -11.02944; relativeY: 0 }
        PathArc { relativeX: -4.24264; relativeY: -1.4645; radiusX: 6; radiusY: 6 }
        PathLine { relativeX: -4.48528; relativeY: -4.48528 }
        PathArc { relativeX: -4.24264; relativeY: -1.4644; radiusX: 6; radiusY: 6; direction: PathArc.Counterclockwise}
        PathLine { relativeX: -9.51472; relativeY: 0 }
        PathArc { x: 0; relativeY: -4; radiusX: 4; radiusY: 4 }
        PathLine { x: 0; y: 4 }
    }
    /*function copy(parent: Item): AbstractBlock {
        var newBlock = Qt.createComponent("IfElseBlock.qml").createObject(parent ?? root.parent)
        return newBlock
    }*/
    function reset() { finished = false }
    function run() {
        if (finished) { completed(next); return }
        finished = true
        if (condition.value()) {
            completed(contentSocket.next)
        } else {
            completed(elseSocket.next)
        }
    }
    function cppCode(): string {
        return "if (" + condition.cppCode() + ") {\n" +
                contentSocket.cppCode() + "} else {\n" +
                elseSocket.cppCode() + "}\n" + bottomSocket.cppCode()
    }
}
