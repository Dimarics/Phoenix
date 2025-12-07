import ".."
import App

StackBlock {
    id: set_joint
    objectName: "SetJoint"
    property alias list: joint_id.model
    ScratchText { text: "установить угол" }
    ListItem { id: joint_id; block: set_joint }
    ScratchText { text: "в" }
    ValueInput { id: angle; text: "60"; block: set_joint }
    ScratchText { text: "°" }
    function run() {
        App.device.protocol.setJoint(joint_id.currentIndex + 1, angle.value())
        var wrapper = function() {
            completed(next);
            App.device.protocol.goalAchieved.disconnect(wrapper)
        }
        App.device.protocol.goalAchieved.connect(wrapper)
    }
}
