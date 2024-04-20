import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15
import Styles 1.0

Window {
    id: root
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    property var styles: Styles

    Control {
        id: wrapper
        width: root.width; height: root.height
        padding: 5
        contentItem: RowLayout {
            spacing: 7
            width: wrapper.availableWidth; height: wrapper.availableHeight
            RankWordChartWithTitle {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
            ControlPanel {
                Layout.fillHeight: true
            }
        }
        background: Rectangle {
            color: Styles.window_color
        }
    }
}
