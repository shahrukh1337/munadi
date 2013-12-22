import QtQuick 2.2

Rectangle {

    Text {
        id: qiblaLabel
        anchors.top: parent.top

        anchors.margins: 20
        anchors.horizontalCenter: parent.horizontalCenter
        text: qsTr("Qibla Direction")
        font.pixelSize: Font.pixelSize * parent.scale
    }
    Rectangle {
        id: compass
        anchors.margins: 20
        anchors.top: qiblaLabel.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        radius: width + 45.0

        width: Math.min(parent.width, parent.height) / 1.5
        height: width

        border.color:  "black"
        border.width: 4

        Component.onCompleted: console.log(parent.width + ", " + parent.height)
    }

    Text {
        id: qiblaInfo
        anchors.top: compass.bottom
        anchors.bottom: parent.bottom
        anchors.margins: 20
        anchors.horizontalCenter: parent.horizontalCenter
        text: qsTr("444N with respect to North")
    }
//    Text {
//        id: qiblaInfo2
//        //anchors.top: compass.bottom
//        //anchors.bottom: parent.bottom
//        anchors.margins: 20
//        anchors.horizontalCenter: parent.horizontalCenter
//        text: qsTr("444N with respect to North")
//    }
//    Text {
//        id: qiblaInfo3
//        //anchors.top: compass.bottom
//        //anchors.bottom: parent.bottom
//        anchors.margins: 20
//        anchors.horizontalCenter: parent.horizontalCenter
//        text: qsTr("444N with respect to North")
//    }
}
