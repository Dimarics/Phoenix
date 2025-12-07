import ".."

StackBlock {
    id: root
    objectName: "AnalogWrite"
    borderColor: "#774DCB"
    fillColor: "#9966FF"
    ScratchText { text: "аналоговый выход" }
    ValueInput { id: pin; state: ""; block: root }
    ScratchText { text: "значение" }
    ValueInput { id: value; state: "int"; block: root }
    function cppCode(): string {
        return "analogWrite(%1, %2);\n".arg(pin.cppCode()).arg(value.cppCode()) + bottomSocket.cppCode()
    }
}
