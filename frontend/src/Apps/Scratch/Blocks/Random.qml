import ".."

ValueBlock {
    id: random
    objectName: "Random"
    ScratchText { text: "случайное от" }
    ValueInput { id: min; text: "1"; block: random }
    ScratchText { text: "до" }
    ValueInput { id: max; text: "10"; block: random }
    function value() { return Math.random() * (max.value() - min.value()) + min.value() }
    function cppCode(): string { return "random(%1, %2)".arg(min.cppCode()).arg(max.cppCode()) }
}
