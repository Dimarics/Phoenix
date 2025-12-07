import ".."
import App

BooleanBlock {
    id: root
    property alias list: button_id.model
    objectName: "ButtonState"
    fillColor: "#00C8A5"
    borderColor: "#009B80"
    ScratchText { text: "состояние кнопки" }
    ListItem { id: button_id; block: root }
    function cppCode(): string {
        return `readButton(${button_id.currentIndex})`
    }
}
