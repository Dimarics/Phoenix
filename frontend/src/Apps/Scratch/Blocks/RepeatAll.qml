import QtQuick
import QtQuick.Shapes
import ".."

RepeatBlock {
    id: repeatAll
    objectName: "RepeatAll"
    ScratchText { text: "повторять всегда" }
    function reset() { finished = false }
    function run() { completed(finished ? next : contentSocket.next) }
    function cppCode(): string { return "while (true) {\n" + contentSocket.cppCode() + "}\n" + bottomSocket.cppCode() }
    /*shapePath: ShapePath {
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

        PathLine { relativeX: 0; relativeY: contentHeight - 8 }
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

        PathLine { x: 4; relativeY: 0 }
        PathArc { x: 0; relativeY: -4; radiusX: 4; radiusY: 4 }
        PathLine { x: 0; y: 4 }
    }*/
}
