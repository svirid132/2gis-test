import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3 as D
import Controller 1.0

RectWrapper {
    id: root
    implicitWidth: 220
    RankWordFileController {
        id: fileController
        Component.onCompleted: read()
    }

    contentItem: Item {
        implicitWidth: childrenRect.width; implicitHeight: childrenRect.height
        ColumnLayout {
            width: root.availableWidth
            CustomTextField {
                id: fileTextField
                clip: true
                selectByMouse: true
                Layout.fillWidth: true
                Layout.preferredWidth: 100
                // enabled: false
                onActiveFocusChanged: {
                    if (!activeFocus) {
                        let filename = text.replace(/^.*[\\/]/, '')
                        MainState.setFilename(filename)
                    }
                }
            }
            D.FileDialog {
                id: fileDialog
                title: 'Выберите файл'
                folder: shortcuts.home
                nameFilters: [ 'Текстовый файл (*.txt)' ]
                onAccepted: {
                    const fullPath = fileDialog.fileUrl.toString().replace(/file:[/][/][/]/, '')
                    fileTextField.text = fullPath
                    let filename = fullPath.replace(/^.*[\\/]/, '')
                    MainState.setFilename(filename)
                }
            }
            Button {
                Layout.fillWidth: true
                text: 'Выбрать файл'
                onClicked: {
                    fileDialog.open()
                }
            }
            RowLayout {
                Layout.fillWidth: true
                Button {
                    hoverEnabled: true
                    Layout.fillWidth: true
                    text: 'старт'
                    Backing { m_color: hovered ? Styles.backing_play_color : 'transparent' }
                }
                Button {
                    hoverEnabled: true
                    Layout.fillWidth: true
                    text: 'стоп'
                    Backing { m_color: hovered ? Styles.backing_pause_color : 'transparent' }
                }
                Button {
                    hoverEnabled: true
                    Layout.fillWidth: true
                    text: 'отмена'
                    Backing { m_color: hovered ? Styles.backing_close_color : 'transparent' }
                }
            }
            RowLayout {
                Layout.fillWidth: true
                CustomProgressBar {
                    Layout.fillWidth: true
                    value: fileController.progress
                }
                Label {
                    text: `${Math.trunc(fileController.progress * 100)}%`
                }
            }
        }
    }
}