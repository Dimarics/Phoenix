import ".."

RepeatBlock {
    id: repeatFor
    objectName: "RepeatFor"
    finished: !condition.value()
    ScratchText { text: "повторять пока" }
    BooleanSocket { id: condition; block: repeatFor }
    function run() { completed(finished ? next : contentSocket.next) }
    function cppCode(): string {
        return "while (" + condition.cppCode() + ") {\n" + contentSocket.cppCode() + "}\n" + bottomSocket.cppCode()
    }
}
