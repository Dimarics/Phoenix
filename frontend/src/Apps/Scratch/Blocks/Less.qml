import ".."

BooleanBlock {
    id: less
    objectName: "Less"
    ValueInput { id: arg_1; block: less }
    ScratchText { text: "<" }
    ValueInput { id: arg_2; text: "50"; block: less }
    function value() { return arg_1.value() < arg_2.value() }
    function cppCode(): string { return "(%1 < %2)".arg(arg_1.cppCode()).arg(arg_2.cppCode()) }
}
