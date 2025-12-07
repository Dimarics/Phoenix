import QtQuick
import QtQuick.Shapes
import QtQuick.Layouts
import QtQuick.Effects

BasicBlock {
    id: root
    default property alias content: layout.data
    property alias headerHeight: layout.height
    property alias next: bottomSocket.next
    property alias glow: glowLoader.active
    property Socket bottomSocket: bottomSocket
    signal completed(StackBlock next)
    objectName: "Stack"
    type: "stack"
    plugX: 24
    z: previous && previous.block.glow ? -2 : 0
    //width: implicitWidth; height: implicitHeight
    implicitWidth: layout.implicitWidth + layout.anchors.leftMargin + layout.anchors.rightMargin
    implicitHeight: layout.height
    borderColor: "#3373CC"
    fillColor: "#4C97FF"
    states: State {
        name: "spot"
        PropertyChanges {
            target: root
            //strokeWidth: -1
            borderColor: "transparent"
            fillColor: "#33000000" //204
        }
        PropertyChanges {
            target: layout
            visible: false
        }
    }
    shapePath: ShapePath {
        // 11.02944 = (16 - 2.48528 * 2)
        // 4.24264 = (2.48528 + 1.75736)
        // 4.48528 = (8 - 1.75736 * 2)
        // 9.51472 = (16 - 4 - 2.48528)
        // 50.48528 = (48 + 2.48528)
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
        PathLine { relativeX: 0; relativeY: layout.height - 8 }
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
    RowLayout {
        id: layout
        height: Math.max(48, implicitHeight + 8)
        spacing: 8
        anchors { left: parent.left; right: parent.right; leftMargin: 8; rightMargin: 8 }
        //children: content
    }
    Socket { id: bottomSocket; x: 24; y: root.height }
    Loader {
        z: -1
        id: glowLoader
        active: false
        sourceComponent: MultiEffect {
            source: Shape {
                preferredRendererType: Shape.CurveRenderer
                ShapePath {
                    strokeWidth: 16
                    strokeColor: "#FFFF00"
                    startX: shapePath.startX; startY: shapePath.startY
                    Component.onCompleted: pathElements = shapePath.pathElements
                }
            }
            width: source.width; height: source.height
            paddingRect: Qt.rect(8, 8, 0, 0)
            blurEnabled: true
            blur: 1.0
            blurMax: 10
        }
    }
    function connectTo(socket: Socket) {
        disconnect()
        if (socket.next) {
            let lastBlock = lastInStack()
            lastBlock.next = socket.next
            socket.next.previous = lastBlock.bottomSocket
        }
        socket.next = this
        previous = socket
    }
    function replace(block: StackBlock) {
        if (previous) {
            previous.next = block
            block.previous = previous
        }
        if (bottomSocket.next) {
            let lastSocket = block.lastInStack().bottomSocket
            lastSocket.next = next
            next.previous = lastSocket
        }
        destroy()
    }
    function remove() {
        if (previous) {
            previous.next = next
        }
        if (next) {
            next.previous = previous
        }
        destroy()
    }
    function lastInStack(): StackBlock {
        for (var block = this; block.next; block = block.next);
        return block
    }
    function stackHeight(socket: Socket): real {
        for (var height = 0, block = socket.next; block; height += block.height, block = block.next);
        return Math.max(24, height)
    }
}
