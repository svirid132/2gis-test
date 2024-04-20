pragma Singleton
import QtQuick 2.15

QtObject {
    id: root
    property string filename: '...'
    function setFilename(filename) {
        root.filename = filename
    }
}
