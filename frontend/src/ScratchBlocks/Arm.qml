import Apps.Scratch
import App
import QtQuick

StackBlock {
    id: root
    objectName: "../../../ScratchBlocks/Arm"
    ScratchText { text: "включить моторы" }
    Timer { id: timer; interval: 500; onTriggered: completed(next) }
    function run() {
        App.device.protocol.armDisarm(true)
        timer.restart()
    }
}
