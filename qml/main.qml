import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import QtQuick.Window 2.1
import Qt.labs.settings 1.0

Rectangle {
    id: main
    color: "steelblue"

    property bool mirror: false

    Extras {
        id: utils
        anchors.fill: parent
        anchors.margins: 10
        Layout.column: main.mirror === true ? 0 : 1
    }
}
