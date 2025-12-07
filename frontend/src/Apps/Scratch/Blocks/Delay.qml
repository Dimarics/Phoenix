import QtQuick
import ".."

StackBlock {
    id: delay
    objectName: "Delay"
    borderColor: "#774DCB"
    fillColor: "#9966FF"
    ScratchText { text: "ждать" }
    ValueInput { id: value; text: "1000"; block: delay }
    ListItem { id: units; block: delay; model: ["миллисекунды", "секунды", "минуты"] }
    Timer {
        id: timer
        onTriggered: {
            //console.log("delay %1".arg(interval))
            completed(next)
        }
    }
    function run() {
        var time = value.value()
        switch(units.currentText) {
        case "секунды": time *= 1000; break;
        case "минуты": time *= 60000; break;
        }
        //console.log("delay %1".arg(time))
        //completed(next)
        timer.interval = time
        timer.restart()
    }
    function cppCode(): string {
        var k = 1
        switch(units.currentText) {
        case "секунды": k = 1000; break;
        case "минуты": k = 60000; break;
        }
        var code = "delay(%1 * %2);\n".arg(value.cppCode()).arg(k)
        return code + bottomSocket.cppCode()
    }
}
