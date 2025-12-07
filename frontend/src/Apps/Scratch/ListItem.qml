import QtQuick as Q
import QtQuick.Controls.Basic as T
import QtQuick.Controls.impl
import QtQuick.Shapes
import QtQuick.Effects
//import QtQuick.Layouts

T.ComboBox {
    id: root
    property BasicBlock block
    font.family: "Segoe UI"; font.pointSize: 14; font.weight: Q.Font.DemiBold
    implicitHeight: 32
    //Layout.preferredHeight: 30
    spacing: 2
    background: Q.Rectangle {
        radius: 4
        color: down ? block.borderColor : block.fillColor
        border.color: block.borderColor
    }
    contentItem: Q.Text {
        leftPadding: 10
        rightPadding: root.indicator.width + root.spacing
        text: root.displayText
        font: root.font
        color: "white"
        verticalAlignment: Q.Text.AlignVCenter
    }
    indicator: Shape  {
        x: root.width - width - 10
        y: root.topPadding + (root.availableHeight - height) / 2 + 1
        width: 12; height: 8
        preferredRendererType: Shape.CurveRenderer
        ShapePath {
            strokeColor: block.borderColor
            fillColor: "white"
            Q.PathSvg { path: "M4.51301 7.34683L0.276603 2.63694C-0.570671 1.69497 0.0592234 0.191833 1.32492 0.135309L4.35483 0H6.7593L10.6513 0.152083C11.9212 0.201705 12.5579 1.70919 11.708 2.65406L7.48697 7.34683C6.69243 8.23017 5.30755 8.23017 4.51301 7.34683Z" }
        }
    }
    delegate: T.ItemDelegate {
        required property var model
        required property int index
        width: popup.width //Q.ListView.view.width
        leftPadding: 10; rightPadding: 10; topPadding: 4; bottomPadding: 4
        contentItem: Q.Text {
            text: model[root.textRole]
            color: "white"
            font: root.font
        }
        background: Q.Rectangle {
            color: root.highlightedIndex === index ? block.borderColor : "transparent"
        }
        //highlighted: root.highlightedIndex === index
    }
    popup: T.Popup {
        id: popup
        //x: (root.width * block.canvas.scale - width) / block.canvas.scale / 2
        x: block.canvas ? (root.width * block.canvas.scale - width) / block.canvas.scale / 2 : 0
        y: root.height + 18
        padding: 6
        topPadding: y > 0 ? 18 : 6
        bottomPadding: y < 0 ? 18 : 6
        implicitWidth: {
            if (!root.model) return 0
            let max_width = 0
            for (let i = 0; i < root.delegateModel.count; ++i) {
                if (root.model[i]) {
                    const text_width = font_metrics.advanceWidth(root.model[i]);
                    max_width = Math.max(max_width, text_width)
                } else if (root.model.get && root.model.get(i)) {
                    const text_width = font_metrics.advanceWidth(root.model.get(i)[root.textRole])
                    max_width = Math.max(max_width, text_width)
                }
            }
            return max_width + leftPadding + rightPadding + 20
        }
        Q.FontMetrics {
            id: font_metrics
            font: root.font
        }
        Q.NumberAnimation {
            id: openAnimation
            target: popup
            property: "y"
            from: root.height / 2; to: root.height + 18; duration: 200
        }
        onVisibleChanged: openAnimation.running = true
        //Q.Behavior on height { Q.NumberAnimation { duration: 2000 } }
        contentItem: Q.ListView {
            id: listView
            clip: true
            //implicitHeight: contentHeight
            model: root.delegateModel
            currentIndex: root.highlightedIndex
            //populate: Q.Transition { Q.NumberAnimation { properties: "x,y"; duration: 100 } }
        }
        background: Shape {
            id: popup_bkg
            property real radius: 6
            layer.enabled: true
            layer.samples: 4
            layer.effect: MultiEffect {
                shadowEnabled: true
                shadowColor: "#BF000000"
            }
            //preferredRendererType: Shape.CurveRenderer
            ShapePath {
                fillColor: block.fillColor
                strokeColor: block.borderColor
                startY: popup_bkg.radius + popup.topPadding - popup.padding
                Q.PathArc { x: popup_bkg.radius; relativeY: -popup_bkg.radius; radiusX: popup_bkg.radius; radiusY: popup_bkg.radius }
                Q.PathLine { x: (popup_bkg.width - 28) / 2; relativeY: 0 }
                Q.PathLine { relativeX: 12; relativeY: popup.y < 0 ? 0 : -12 }
                Q.PathLine { relativeX: 4; relativeY: 0 }
                Q.PathLine { relativeX: 12; relativeY: popup.y < 0 ? 0 : 12 }
                Q.PathLine { x: popup_bkg.width - popup_bkg.radius; relativeY: 0 }
                Q.PathArc { relativeX: popup_bkg.radius; relativeY: popup_bkg.radius; radiusX: popup_bkg.radius; radiusY: popup_bkg.radius }
                Q.PathLine { relativeX: 0; y: listView.contentHeight + popup.bottomPadding + popup.topPadding - popup_bkg.radius - popup.bottomPadding + popup.padding }
                Q.PathArc { relativeX: -popup_bkg.radius; relativeY: popup_bkg.radius; radiusX: popup_bkg.radius; radiusY: popup_bkg.radius }
                Q.PathLine { x: (popup_bkg.width + 28) / 2; relativeY: 0 }
                Q.PathLine { relativeX: -12; relativeY: popup.y < 0 ? 12 : 0 }
                Q.PathLine { relativeX: -4; relativeY: 0 }
                Q.PathLine { relativeX: -12; relativeY: popup.y < 0 ? -12 : 0 }
                Q.PathLine { x: popup_bkg.radius; relativeY: 0 }
                Q.PathArc { x: 0; relativeY: -popup_bkg.radius; radiusX: popup_bkg.radius; radiusY: popup_bkg.radius }
                Q.PathLine { x: 0; y: popup_bkg.radius + popup.topPadding - popup.padding }
            }
        }
    }
}

