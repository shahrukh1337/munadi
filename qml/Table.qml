import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

Rectangle {

    color: main.color
    width: 1; height: 1
    Layout.fillWidth: true
    Layout.fillHeight: true

    Component.onCompleted: {
        updatePrayerTimes();
        updateTimeLeft();
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: main.margin

        Prayer {
            id: fajr
            prayerName: qsTr("Fajr")
        }

        Prayer {
            id: sunrise
            prayerName: qsTr("Sunrise")
        }

        Prayer {
            id: duhr
            prayerName: qsTr("Duhr")
        }

        Prayer {
            id: asr
            prayerName: qsTr("Asr")
        }

        Prayer {
            id: magrib
            prayerName: qsTr("Magrib")
        }

        Prayer {
            id: isha
            prayerName: qsTr("Ishaa")
        }

        Rectangle {
            id: tlc
            width: 1; height: 1
            Layout.fillWidth: true
            Layout.fillHeight: true

            Text {
                id: timeLeft;
                anchors.centerIn: parent
                font.pixelSize: parent.height / 3
                font.italic: true
            }
        }

        Connections {
            target: engine
            onAthanStarted: {
                console.log("onAthanStarted");
                updateTimeLeft();
            }
            onAthanStopped: {
                console.log("onAthanStopped");
                updateTimeLeft();
            }
            onMinuteElapsed: {
                console.log("onUpdateTimeLeft")
                updatePrayerTimes();
                updateTimeLeft();
            }
        }
    }

    function updatePrayerTimes() {

        fajr.prayerTime = Qt.formatTime(engine.getFajr(), "h:mm AP");
        sunrise.prayerTime = Qt.formatTime(engine.getSunrise(), "h:mm AP");
        duhr.prayerTime = Qt.formatTime(engine.getDuhr(), "h:mm AP");
        asr.prayerTime = Qt.formatTime(engine.getAsr(), "h:mm AP");
        magrib.prayerTime = Qt.formatTime(engine.getMagrib(), "h:mm AP");
        isha.prayerTime = Qt.formatTime(engine.getIsha(), "h:mm AP");
    }

    function updateTimeLeft() {
        console.log("updateTimeLeft")
        if(engine.isAthanPlaying()) {
            timeLeft.text = engine.getCurrPrayerLabel() + qsTr(" prayer time ...");
        }
        else {
            var prayerLabel = engine.getNextPrayerLabel();

            if(prayerLabel === "") {
                tlc.visible = false;
            } else {
                tlc.visible = true;
                timeLeft.text = prayerLabel + " in " + engine.getTimeDifference();
            }
        }
    }
}
