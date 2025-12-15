import QtQuick as T

T.Text {
    id: root
    color: "white"
    property T.Item background: null
    //clip: true
    //antialiasing: false
    font.pointSize: Style.textSize
    verticalAlignment: Text.AlignVCenter
    elide: Text.ElideRight
    onBackgroundChanged: {
        background.z = -1
        background.parent = root
        background.anchors.fill = root
    }
}
