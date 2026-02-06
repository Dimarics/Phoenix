import Apps.Scratch
import App
import QtQuick

StackBlock {
    id: root
    objectName: "../../../ScratchBlocks/Takeoff"
    ScratchText { text: "взлёт на" }
    ValueInput { id: value; text: "1000"; block: root; state: "double"; min: 0; max: 10000 }
    ScratchText { text: "мм" }
    function run() {
        App.device.protocol.takeoff(value.value() / 1000)
        completed(next)
    }
}
