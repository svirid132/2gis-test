import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.15
import QtQuick.Layouts 1.15

MouseArea {
    id: root
    implicitWidth: rowLayout.implicitWidth; height: 20
    hoverEnabled: true
    property alias sourceImage: img.source
    property alias text: textLabel.text
    onClicked: {
        root.forceActiveFocus()
    }

    RowLayout {
        id: rowLayout
        width: root.width
        spacing: 5
        Item {
            width: 20; height: 20
            Image {
                id: img
                sourceSize.width: parent.width
                sourceSize.height: parent.height
                smooth: true
                visible: false
            }
            ColorOverlay {
                anchors.fill: img
                source: img
                color: root.containsMouse ? Styles.hovered_icon_button_color : Styles.icon_button_color
            }
        }
        Label {
            id: textLabel
            text: ''
        }
    }
}
