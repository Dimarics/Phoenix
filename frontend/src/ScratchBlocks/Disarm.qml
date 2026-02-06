import Apps.Scratch
import App

StackBlock {
    id: root
    objectName: "../../../ScratchBlocks/Disarm"
    ScratchText { text: "выключить моторы" }
    function run() {
        App.device.protocol.armDisarm(false)
        completed(next)
    }
}
