import ".."

ContentBlock {
    id: root
    objectName: "If"
    ScratchText { text: "если" }
    BooleanSocket { id: condition; block: root }
    ScratchText { text: "то" }
    function reset() { finished = false }
    function run() {
        if (finished) { completed(next); return }
        finished = true
        completed(condition.value() ? contentSocket.next : null)
    }
    function cppCode(): string {
        return "if (" + condition.cppCode() + ") {\n" + contentSocket.cppCode() + "}\n" + bottomSocket.cppCode()
    }
}
