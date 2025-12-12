import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Shapes
import QtQuick.Layouts
import QtQuick.Effects
import App
import Components as C

Item {
    id: workspace
    property alias canvasScale: canvas.scale
    property alias movement: blocksPanel.movement
    property alias controls: blocksPanel.controls
    property alias sensors: blocksPanel.sensors
    property ListModel variables: ListModel { dynamicRoles: true }
    clip: true
    signal removeVariable(name: string)
    signal renameVariable(name: string, newName: string)
    onRemoveVariable: name => {
        for (let i = 0; i < variables.count; ++i)
        if (name === variables.get(i).name)
        variables.remove(i, 1)
    }
    function variableRenameRequest(name: string) {
        renameVariableMenu.name = name
        renameVariableMenu.open()
    }
    component ScratchPopup: Popup {
        id: scratchPopup
        parent: workspace
        anchors.centerIn: parent
        dim: true
        font.family: "Segoe UI"; font.pointSize: 12
        Overlay.modeless: Rectangle { color: "#30000000"; parent: scratchPopup.parent }
        background: Rectangle {
            radius: 8
        }
    }
    ScratchPopup {
        id: renameVariableMenu
        property string name
        onOpened: varName.forceActiveFocus()
        onClosed: varName.clear()
        ColumnLayout {
            spacing: 12
            anchors.fill: parent
            //anchors.margins: 16
            Text {
                text: "Переименовать переменную"
                font.family: "Segoe UI"; font.pointSize: 14; font.weight: Font.DemiBold
            }
            Text {
                text: "Новое имя переменной:"
                font: renameVariableMenu.font
            }
            TextField {
                id: varName
                font: renameVariableMenu.font
                Layout.fillWidth: true
            }
            Row {
                spacing: 12
                Layout.alignment: Qt.AlignRight
                Button { text: "Отмена"; font: renameVariableMenu.font; onClicked: renameVariableMenu.close() }
                Button { text: "ОК"; font: renameVariableMenu.font;
                    onClicked: {
                        const newName = varName.text.trim()
                        for (let i = 0; i < variables.count; ++i)
                            if (renameVariableMenu.name === variables.get(i).name)
                                variables.setProperty(i, "name", newName)
                        renameVariable(renameVariableMenu.name, newName)
                        renameVariableMenu.close()
                    }
                }
            }
        }
    }
    Item {
        id: dragHandler
        property Socket socket
        property StackBlock placeItem
        property StackBlock stackBlock
        anchors.fill: parent
        function createNewBlock(block) {
            var newBlock = Qt.createComponent("Blocks/" + block.objectName + ".qml").createObject(overlay)
            block.copyData(newBlock)
            block.dragTarget = newBlock
            newBlock.canvas = canvas
            newBlock.dragStart.connect(blockDragStart)
            newBlock.dragMove.connect(blockDragMove)
            newBlock.dropped.connect(blockDropped)
            newBlock.clicked.connect(toFrontPlane)
            //blockDragStart(newBlock)
        }
        function blockDragStart(block) {
            block.disconnect()
            block.parent = overlay
        }
        function blockDragMove(block, pos) {
            let inBlockPanel = pointContains(blocksPanel, pos)
            //console.log(inBlockPanel)
            if (inBlockPanel) {
                trashOverlay.opacity = 1
                //drag.source.cursorShape = Qt.ForbiddenCursor
            } else {
                trashOverlay.opacity = 0
                //drag.source.cursorShape = Qt.ClosedHandCursor
            }
            if (!inBlockPanel && pointContains(dragHandler, pos)) {
                let newSocket = bestMatch(block)
                if (newSocket) {
                    if (newSocket !== socket) {
                        if (socket) socket.setGlow(false)
                        socket = newSocket
                        toFrontPlane(socket.block)
                        if (socket.type === "stack") {
                            removePlaceItem()
                            placeItem = block.clone(canvas)
                            placeItem.z = -2
                            placeItem.state = "spot"
                            placeItem.connectTo(socket)
                        } else if (socket.type === "value" || socket.type === "bool") {
                            socket.setGlow(true)
                        }
                    }
                } else {
                    //--------------------------------------------
                    if (socket) {
                        socket.setGlow(false)
                        socket = null
                    }
                    if (block.type === "stack") {
                        let newStackBlock
                        let bottomBlock = block.lastInStack()
                        let distance = Infinity
                        for (let plugBlock of canvas.children) {
                            if (plugBlock.type !== "stack") continue
                            if (plugBlock === placeItem) plugBlock = placeItem.next
                            let dis = bottomBlock.bottomSocket.distance(plugBlock)
                            if (dis < 48 && dis < distance) {
                                distance = dis
                                newStackBlock = plugBlock
                            }
                        }
                        if (newStackBlock) {
                            if (newStackBlock === stackBlock ||
                                    (newStackBlock.previous && newStackBlock.previous.block === placeItem))
                                return
                            removePlaceItem()
                            stackBlock = newStackBlock
                            toFrontPlane(newStackBlock)
                            placeItem = block.clone(canvas)
                            placeItem.state = "spot"
                            placeItem.x = newStackBlock.x
                            placeItem.y = newStackBlock.y - placeItem.bottomSocket.bindY
                            //placeItem.z = newStackBlock.z
                            newStackBlock.connectTo(placeItem.bottomSocket)
                            return
                        }
                    }
                    //--------------------------------------------
                    removePlaceItem()
                    stackBlock = null
                }
            }
        }
        function blockDropped(block, pos) {
            if (pointContains(blocksPanel, pos)) {
                trashOverlay.opacity = 0
                block.destruct()
            } else {
                block.parent = canvas
                if (placeItem) {
                    socket = null
                    stackBlock = null
                    placeItem.replace(block)
                } else if (socket) {
                    if (socket.type === "value" || socket.type === "bool") {
                        block.connectTo(socket)
                    }
                }
                //workspace.adjustWidth(); workspace.adjustHeight()
            }
        }
        function bestMatch(plugBlock) {
            let distance = Infinity
            let newSocket = undefined
            function check(block) {
                for (let socket of block.sockets) {
                    if (socket.next) {
                        check(socket.next)
                    }
                    if ((plugBlock.type === "stack" && socket.type === "stack") ||
                            (plugBlock.type === "value" && socket.type === "value" && !socket.next) ||
                            (plugBlock.type === "bool" && socket.type === "bool" && !socket.next)) {
                        if (block === placeItem) continue;
                        let dis = socket.distance(plugBlock)
                        if (dis < 48 && dis < distance) {
                            distance = dis
                            newSocket = socket
                        }
                    }
                }
            }
            for (let block of canvas.children) check(block)
            return newSocket
        }
        function toFrontPlane(block) {
            block = block.rootBlock()
            var temp = block.parent
            block.parent = parent.parent
            block.parent = temp
        }
        function removePlaceItem() {
            if (placeItem) {
                if (stackBlock) {
                    stackBlock.disconnect()
                    stackBlock.parent = canvas
                    stackBlock = null
                }
                placeItem.remove()
            }
        }
        function pointContains(item: Item, pos: point): bool {
            var leftTop = item.mapToGlobal(0, 0)
            var bottomRight = item.mapToGlobal(item.width, item.height)
            return pos.x > leftTop.x && pos.x < bottomRight.x && pos.y > leftTop.y && pos.y < bottomRight.y
        }
    }
    C.ScrollArea {
        id: flickable
        contentX: 1000; contentY: 1000
        contentWidth: canvas.width * canvas.scale; contentHeight: canvas.height * canvas.scale
        anchors.fill: parent
        horizontalScrollBar.leftPadding: blocksPanel.width + 2
        Image {
            id: background_image
            property real side: 24 * canvas.scale
            property real offsetX: 0
            property real offsetY: 0
            width: flickable.width + side * 2; height: flickable.height + side * 2
            //x: -flickable.contentX % 24; y: -flickable.contentY % 24
            x: flickable.contentX - (flickable.contentX - offsetX) % side - side
            y: flickable.contentY - (flickable.contentY - offsetY) % side - side
            source: "qrc:/images/scratch_background.svg"
            smooth: false
            //mipmap: true
            fillMode: Image.Tile
            horizontalAlignment: Image.AlignLeft
            verticalAlignment: Image.AlignTop
            transformOrigin: Item.TopLeft
            sourceSize: Qt.size(side, side)
            //anchors.fill: parent
        }
        Item {
            id: canvas
            width: 5000; height: 5000
            transformOrigin: Item.TopLeft
        }
        //onWidthChanged: workspace.adjustWidth()
        //onHeightChanged: workspace.adjustHeight()
    }
    BlocksPanel {
        id: blocksPanel
        width: 375; height: flickable.height
        /*Component.onCompleted: {
            for (let child of content)
                if (child instanceof AbstractBlock)
                    child.dragStart.connect(dragHandler.createNewBlock)
        }*/
    }
    Rectangle {
        id: trashOverlay
        color: "#33000000"
        opacity: 0
        anchors.fill: blocksPanel
        Behavior on opacity { OpacityAnimator { duration: 300 } }
        Image {
            width: 64; height: 64
            source: "qrc:/images/trash_opened.svg"
            sourceSize: Qt.size(width, height)
            fillMode: Image.PreserveAspectFit
            mipmap: true
            verticalAlignment: Qt.AlignVCenter
            horizontalAlignment: Qt.AlignHCenter
            anchors.centerIn: parent
        }
    }
    MultiEffect {
        source: overlay
        scale: canvas.scale
        anchors.fill: overlay
        transformOrigin: Item.TopLeft
        blurEnabled: true
        blur: 1.0
        brightness: -1
    }
    Item {
        id: overlay
        x: -flickable.contentX; y: -flickable.contentY
        width: canvas.width; height: canvas.height
        transformOrigin: Item.TopLeft
        scale: canvas.scale
    }
    component ZoomButton: Button {
        id: zoomButton
        width: 40; height: 40
        icon.width: 16; icon.height: 16
        icon.color: "white"
        containmentMask: background
        background: Shape {
            containsMode: Shape.FillContains
            preferredRendererType: Shape.CurveRenderer
            ShapePath {
                strokeWidth: -1
                fillColor: zoomButton.pressed ? "#0F83D6" : zoomButton.hovered ? "#46BDFC" : "#1EAAFF"
                Behavior on fillColor { ColorAnimation { duration: 150 } }
                PathAngleArc {
                    centerX: width / 2; centerY: height / 2
                    radiusX: width / 2; radiusY: height / 2
                    startAngle: 0; sweepAngle: 360
                }
            }
        }
    }
    ZoomButton {
        id: zoomReset
        icon.source: "qrc:/images/zoom_reset.svg"
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        onClicked: { canvas.scale = 1 }
    }
    ZoomButton {
        id: zoomOut
        icon.source: "qrc:/images/zoom_out.svg"
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.bottom: zoomReset.top
        anchors.bottomMargin: 10
        onClicked: {
            canvas.scale *= 0.8
            flickable.returnToBounds()
        }
    }
    ZoomButton {
        id: zoomIn
        icon.source: "qrc:/images/zoom_in.svg"
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.bottom: zoomOut.top
        anchors.bottomMargin: 10
        onClicked: { canvas.scale *= 1.25 }
    }
    property real minX: 0
    /*function adjustWidth() {
        let w = flickable.width * 1.5
        if (canvas.children.length) {
            let min_x = Infinity, max_x = 0
            for (let child of canvas.children) {
                min_x = Math.min(min_x, child.x)
                max_x = Math.max(max_x, child.x + child.width)
            }
            let children_width = min_x !== Infinity && max_x !== 0 ? max_x - min_x : 0
            w = children_width < flickable.width / 2 ? flickable.width * 2 - children_width : flickable.width + children_width
            canvas.width = w
            let dx = (children_width < flickable.width / 2 ? flickable.width - children_width : flickable.width / 2) - min_x
            for (let child of canvas.children) child.x += dx
            background_image.offsetX += dx * canvas.scale
            flickable.contentX += dx * canvas.scale
        } else {
            canvas.width = w
        }
    }
    function adjustHeight() {
        let h = flickable.height * 1.5
        if (canvas.children.length) {
            let min_y = Infinity, max_y = 0
            for (let child of canvas.children) {
                min_y = Math.min(min_y, child.y)
                max_y = Math.max(max_y, child.y + child.height)
            }
            let children_height = min_y !== Infinity && max_y !== 0 ? max_y - min_y + 8 : 0
            let dy = (children_height < flickable.height / 2 ? flickable.height - children_height : flickable.height / 2) - min_y
            for (let child of canvas.children) child.y += dy
            h = children_height < flickable.height / 2 ? flickable.height * 2 - children_height : flickable.height + children_height
            background_image.offsetY += dy * canvas.scale
            flickable.contentY += dy * canvas.scale
        }
        canvas.height = h
    }*/
    function topLevelBlocks() {
        let blocks = []
        for (let child of canvas.children) {
            if (child instanceof StackBlock) {
                blocks.push(child)
            }
        }
        blocks.sort((a, b) => b.y - a.y)
        return blocks
    }
    function clear() {
        for (let child of canvas.children) child.destroy()
        blocksPanel.clear()
        variables.clear()
        //adjustWidth(); adjustHeight()
    }
    function save(filePath) {
        var json = {
            "app": "Scratch",
            "device": App.deviceName,
            "contentX": flickable.contentX,
            "contentY": flickable.contentY,
            "stacks": []
        }
        function getBlockJSON(block) {
            let blockJSON = { "name": block.objectName }
            if (block.getData && block.getData()) blockJSON.data = block.getData()
            blockJSON.plugs = []
            for (let i = 0; i < block.sockets.length; ++i) {
                if (block.sockets[i].next) {
                    let plug = getBlockJSON(block.sockets[i].next)
                    plug.index = i
                    blockJSON.plugs.push(plug)
                }
            }
            return blockJSON
        }
        for (let block of canvas.children) {
            let blockJSON = getBlockJSON(block)
            blockJSON.x = block.x; blockJSON.y = block.y
            json.stacks.push(blockJSON)
        }
        if (variables.count) {
            json.variables = []
            for (let i = 0; i < variables.count; ++i) {
                let variable = variables.get(i)
                json.variables.push({"name": variable.name, "value": variable.value})
            }
        }
        App.saveFile(JSON.stringify(json, null, 4), scratch.currentFilePath)
    }
    function open(data: string): bool {
        clear()
        //const json = App.readJSON(filePath)
        try {
            const json = JSON.parse(data)
            if (json.app !== "Scratch") {
                console.error("Неверный формат")
                return false
            } else if (json.device !== App.deviceName) {
                console.error("Этот файл предназначен для другого устройства")
                return false
            }
            flickable.contentX = json.contentX; flickable.contentY = json.contentY
            // заполнение переменными
            if (json.variables) {
                for (let variable of json.variables) {
                    blocksPanel.addVariable(variable.name, variable.value)
                }
            }
            // создание блоков
            function createBlock(blockJSON) {
                let block = Qt.createComponent("Blocks/" + blockJSON.name + ".qml").createObject(canvas, {"canvas": canvas})
                if (blockJSON.data) block.setData(blockJSON.data)
                // connect ------
                block.dragStart.connect(dragHandler.blockDragStart)
                block.dragMove.connect(dragHandler.blockDragMove)
                block.dropped.connect(dragHandler.blockDropped)
                block.clicked.connect(dragHandler.toFrontPlane)
                //---------------
                for (let plug of blockJSON.plugs) {
                    createBlock(plug).connectTo(block.sockets[plug.index])
                }
                return block
            }
            for (let blockJSON of json.stacks) {
                let block = createBlock(blockJSON);
                block.x = blockJSON.x; block.y = blockJSON.y
            }
            //adjustWidth(); adjustHeight()
            return true
        } catch (error) {
            console.error(error)
            return false
        }
    }
    function saveData(): string {
        var json = {
            "app": "Scratch",
            "device": App.deviceName,
            "contentX": flickable.contentX,
            "contentY": flickable.contentY,
            "stacks": []
        }
        function getBlockJSON(block) {
            let blockJSON = { "name": block.objectName }
            if (block.getData && block.getData()) blockJSON.data = block.getData()
            blockJSON.plugs = []
            for (let i = 0; i < block.sockets.length; ++i) {
                if (block.sockets[i].next) {
                    let plug = getBlockJSON(block.sockets[i].next)
                    plug.index = i
                    blockJSON.plugs.push(plug)
                }
            }
            return blockJSON
        }
        for (let block of canvas.children) {
            let blockJSON = getBlockJSON(block)
            blockJSON.x = block.x; blockJSON.y = block.y
            json.stacks.push(blockJSON)
        }
        if (variables.count) {
            json.variables = []
            for (let i = 0; i < variables.count; ++i) {
                let variable = variables.get(i)
                json.variables.push({"name": variable.name, "value": variable.value})
            }
        }
        return JSON.stringify(json, null, 4)
    }
}
