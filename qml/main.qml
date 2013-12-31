import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import Qt.labs.settings 1.0

Rectangle {
    id: main
    color: "#EFEBD6"

    property bool mirror: false
    property int margin: width / 50

    Extras {
        id: utils
        anchors.fill: parent
        anchors.margins: parent.margin
        Layout.column: main.mirror === true ? 0 : 1
    }
}
