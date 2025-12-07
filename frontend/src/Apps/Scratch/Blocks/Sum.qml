import ".."

ValueBlock {
    id: sum
    objectName: "Sum"
    ValueInput { id: arg_1; block: sum }
    ScratchText { text: "+" }
    ValueInput { id: arg_2; block: sum }
    function value() { return arg_1.value() + arg_2.value() }
    function cppCode(): string { return `(${arg_1.cppCode()} + ${arg_2.cppCode()})` }
}
