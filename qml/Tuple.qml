import QtQuick 2.2
import QtQuick.Layouts 1.1

Rectangle {

    width: 1; height: 1
    Layout.fillWidth: true
    Layout.fillHeight: true

    property alias text: label.text
    property alias italic: label.font.italic

    Text {
        id: label
        anchors.centerIn: parent
        font.pixelSize: parent.height / 2
        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        style: Text.Sunken
    }
}
