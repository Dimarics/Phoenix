pragma Singleton
import QtQuick

QtObject {
    property int textSize: 12
    property color textColor: "white"
    property color highlightTextColor: "#00FFFF"
    property color textSelectionColor: "#0078D7"

    property color lightWidgetColor: "#00326E"
    property color darkWidgetColor: "#002346" //"#142142"
    property color highlightBackgroundColor: "#145FA5"
    property color headerBackgroundColor: "#052D55"
    property color darkBorderColor: "#002346"
    property color brightBorderColor: "#305A8A"
    property color lightBorderColor: "#50A0F0"

    property color buttonBackgroundColor: "#20FFFFFF"
    property color buttonBorderColor: "#30FFFFFF"
    property color buttonTextColor: "white"
    property color buttonHoveredBackgroundColor: buttonBackgroundColor
    property color buttonHoveredBorderColor: buttonPressedBackgroundColor
    property color buttonHoveredTextColor: "#00FFFF"
    property color buttonPressedBackgroundColor: "#50FFFFFF"
    property color buttonPressedBorderColor: buttonPressedBackgroundColor
    property color buttonPressedTextColor: buttonHoveredTextColor

    property color color_1: "#002346"
    property color color_2: "#052D55"
    property color color_3: "#00326E"
    property color color_4: "#305A8A"
    property color color_5: "#145FA5"
    property color color_6: "#50A0F0"

    property font font
    font.family: "Arial"
}
