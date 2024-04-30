import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import UI 1.0
import States 1.0
import Controller 1.0
import Model 1.0

RectWrapper {
    id: root
    contentItem: ColumnLayout {
        implicitWidth: root.availableWidth
        implicitHeight: root.availableHeight
        Label {
            text: `Количество слов в ${MainState.filename}`
            Layout.fillWidth: true
            font.pointSize: 16
            horizontalAlignment: Text.AlignHCenter
            clip: true
            elide: Text.ElideLeft
        }
        ScrollView {
            id: scrollView
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            bottomPadding: 15
            RankWordChart {
                width: Math.max(implicitWidth, scrollView.width)
                implicitHeight: scrollView.availableHeight
                onClickByColumn: {
                    if (!RankWordFileController.started && RankWordFileController.progress !== 1){
                        return
                    }
                    if (isSelected) {
                        FilterWordModel.remove(word)
                    } else {
                        FilterWordModel.append(word)
                    }
                    switchSelectedColumn(index, !isSelected)
                }
            }
        }
    }
}
