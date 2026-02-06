import Apps.Scratch
import App

StackBlock {
    id: root
    objectName: "../../../ScratchBlocks/SetPoint"
    ScratchText { text: "движение в точку - x" }
    ValueInput { id: x_value; text: "500"; block: root; state: "double"; min: -6000; max: 6000 }
    ScratchText { text: " y" }
    ValueInput { id: y_value; text: "500"; block: root; state: "double"; min: -6000; max: 6000 }
    ScratchText { text: " z" }
    ValueInput { id: z_value; text: "1000"; block: root; state: "double"; min: 0; max: 6000 }
    ScratchText { text: "мм" }
    function run() {
        //App.device.protocol.setPoint(x_value.value(), y_value.value(), z_value.value())
        App.device.protocol.setX(x_value.value() / 1000);
        App.device.protocol.setY(y_value.value() / 1000);
        App.device.protocol.setZ(z_value.value() / 1000);

        completed(next)
    }
}
