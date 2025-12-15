import App
import Apps.Scratch

BooleanBlock {
    id: root
    objectName: "../../../ScratchBlocks/ArucoId"
    fillColor: "#00C8A5"
    borderColor: "#009B80"
    ScratchText { text: "маркер №" }
    ListItem { id: marker_ids; block: root; model: [1, 2, 3, 4, 5, 6, 7, 8, 9, 10] }
    ScratchText { text: "виден" }
    function value() {
        for (let id of App.device.arucoIds) {
            if (id === marker_ids.currentIndex + 1) {
                return true
            }
        }
        return false
    }
}
