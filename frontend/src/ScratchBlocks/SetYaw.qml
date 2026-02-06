import Apps.Scratch
import App

StackBlock {
    id: root
    objectName: "../../../ScratchBlocks/SetYaw"
    ScratchText { text: "задать рысканье" }
    ValueInput { id: value; block: root; state: "double"; min: -180; max: 180 }
    function run() {
        App.device.protocol.setYaw(value.value() * Math.PI / 180)
        completed(next)
    }
}
