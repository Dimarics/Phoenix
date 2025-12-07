import ".."
import App

BooleanBlock {
    id: root
    objectName: "DigitalRead"
    borderColor: "#774DCB"
    fillColor: "#9966FF"
    ScratchText { text: "цифровой вход" }
    ValueInput { id: pin; state: ""; block: root }
    function cppSetup(): string {
        return "pinMode(" + pin.value() + ", INPUT);\n"
    }
    function cppCode(): string {
        return "digitalRead(" + pin.value() + ")" + bottomSocket.cppCode()
    }
}
