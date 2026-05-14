import QtQuick
import QtQuick.Layouts
import QtQuick.Templates as T
import Components as C
import Hobots
import App

//pragma ComponentBehavior: Bound

C.AppWidget {
    id: root
    /*
    TextEdit {
        id: txt
        anchors.fill: root
        readOnly: false
        activeFocusOnPress: true
        text: "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        color: "white"
        Rectangle {
            anchors.fill: parent
            z: -1
            color: "black"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: txt.forceActiveFocus(Qt.MouseFocusReason)
        }
    }
    */
    /*
    enum Templates {
        Coordinates, Joints, Speed, Accel, Decel, Laser, Pomp, Grub, PWM//, Delay
    }
    property list<int> templates: [
        Script.Coordinates, Script.Joints, Script.Speed, Script.Laser,
        Script.Pomp, Script.Grub, Script.PWM//, Script.Delay
    ]
    */
    property list<var> templates
    defaultFilePath: `${App.tempLocation}${App.deviceName}_Script.txt`
    openNameFilters: [FilePicker.All]
    toolBar: C.ToolBar {
        onClear: textEdit.text = ""
        onStart: {
            scriptController.start()
            //scriptController.messageHandler()
        }
        onPause: {
            App.device.protocol.stay()
        }
        onResume: {
            scriptController.messageHandler(scriptController.currentCommand)
        }
        onStop: {
            scriptController.stop()
            App.device.protocol.stop()
        }
        Item { Layout.fillWidth: true }
        C.TabBox {
            id: languageBox
            model: ["C++", "Python"]
            onCurrentIndexChanged: scriptController.language = currentIndex ? ScriptController.Python : ScriptController.Cpp
            Component.onCompleted: scriptController.language = currentIndex ? ScriptController.Python : ScriptController.Cpp
        }
        Item { Layout.fillWidth: true }
    }
    /*
    function open(path: string): bool {
        const text = App.readFile(path);
        if (!text) return false
        textEdit.text = text; return true
    }
    function save(path: string) { App.saveFile(textEdit.text, path) }
    */
    function open(data: string): bool {
        textEdit.text = data
    }
    function saveData(): string {
        return textEdit.text
    }
    function save(path: string) {
        App.saveFile(textEdit.text, root.currentFilePath)
    }
    C.JsonSettings {
        categories: [App.deviceName, "Script"]
        path: App.appDataLocation + "settings.json"
        property alias language: languageBox.currentIndex
        property alias currentPath: root.currentFilePath
        onLoaded: {
            if (!root.open(App.readFile(root.currentFilePath)))
                root.currentFilePath = root.defaultFilePath
        }
    }
    /*C.ComboBox {
        id: languageBox
        //width: 250
        model: ["C++", "Python"]
        anchors {
            top: toolBar.bottom
            left: commandListHeader.left; leftMargin: 12
            right: commandListHeader.right; rightMargin: 12
        }
    }*/
    C.SplitView {
        width: parent.width
        anchors { top: toolBar.bottom; bottom: parent.bottom }
        Item {
            C.SplitView.preferredWidth: 250
            C.Text {
                id: commandListHeader
                width: parent.width
                text: "API"
                padding: 4
                horizontalAlignment: Text.AlignHCenter
                background: Rectangle { color: C.Style.tabColor }
                //anchors.topMargin: 12
            }
            ListView {
                id: commandList
                width: parent.width
                //padding: 12
                clip: true
                anchors { top: commandListHeader.bottom; bottom: parent.bottom }
                model: ListModel { }
                delegate: T.ItemDelegate {
                    id: delegate
                    //implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                    //                        implicitContentWidth + leftPadding + rightPadding)
                    //required property var model
                    required property int index
                    required property string tip
                    required property string template
                    implicitWidth: ListView.view.width
                    implicitHeight: contentItem.implicitHeight + topPadding + bottomPadding
                    leftPadding: 16; rightPadding: 16; topPadding: 4; bottomPadding: 4
                    contentItem: C.Text {
                        text: tip
                        font.pointSize: C.Style.textSize
                        scale: delegate.pressed ? 0.97 : 1
                        Behavior on scale { NumberAnimation { duration: 100 } }
                    }
                    background: Rectangle {
                        color: delegate.hovered ? C.Style.listViewBkgHighlightColor : "transparent"
                        border.color: "transparent"
                    }
                    onClicked: textEdit.insert(textEdit.cursorPosition, template)
                }
                Component.onCompleted: {
                    for (let template of root.templates) {
                        model.append(template)
                    }
                }
                /*
                Component.onCompleted: {
                    for (let template of App.device.availableFeatures) {
                        //console.log(typeof(template), typeof(C.HobotDevice.Coordinates))
                        switch (template) {
                        case C.HobotDevice.Coordinates:
                            model.append({"tip": "значение x", "template": "x()"})
                            model.append({"tip": "значение y", "template": "y()"})
                            model.append({"tip": "значение z", "template": "z()"})
                            model.append({"tip": "двигаться по x", "template": "moveX(value)"})
                            model.append({"tip": "двигаться по y", "template": "moveY(value)"})
                            model.append({"tip": "двигаться по z", "template": "moveZ(value)"})
                            model.append({"tip": "перейти в точку x, y", "template": "moveXY(x, y)"})
                            model.append({"tip": "перейти в точку x, y, z", "template": "moveXYZ(x, y, z)"})
                            break;
                        case C.HobotDevice.Joints:
                            model.append({"tip": "значение угла", "template": "joint(index)"})
                            model.append({"tip": "установить угол", "template": "setJoint(index, value)"})
                            break;
                        case C.HobotDevice.Speed:
                            model.append({"tip": "значение скорости", "template": "speed(значение)"})
                            model.append({"tip": "установить скорость", "template": "setSpeed(значение)"})
                            break;
                        case C.HobotDevice.Grub:
                            model.append({"tip": "захват", "template": "grub(0 - открыт, 1 - закрыт)"})
                            break;
                        case C.HobotDevice.Pomp:
                            model.append({"tip": "помпа", "template": "pomp(0 - выключена, 1 - активна)"})
                            break;
                        case C.HobotDevice.Laser:
                            model.append({"tip": "лазер", "template": "laser(0 - выключен, 1 - активен)"})
                            break;
                        case C.HobotDevice.LineSensor:
                            model.append({"tip": "датчик линии видит линию", "template": "lineSensor(port)"})
                            break;
                        case C.HobotDevice.ColorSensor:
                            model.append({"tip": "красный цвет", "template": "colorRed(sensor port)"})
                            model.append({"tip": "зелёный цвет", "template": "colorGreen(sensor port)"})
                            model.append({"tip": "синий цвет", "template": "colorBlue(sensor port)"})
                            break;
                        case C.HobotDevice.TouchSensor:
                            model.append({"tip": "состояние датчика нажатия", "template": "touchSensor(port)"})
                            break;
                        case C.HobotDevice.Sonar:
                            model.append({"tip": "расстояние до объекта", "template": "sonarDistance(port)"})
                            break;
                        case C.HobotDevice.PWM:
                            model.append({"tip": "ШИМ", "template": "pwm(pin, value)"})
                            break;
                        }
                    }
                    model.append({"tip": "ждать", "template": "delay(msecs)"})
                }
                */
            }
        }
        C.SplitView {
            orientation: Qt.Vertical
            //C.SplitView.fillWidth: true
            Rectangle {
                //id: background
                color: "white"
                C.SplitView.fillHeight: true
                //width: parent.width
                //anchors { left: lineCount.right; right: parent.right; top: toolBar.bottom; bottom: parent.bottom }
                Rectangle {
                    id: lineCount
                    color: "#EAEAEA"
                    width: lineCountText.width
                    clip: true
                    anchors { top: parent.top; bottom: parent.bottom }
                    Text {
                        id: lineCountText
                        color: "gray"
                        font: textEdit.font
                        y: -textEditScroller.contentY
                        width: Math.max(implicitWidth, 60)
                        horizontalAlignment: Qt.AlignRight
                        leftPadding: 16; rightPadding: 16
                        topPadding: textEdit.topPadding; bottomPadding: textEdit.bottomPadding
                        textFormat: Text.RichText
                        text: {
                            var numbers = ""
                            for (let i = 1; i <= textEdit.lineCount; ++i)
                                numbers += i === textEdit.currentLine ? "<b>" + i + "</b><br>" : i + "<br>"
                            return numbers
                        }
                    }
                }
                C.TextEditScroller {
                    id: textEditScroller
                    anchors { left: lineCount.right; right: parent.right; top: parent.top; bottom: parent.bottom }
                    textEdit: TextEdit {
                        id: textEdit
                        property int currentLine: 1
                        focus: true
                        color: "black"
                        selectionColor: C.Style.listViewBkgHighlightColor
                        font.pointSize: 14
                        font.family: "consolas"
                        padding: 6//; leftPadding: 6
                        //Keys.onTabPressed: insert(cursorPosition, "    ")
                        //width: implicitWidth; height: implicitHeight
                        /*
                        onCursorPositionChanged: {
                            var line = 1, pos = 0;
                            while ((pos = text.indexOf('\n', pos) + 1) > 0 && pos <= cursorPosition) {
                                line++;
                            }
                            currentLine = line;
                        }
                        */
                    }
                }
                /*C.Flickable {
                    id: flickable
                    clip: true
                    sliderColor: "gray"
                    contentWidth: textEdit.width; contentHeight: textEdit.height
                    TextEdit {
                        id: textEdit
                        property int currentLine: 1
                        focus: true
                        color: "black"
                        selectionColor: C.Style.textSelectionColor
                        font.pointSize: 14
                        font.family: "consolas"
                        padding: 10; leftPadding: 6
                        Keys.onTabPressed: insert(cursorPosition, "    ")
                        width: Math.max(implicitWidth, flickable.width); height: Math.max(implicitHeight, flickable.height)
                        onCursorPositionChanged: {
                            var line = 1, pos = 0;
                            while ((pos = text.indexOf('\n', pos) + 1) > 0 && pos <= cursorPosition) {
                                line++;
                            }
                            currentLine = line;
                            scrollToCursor()
                        }
                        function scrollToCursor() {
                            var speed = 500
                            var padding = 4
                            var viewRect = Qt.rect(flickable.contentX, flickable.contentY, flickable.width, flickable.height)

                            if (viewRect.left + padding > cursorRectangle.left) {
                                flickable.contentX += -viewRect.left + cursorRectangle.left - padding
                            } else if (viewRect.right - padding < cursorRectangle.right) {
                                flickable.contentX += -viewRect.right + cursorRectangle.right + padding
                            }

                            if (viewRect.top + padding > cursorRectangle.top) {
                                flickable.contentY += -viewRect.top + cursorRectangle.top - padding
                            } else if (viewRect.bottom - padding < cursorRectangle.bottom) {
                                flickable.contentY += -viewRect.bottom + cursorRectangle.bottom + padding
                            }
                        }
                    }
                    Behavior on contentX { NumberAnimation { duration: 100 } }
                    Behavior on contentY { NumberAnimation { duration: 100 } }
                    //onFlickEnded: textEdit.scrollToCursor()
                }*/
            }
            Rectangle {
                color: C.Style.backgroundColor_2
                C.SplitView.preferredHeight: 200
                C.TextEditScroller {
                    id: terminalScroller
                    anchors.fill: parent
                    textEdit: TextEdit {
                        id: terminal
                        readOnly: true
                        color: C.Style.textColor
                        font.pointSize: C.Style.textSize
                        font.family: "consolas"
                        padding: 6; topPadding: -font.pointSize
                        selectionColor: "#8000FF"//C.Style.textSelectionColor
                        //selectedTextColor: "black"
                    }
                }
            }
        }
    }
    Connections {
        target: App.device.protocol
        ignoreUnknownSignals: true
        function onReached() {
            if (toolBar.status === C.ToolBar.Running) scriptController.next()
        }
    }
    ScriptController {
        id: scriptController
        property string currentCommand
        textDocument: textEdit.textDocument
        terminalDocument: terminal.textDocument
        onCommandReceived: message => messageHandler(message)
        onStateChanged: toolBar.status = state
        //onFinished: toolBar.status = C.ToolBar.NotRunning
        function messageHandler(message) {
            /*
            currentCommand = message
            var args = message.split(' ')
            switch(args[0]) {
            case "moveX": App.device.protocol.moveToX(args[1]); break
            case "moveY": App.device.protocol.moveY(args[1]); break
            case "moveZ": App.device.protocol.moveZ(args[1]); break
            case "moveXY": App.device.protocol.moveXY(args[1], args[2]); break
            case "moveXYZ": App.device.protocol.moveXYZ(args[1], args[2], args[3]); break
            case "setJoint": App.device.protocol.setJoint(args[1], args[2]); break
            case "setSpeed": App.device.setSpeed(args[1]); break
            case "setAccel": App.device.setAccel(args[1]); break
            case "setDecel": App.device.setDecel(args[1]); break
            }
            switch(args[0]) {
            case "moveX": case "moveY": case "moveZ": case "moveXY": case "moveXYZ": case "setJoint": break
            default: next(); break
            }
            */
            currentCommand = message
            var args = message.split(' ')
            root.commandsHandler(args[0], args)
        }
    }
    Timer {
        id: delay_timer
        repeat: false
        onTriggered: next()
    }
    function commandsHandler(command, args) { }
    function next() { scriptController.next() }
    function delay(value) {
        delay_timer.interval = value
        delay_timer.restart()
    }
}
