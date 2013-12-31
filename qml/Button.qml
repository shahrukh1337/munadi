import QtQuick 2.2
import QtQuick.Layouts 1.1

Rectangle {
    Layout.fillHeight: parent
    Layout.fillWidth: true

    property alias iconSource: img.source
    signal click

    color: ma.containsMouse ? "lightgrey" : "white"

    Image {
        id: img
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
        anchors.margins: 4
    }

    MouseArea {
        id: ma
        hoverEnabled: true
        anchors.fill: parent
        onClicked: parent.click()
    }
}
