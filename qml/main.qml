import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import QtQuick.Window 2.1
import Qt.labs.settings 1.0

Rectangle {
    id: main
    color: "#F7F7EB"

    property bool mirror: false

    Extras {
        id: utils
        anchors.fill: parent
        anchors.margins: 6
        Layout.column: main.mirror === true ? 0 : 1
    }
}
