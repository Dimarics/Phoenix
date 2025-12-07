import ".."
import App

StackBlock {
    id: root
    property alias list: sensor_id.model
    objectName: "ColorSensorBrightness"
    fillColor: "#00C8A5"
    borderColor: "#009B80"
    ScratchText { text: "датчик цвета" }
    ListItem { id: sensor_id; block: root }
    ScratchText { text: "яркость" }
    ValueInput { id: brightness; text: "255"; block: root }
    function cppCode(): string {
        return `setColorSensorBrightness(${sensor_id.currentIndex}, ${brightness.cppCode()});\n${bottomSocket.cppCode()}`
    }
}
