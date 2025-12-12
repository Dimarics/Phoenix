import Apps.Scratch
import App

StackBlock {
    id: root
    objectName: "../../../ScratchBlocks/SetYaw"
    ScratchText { text: "задать рысканье" }
    ValueInput { id: value; block: root; min: -100; max: 100 }
    function run() {
        App.device.protocol.setYaw(value.value())
        completed(next)
    }
}
