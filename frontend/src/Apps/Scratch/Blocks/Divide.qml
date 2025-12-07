import ".."

ValueBlock {
    id: divide
    objectName: "Divide"
    ValueInput { id: arg_1; block: divide }
    ScratchText { text: "/" }
    ValueInput { id: arg_2; block: divide }
    function value() { return arg_1.value() / arg_2.value() }
    function cppCode(): string { return "(%1 / %2)".arg(arg_1.cppCode()).arg(arg_2.cppCode()) }
}
