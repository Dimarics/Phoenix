import ".."
import App

ValueBlock {
    id: sonar
    objectName: "Sonar"
    fillColor: "#00C8A5"
    borderColor: "#009B80"
    property alias list: sonar_id.model
    ScratchText { text: "сонар" }
    ListItem { id: sonar_id; block: sonar }
    ScratchText { text: "расстояние до объекта в" }
    ListItem { id: units; block: sonar; model: ["миллиметрах", "сантиметрах", "дециметрах", "метрах"] }
    function value() {
        return App.device.data[2 + 5 * sonar_id.currentIndex] * Math.pow(0.1, units.currentIndex)
    }
    function cppCode(): string {
        return `getDistance(${sonar_id.currentIndex}, ${Math.pow(0.1, units.currentIndex)})`
    }
}
