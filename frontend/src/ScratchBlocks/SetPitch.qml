import Apps.Scratch
import App

StackBlock {
    id: root
    objectName: "../../../ScratchBlocks/SetPitch"
    ScratchText { text: "задать тангаж" }
    ValueInput { id: value; block: root; state: "double"; min: -100; max: 100 }
    function run() {
        App.device.protocol.setPitch(value.value())
        completed(next)
    }
}
