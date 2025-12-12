import Apps.Scratch
import App

StackBlock {
    id: root
    objectName: "../../../ScratchBlocks/SetThrottle"
    ScratchText { text: "установить тягу" }
    ValueInput { id: value; text: "10"; block: root; min: 0; max: 100 }
    function run() {
        App.device.protocol.setThrottle(value.value())
        completed(next)
    }
}
