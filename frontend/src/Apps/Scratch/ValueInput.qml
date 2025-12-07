import QtQuick
import QtQuick.Shapes
import QtQuick.Controls.Basic
//import QtQuick.Layouts

Socket {
    id: root
    //property int valueType: ValueInput.Float
    property alias text: textInput.text
    property Item focusEffectObject
    //property var value: next ? next.value() : textInput.text
    type: "value"
    visible: true
    opacity: !next
    offsetX: 32; offsetY: height / 2
    implicitWidth: Math.max(40, next ? next.width : textInput.implicitWidth)
    implicitHeight: Math.max(32, next ? next.height : textInput.implicitHeight)
    //Layout.preferredWidth: next ? next.width : textInput.implicitWidth
    //Layout.preferredHeight: next ? next.height : textInput.implicitHeight
    //Layout.minimumWidth: 40; Layout.minimumHeight: 32;
    path: [
        PathMove { x: height / 2 },
        PathLine { relativeX: width - height },
        PathArc { relativeX: 0; relativeY: height; radiusX: height / 2; radiusY: height / 2 },
        PathLine { relativeX: -width + height; relativeY: 0 },
        PathArc { relativeX: 0; relativeY: -height; radiusX: height / 2; radiusY: height / 2 }
    ]
    Component { id: intValidator; IntValidator{} }
    Component { id: doubleValidator; DoubleValidator { locale: 'C'; notation: DoubleValidator.StandardNotation } }
    state: "double"
    states: [
        State {
            name: "int"
            PropertyChanges {
                target: root
                //value: parseInt(next ? next.value() : textInput.text)
                //function value(): int { return parseInt(next ? next.value() : textInput.text) }
            }
            PropertyChanges {
                target: textInput
                validator: intValidator.createObject()
            }
        },
        State {
            name: "double"
            PropertyChanges {
                target: root
                //value: parseFloat(next ? next.value() : textInput.text)
                //function value(): floa { return parseFloat(next ? next.value() : textInput.text) }
            }
            PropertyChanges {
                target: textInput
                validator: doubleValidator.createObject()
            }
        }
    ]
    Component {
        id: focusEffect
        Rectangle {
            id: focusEffectSource
            z: -1
            radius: height / 2
            color: "#4DFFFFFF"
            anchors.fill: parent
            anchors.margins: -4
            NumberAnimation {
                target: focusEffectSource
                property: "scale"
                from: 0.8; to: 1; duration: 120
                running: true
            }
            NumberAnimation {
                target: focusEffectSource
                property: "scale"
                from: 1; to: 0.8; duration: 120
                running: !textInput.activeFocus
                onFinished: target.destroy()
            }
        }
    }
    TextField {
        visible: !next
        id: textInput
        //text: "0"
        color: "#555F73"
        selectedTextColor: "white"
        font.family: "Segoe UI"; font.pointSize: 14; font.weight: 600
        horizontalAlignment: TextField.AlignHCenter
        verticalAlignment: TextField.AlignVCenter
        topPadding: 0; bottomPadding: 0
        leftPadding: 12; rightPadding: 12
        anchors.fill: root
        containmentMask:  Shape {
            containsMode: Shape.FillContains; ShapePath { Component.onCompleted: pathElements = path }
        }
        background: Rectangle {
            radius: height / 2
            border.color: block.borderColor
        }
        onActiveFocusChanged: {
            if (activeFocus) {
                selectAll();
                if (!focusEffectObject) focusEffectObject = focusEffect.createObject(root)
            }
        }
        Keys.onReturnPressed: focus = false
    }
    function value() {
        var value = next ? next.value() : textInput.text
        switch (state) {
        case "int": value = parseInt(value) || 0; break;
        case "double": value = parseFloat(value) || 0; break;
        }
        return value
    }
    function copy(block) {
        var value_socket = Qt.createComponent("ValueInput.qml").createObject(block, { block: block })
        value_socket.text = textInput.text
        return value_socket
    }
    function cppCode(): string {
        return next ? next.cppCode() : value()
    }
}
