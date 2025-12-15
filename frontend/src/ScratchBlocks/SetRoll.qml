import Apps.Scratch
import App

StackBlock {
    id: root
    objectName: "../../../ScratchBlocks/SetRoll"
    ScratchText { text: "задать крен" }
    ValueInput { id: value; block: root; state: "double"; min: -100; max: 100 }
    function run() {
        App.device.protocol.setRoll(value.value())
        completed(next)
    }
}
