import ".."
import App

StackBlock {
    id: root
    objectName: "RotateMotor"
    property alias list: motor_id.model
    ScratchText { text: "вращать мотор" }
    ListItem { id: motor_id; block: root }
    ListItem { id: dir; block: root; model: ["по часовой стрелке", "против часовой стрелки"] }
    ScratchText { text: "со скоростью" }
    ValueInput { id: speed; text: "150"; block: root }
    function cppCode(): string {
        //return "analogWrite(MOTOR_%1, %2);\n%3".arg(motor_id.currentText).arg(speed.value()).arg(bottomSocket.cppCode())
        return `rotateMotor(${motor_id.currentIndex}, ${dir.currentIndex}, ${speed.cppCode()});\n${bottomSocket.cppCode()}`
    }
    function run() {
        //App.device.protocol.rotateMotor(motor_id.currentIndex, dir.currentIndex, speed.value(), ()=>completed(next))
        App.device.rotateMotor(motor_id.currentIndex, dir.currentIndex, speed.value())
        completed(next)
    }
}
