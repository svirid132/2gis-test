import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3 as D
import Controller 1.0
import QtQuick.Dialogs 1.3

RectWrapper {
    id: root
    implicitWidth: 220
    RankWordFileController {
        id: fileController
        fullpath: {
            return fileTextField.text.replace(/^[\\]/, '/')
        }
        onError: {
            errorDialog.open()
        }
    }

    MessageDialog {
        id: errorDialog
        title: 'Ошибка чтения'
        text: `Ошибка чтения файла ${MainState.filename}`
        icon: StandardIcon.Critical
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
                enabled: !fileController.started && !fileController.reading
            }
            D.FileDialog {
                id: fileDialog
                title: 'Выберите файл'
                folder: shortcuts.home
                nameFilters: [ 'Текстовый файл (*.txt)' ]
                onAccepted: {
                    const fullPath = fileDialog.fileUrl.toString().replace(/file:[/][/][/]/, '')
                    fileTextField.text = fullPath
                }
            }
            Button {
                Layout.fillWidth: true
                text: 'Выбрать файл'
                enabled: !fileController.started && !fileController.reading
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
                    Backing {
                        m_color: {
                            if (hovered || fileController.reading) {
                                return Styles.backing_play_color
                            }

                            return 'transparent'
                        }
                    }
                    onClicked: {
                        if (!fileController.started) {
                            fileController.read()
                            let filename = fileTextField.text.replace(/^.*[\\/]/, '')
                            MainState.setFilename(filename)
                        }
                        fileController.resume()
                    }
                }
                Button {
                    hoverEnabled: true
                    Layout.fillWidth: true
                    text: 'стоп'
                    Backing { m_color: {
                            if (hovered || (fileController.started && !fileController.reading)) {
                                return Styles.backing_pause_color
                            }

                            return 'transparent'
                        }
                    }
                    onClicked: {
                        fileController.pause()
                    }
                }
                Button {
                    hoverEnabled: true
                    Layout.fillWidth: true
                    text: 'отмена'
                    Backing { m_color: hovered ? Styles.backing_close_color : 'transparent' }
                    onClicked: {
                        fileController.cancel()
                        MainState.setFilename('...')
                    }
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
