import ".."
import App

ValueBlock {
    id: root
    property alias list: sensor_id.model
    objectName: "ColorSensor"
    fillColor: "#00C8A5" //"#00D7B0"
    borderColor: "#009B80" //"#00AC8D"
    ScratchText { text: "датчик цвета" }
    ListItem { id: sensor_id; block: root }
    ScratchText { text: "получить значение" }
    ListItem { id: color_name; block: root; model: ["красный", "зелёный", "синий", "освящённость", "отражённость"] }
    function value() { return App.device.colorSensor[sensor_id.currentIndex] }
    function cppCode() {
        switch(color_name.currentText) {
        case "красный":
            return `getRed(${sensor_id.currentIndex})`
        case "зелёный":
            return `getGreen(${sensor_id.currentIndex})`
        case "синий":
            return `getBlue(${sensor_id.currentIndex})`
        case "освящённость":
            return `getIlluminance(${sensor_id.currentIndex})`
        case "отражённость":
            return `getReflection(${sensor_id.currentIndex})`
        }
    }
}
