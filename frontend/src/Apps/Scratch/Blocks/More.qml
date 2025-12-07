import ".."
import QtQuick.Layouts

BooleanBlock {
    id: more
    objectName: "More"
    ValueInput { id: arg_1; block: more }
    ScratchText { text: ">" }
    ValueInput { id: arg_2; text: "50"; block: more }
    function value() { return arg_1.value() > arg_2.value() }
    function cppCode(): string { return "(%1 > %2)".arg(arg_1.cppCode()).arg(arg_2.cppCode()) }
}
