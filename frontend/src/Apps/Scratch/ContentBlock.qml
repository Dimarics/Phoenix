import QtQuick
import QtQuick.Shapes
import QtQuick.Layouts
import ".."

StackBlock {
    id: root
    readonly property real contentHeight: stackHeight(contentSocket)
    readonly property Socket contentSocket: _contentSocket
    property bool finished
    implicitHeight: headerHeight + contentHeight + 32
    borderColor: "#CF8B17"
    fillColor: "#FFAA19"
    data: Socket { id: _contentSocket; x: 40; y: headerHeight; block: root }
    shapePath: ShapePath {
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
}
