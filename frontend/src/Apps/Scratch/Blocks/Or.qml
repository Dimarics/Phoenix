import ".."

BooleanBlock {
    id: or
    objectName: "Or"
    BooleanSocket { id: arg_1; block: or }
    ScratchText { text: "или" }
    BooleanSocket { id: arg_2; block: or }
    function value() { return arg_1.value() || arg_2.value() }
    function cppCode(): string { return `(${arg_1.cppCode()} || ${arg_2.cppCode()})` }
}
