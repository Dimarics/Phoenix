import ".."

StackBlock {
    id: root
    objectName: "DigitalWrite"
    borderColor: "#774DCB"
    fillColor: "#9966FF"
    ScratchText { text: "цифровой выход" }
    ValueInput { id: pin; state: ""; block: root }
    ScratchText { text: "значение" }
    ValueInput { id: value; block: root }
    function cppSetup(): string {
        return "pinMode(" + pin.value() + ", OUTPUT);\n"
    }
    function cppCode(): string {
        return "digitalWrite(%1, %2);\n".arg(pin.value()).arg(value.value()) + bottomSocket.cppCode()
    }
}
