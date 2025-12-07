import QtQuick

QtObject {
    //property real jearRatio: 1
    property bool inverted: false
    property real value: 0
    property real rotation: 0
    //property real angleDegrees: value * 360 / Math.PI
    property real offset
    property real userValue: toUserValue(value)
    property real min: -180
    property real max: 180
    readonly property real rmin: fromUserValue(inverted ? max : min)
    readonly property real rmax: fromUserValue(inverted ? min : max)
    signal rotated(value: real)
    onValueChanged: userValue = toUserValue(value)
    onUserValueChanged: value = fromUserValue(userValue)
    onRminChanged: if (value < rmin) value = rmin
    onRmaxChanged: if (value > rmax) value = rmax
    /*onValueChanged: {
        console.log("value:", value)
        userValue = Qt.binding(()=>toUserValue(value))
    }
    onUserValueChanged: {
        console.log("userValue:", userValue)
        value = Qt.binding(()=>fromUserValue(userValue))
    }*/
    function setValue(angle: real) {
        var newValue = angle
        if (newValue < rmin) newValue = rmin
        if (newValue > rmax) newValue = rmax
        value = newValue
    }
    function rotate(angle: real) {
        var newValue = value + angle
        if (newValue < rmin) newValue = rmin
        if (newValue > rmax) newValue = rmax
        rotated(newValue - value)
        value = newValue
    }
    function fromUserValue(userValue: real): real {
        const value = (userValue - offset) * Math.PI / 180
        return inverted ? -value : value
    }
    function toUserValue(value: real): real {
        const userValue = inverted ? -value : value
        return userValue * 180 / Math.PI + offset
    }
}
