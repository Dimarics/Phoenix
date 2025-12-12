import Apps.Scratch
import App

StackBlock {
    id: root
    objectName: "../../../ScratchBlocks/Arm"
    ScratchText { text: "включить моторы" }
    function run() {
        App.device.protocol.arm()
        completed(next)
    }
}
