import App
import Apps.Scratch

ValueBlock {
    id: root
    objectName: "../../../ScratchBlocks/PhxSonarDistance"
    fillColor: "#00C8A5"
    borderColor: "#009B80"
    ScratchText { text: "сонар: расстояние до объекта в" }
    ListItem { id: units; block: root; model: ["миллиметрах", "сантиметрах", "дециметрах", "метрах"] }
    function value() {
        return App.device.sonarDistance * Math.pow(0.1, units.currentIndex)
    }
}
