import ".."
import App

BooleanBlock {
    id: root
    objectName: "SonarDetectionMode"
    fillColor: "#00C8A5"
    borderColor: "#009B80"
    property alias list: sensor_id.model
    ScratchText { text: "сонар" }
    ListItem { id: sensor_id; block: root }
    ScratchText { text: "объект" }
    ListItem { id: condition; block: root; model: ["ближе", "дальше", "точно в"] }
    ValueInput { id: value; block: root; text: "50" }
    ListItem { id: units; block: root; model: ["миллиметров", "сантиметров", "дециметров", "метров"] }
    //function value() {
    //    return App.device.data[2 + 5 * sonar_id.currentIndex] * Math.pow(0.1, units.currentIndex)
    //}
    function cppCode(): string {
        var c
        switch(condition.currentText) {
        case "ближе": c = '<'; break;
        case "дальше": c = '>'; break;
        case "точно в": c = '=='; break;
        }
        return `(getDistance(${sensor_id.currentIndex}, ${Math.pow(0.1, units.currentIndex)}) ${c} ${value.cppCode()})`
    }
}
