import ".."
import App

StackBlock {
    id: root
    objectName: "PinMode"
    borderColor: "#774DCB"
    fillColor: "#9966FF"
    ScratchText { text: "настроить пин" }
    ValueInput { id: pin; state: ""; block: root }
    ScratchText { text: "как" }
    ListItem {
        id: mode; block: root
        model: [ "выход", "вход", "вход с подтяжкой к плюсу", "вход с подтяжкой к минусу" ]
    }
    function cppCode(): string {
        var modeCode
        switch (mode.currentText) {
        case "выход": modeCode = "OUTPUT"; break;
        case "вход": modeCode = "INPUT"; break;
        case "вход с подтяжкой к плюсу": modeCode = "INPUT_PULLUP"; break;
        case "вход с подтяжкой к минусу": modeCode = "INPUT_PULLDOWN"; break;
        }
        return "pinMode(%1, %2);\n".arg(pin.value()).arg(modeCode) + bottomSocket.cppCode()
    }
}
