import QtQuick 2.15
import QtQuick.Controls 2.15

ProgressBar {
     id: control
     value: 0.5

     background: Rectangle {
         implicitWidth: 200
         implicitHeight: 6
         color: Styles.progress_bar_back_color
         radius: 3
     }

     contentItem: Item {
         implicitWidth: 200
         implicitHeight: 4

         Rectangle {
             width: control.visualPosition * parent.width
             height: parent.height
             radius: 2
             color: Styles.progress_bar_indicator_color
         }
     }
}
