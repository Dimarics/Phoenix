import Apps.Scratch
import App
import QtQuick

StackBlock {
    id: root
    objectName: "../../../ScratchBlocks/Land"
    ScratchText { text: "посадка" }
    function run() {
        App.device.protocol.land()
        completed(next)
    }
}
