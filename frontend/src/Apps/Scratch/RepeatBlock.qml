import QtQuick

ContentBlock {
    id: root
    data: Image {
        id: logo
        x: root.width - width / 2 - 16; y: root.height - height / 2 - 16
        width: 20
        source: "qrc:/images/repeat_arrow.svg"
        sourceSize: canvas ? Qt.size(width * canvas.scale, height * canvas.scale) : Qt.size(width, height)
        fillMode: Image.PreserveAspectFit
        mipmap: true
        verticalAlignment: Qt.AlignVCenter
        horizontalAlignment: Qt.AlignHCenter
    }
}
