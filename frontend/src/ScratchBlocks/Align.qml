import Apps.Scratch
import App

StackBlock {
    id: root
    objectName: "../../../ScratchBlocks/Align"
    ScratchText { text: "выровняться" }
    function run() {
        App.device.protocol.align()
        completed(next)
    }
}
