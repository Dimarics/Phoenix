import ".."

BooleanBlock {
    id: and
    objectName: "And"
    BooleanSocket { id: arg_1; block: and }
    ScratchText { text: "и" }
    BooleanSocket { id: arg_2; block: and }
    function value() { return arg_1.value() && arg_2.value() }
    function cppCode(): string { return "(%1 && %2)".arg(arg_1.cppCode()).arg(arg_2.cppCode()) }
}
