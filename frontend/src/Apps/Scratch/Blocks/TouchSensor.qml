import ".."
import App

ValueBlock {
    id: root
    property alias list: sensor_id.model
    objectName: "TouchSensor"
    fillColor: "#00C8A5"
    borderColor: "#009B80"
    ScratchText { text: "состояние датчика нажатия" }
    ListItem { id: sensor_id; block: root }
    function value() {
        return App.device.data[3 + 5 * sensor_id.currentIndex]
    }
    function cppCode(): string {
        return `touchSensor(${sensor_id.currentIndex})`
    }
}
