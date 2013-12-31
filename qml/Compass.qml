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

        Image {
            id: arrow
            anchors.centerIn: parent
            //width: 20
            height: parent.height / 2
            smooth: true
            antialiasing: true
            fillMode: Image.PreserveAspectFit

            source: "qrc:/img/arrow.png"

            Behavior on rotation {
                NumberAnimation {
                    //This specifies how long the animation takes
                    duration: 5000
                    //This selects an easing curve to interpolate with, the default is Easing.Linear
                    easing.type: Easing.OutElastic
                }
            }

            Component.onCompleted: {
                settingsCache.refresh();
                rotation = engine.getQibla();
                console.debug(engine.getQibla());
            }
        }
    }

    Text {
        id: qiblaInfo
        anchors.top: compass.bottom
        anchors.bottom: parent.bottom
        anchors.margins: 20
        anchors.horizontalCenter: parent.horizontalCenter
        text: qsTr(engine.getQibla() + "° with respect to North")
    }
}
