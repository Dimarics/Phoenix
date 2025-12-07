import QtQuick
import QtQuick.Templates

TextField {
    id: root
    property string locale: 'C'
    property int decimals: 1
    property real min: 0
    property real max: 0
    property real value: 0
    signal valueModified
    text: "0"
    implicitWidth: 85; implicitHeight: 25
    color: Style.highlightTextColor
    selectionColor: Style.textSelectionColor
    font.pointSize: 11
    topPadding: 3
    verticalAlignment: TextInput.AlignVCenter
    horizontalAlignment: Text.AlignHCenter
    //clip: true
    //validator: DoubleValidator { id: validator; locale: root.locale; decimals: 2; notation: DoubleValidator.StandardNotation }
    validator: RegularExpressionValidator {
        property int numDigits: parseInt(Math.max(Math.abs(min), Math.abs(max))).toString().length
        regularExpression: new RegExp(decimals ? `^-?\\d{1,${numDigits}}[.,]\\d{0,${decimals}}$` : `^-?\\d{1,${numDigits}}$`)
    }
    background: Rectangle {
        z: -1
        radius: height / 2
        color: Style.darkWidgetColor
        border.color: root.hovered || root.activeFocus? Style.lightBorderColor : Style.brightBorderColor
        anchors.fill: parent
    }
    Keys.onReturnPressed: {
        focus = false
        valueModified()
    }
    onFocusChanged: {
        if (!focus) {
            let newValue = parseFloat(text)
            if (newValue < min) newValue = min
            if (newValue > max) newValue = max
            value = newValue
        }
    }
    onValueChanged: {
        if (value === undefined || isNaN(value)) {
            clear()
        } else {
            text = value.toFixed(decimals)
        }
    }
    onTextChanged: {
        //const regexp = /^\d{1,3}$|^-?\d{1,3}[.,]\d{0,2}$/
        //if (!regexp.test(text))
        text = text.replace(',', '.')
    }
    function setValue(value) { if (!activeFocus) text = value.toFixed(decimals) }
}
