import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3 as D
import Controller 1.0
import Model 1.0
import Types 1.0

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

    D.MessageDialog {
        id: errorDialog
        title: 'Ошибка чтения'
        text: `Ошибка чтения файла ${MainState.filename}`
        icon: D.StandardIcon.Critical
    }

    contentItem: ColumnLayout {
        width: root.availableWidth; height: root.availableHeight
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
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
            ColumnLayout {
                anchors.fill: parent
                Label {
                    text: 'Фильтр слов:'
                    font.pointSize: 12
                }
                ListView {
                    id: listView
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    model: FilterWordModel
                    property int currentEditable: -1
                    delegate: Item {
                        id: delegateItem
                        implicitWidth: listView.width
                        implicitHeight: 35
                        Rectangle {
                            anchors.fill: parent
                            z: listView.currentEditable === index ? 10 : 1
                            onZChanged: {
                                if (listView.currentEditable === index) {
                                    textEdit.forceActiveFocus()
                                }
                            }

                            TextInput {
                                id: textEdit
                                anchors.fill: parent
                                font.pointSize: 11
                                verticalAlignment: TextEdit.AlignVCenter
                                validator: RegExpValidator {regExp: RegExp(RegExpIns.filter)}
                                text: display
                                selectByMouse: true
                                function edit() {
                                    const isEdit = listView.model.edit(index, text)
                                    if (!isEdit) {
                                        text = Qt.binding(() => display)
                                    }
                                    listView.currentEditable = -1
                                    listView.model.save()
                                }
                                onAccepted: edit()
                                onActiveFocusChanged: {
                                    if (!activeFocus) {
                                        edit()
                                    }
                                }
                            }
                        }
                        MouseArea {
                            id: mouseArea
                            anchors.fill: parent
                            hoverEnabled: true
                            z: listView.currentEditable === index ? 1 : 10
                            Rectangle {
                                anchors.fill: parent
                                Label {
                                    anchors.fill: parent
                                    verticalAlignment: Text.AlignVCenter
                                    text: display
                                    font.pointSize: 11
                                }
                            }
                            RowLayout {
                                visible: mouseArea.containsMouse
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.right: parent.right
                                IconButton {
                                    sourceImage: 'qrc:/resource/icons/edit.svg'
                                    onClicked: {
                                        listView.currentEditable = index
                                    }
                                }
                                IconButton {
                                    sourceImage: 'qrc:/resource/icons/trash.svg'
                                    onClicked: {
                                        listView.model.remove(index)
                                        listView.model.save()
                                    }
                                }
                            }
                        }
                    }
                }
                Item {
                    Layout.fillWidth: true
                    height: childrenRect.height
                    IconButton {
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: 'Добавить текст'
                        sourceImage: 'qrc:/resource/icons/add.svg'
                        onClicked: {
                            listView.model.appendNewWord()
                            listView.currentEditable = listView.model.rowCount() - 1
                        }
                    }
                }

            }
        }
    }
}
