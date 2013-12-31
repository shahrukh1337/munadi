import QtQuick 2.2
import QtQuick.Layouts 1.1

Rectangle {
    color: parent.color
    width: 1; height: 1

    Layout.fillWidth: true
    Layout.fillHeight: true

    Rectangle {
        id: eItems
        width: parent.width
        anchors.top: parent.top
        anchors.bottom: buttons.top
        anchors.bottomMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter

        Loader {
            id: loader
            anchors.fill: parent

            Component.onCompleted: source = "Info.qml"
        }

    }

    Rectangle {
        id: buttons
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width
        height: (parent.height / 15)

        color: parent.color

        RowLayout {
            spacing: 10
            anchors.fill: parent
            layoutDirection: main.mirror === true ? "RightToLeft" : "LeftToRight"

            Button {
                iconSource: "qrc:/img/info.png"
                onClick: loader.source = "Info.qml"
            }

            Button {
                iconSource: "qrc:/img/compass.png"
                onClick: loader.source = "Compass.qml"
            }

            Button {
                iconSource: "qrc:/img/settings.png"
                onClick: loader.source = "Settings.qml"
            }
        }
    }
}
