import ".."
import App

ValueBlock {
    id: root
    objectName: "AnalogRead"
    borderColor: "#774DCB"
    fillColor: "#9966FF"
    ScratchText { text: "аналоговый вход" }
    ValueInput { id: pin; state: ""; block: root }
    function cppCode(): string {
        return "analogRead(" + pin.cppCode() + ")" + bottomSocket.cppCode()
    }
}
