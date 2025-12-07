import ".."
import App

BooleanBlock {
    id: root
    property alias list: sensor_id.model
    objectName: "LineSensor"
    fillColor: "#00C8A5" //"#00D7B0"
    borderColor: "#009B80"
    ScratchText { text: "датчик линии" }
    ListItem { id: sensor_id; block: root }
    ScratchText { text: "видит линию" }
    function value() { return App.device.lineSensor[line_id.currentIndex] }
}
