import ".."

RepeatBlock {
    id: repeatCount
    objectName: "RepeatCount"
    property int count: 0
    ScratchText { text: "повторить" }
    ValueInput { id: limit; text: "10"; state: "int"; block: repeatCount }
    ScratchText { text: "раз" }
    //onFinishedChanged: count = 0
    function reset() { finished = false; count = 0 }
    function run() {
        /*if (!finished && count < limit.value()) {
            if (++count >= limit.value()) finished = true
            completed(contentSocket.next)
        } else {
            if (!finished) finished = true
            completed(next)
        }*/
        if (finished) { completed(next); return }
        if (count < limit.value()) {
            if (++count === limit.value()) finished = true
            completed(contentSocket.next)
        }
    }
    function cppCode(): string {
        return "for (int _i = 0; _i < " + limit.cppCode() + "; ++_i) {\n" +
                contentSocket.cppCode() + "}\n" + bottomSocket.cppCode()
    }
}
