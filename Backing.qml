import QtQuick 2.15
import QtQuick.Controls 2.15

Control {
    anchors.fill: parent
    leftPadding: parent.leftInset; rightInset: parent.rightInset
    topPadding: parent.topInset; bottomPadding: parent.bottomInset
    property color m_color: 'green'
    contentItem: Rectangle {
        width: availableWidth
        height: availableHeight
        color: m_color
        opacity: 0.5
        radius: 5
    }
}
