import QtQuick 2.15

import QtQuick 2.12
import QtQuick.Controls 2.12

TextField {
    id: control
    placeholderText: "Не выбран файл"
    topPadding: 0; bottomPadding: 0; leftPadding: 5; rightPadding: 5;

    cursorDelegate: Rectangle {
        id: cursor
        visible: false
        color: "black"
        width: control.cursorRectangle.width

        SequentialAnimation {
            loops: Animation.Infinite
            running: control.cursorVisible

            PropertyAction {
                target: cursor
                property: 'visible'
                value: true
            }

            PauseAnimation {
                duration: 600
            }

            PropertyAction {
                target: cursor
                property: 'visible'
                value: false
            }

            PauseAnimation {
                duration: 600
            }
        }
    }
    selectionColor:  Styles.text_field_selection_color

    font.pointSize: 12
    background: Rectangle {
        antialiasing: true
        implicitWidth: 200
        implicitHeight: 40
        color: control.enabled ? "transparent" : Styles.text_field_disabled_color
        border.color: control.enabled ? Styles.text_field_border_color : "transparent"
        radius: 10
    }
}

