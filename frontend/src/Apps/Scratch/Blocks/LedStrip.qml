import ".."
import App

StackBlock {
    id: root
    objectName: "LedStrip"
    borderColor: "#774DCB"
    fillColor: "#9966FF"
    ScratchText { text: "светодиод с адресом" }
    ValueInput { id: address; text: "0"; state: "int"; block: root }
    ScratchText { text: "установить цвет - красный" }
    ValueInput { id: red; text: "50"; state: "int"; block: root }
    ScratchText { text: "зелёный" }
    ValueInput { id: green; text: "50"; state: "int"; block: root }
    ScratchText { text: "синий" }
    ValueInput { id: blue; text: "50"; state: "int"; block: root }
    function cppCode(): string {
        return `setColor(${address.cppCode()}, ${red.cppCode()}, ${green.cppCode()}, ${blue.cppCode()});\n${bottomSocket.cppCode()}`
    }
}
