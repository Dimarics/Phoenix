import App
import Apps.Scratch

BooleanBlock {
    id: root
    objectName: "../../../ScratchBlocks/PhxButtonState"
    fillColor: "#00C8A5"
    borderColor: "#009B80"
    ScratchText { text: "если кнопка нажата" }
    function value() {
        return App.device.buttonState
    }
}
