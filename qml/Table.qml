import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

Rectangle {

    color: "#F7F7EB"
    width: 1; height: 1
    Layout.fillWidth: true
    Layout.fillHeight: true

    Component.onCompleted: {
        updatePrayerTimes();
        updateTimeLeft();
    }

    ColumnLayout {
        anchors.margins: 3
        anchors.fill: parent
        spacing: 10

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
            width: 1; height: 1
            Layout.fillWidth: true
            Layout.fillHeight: true

            Text {
                id: timeLeft;
                anchors.centerIn: parent
                font.pixelSize: parent.height / 3
                font.italic: true

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
        timeLeft.text = engine.isAthanPlaying()
                ? engine.getCurrPrayerLabel() + qsTr(" prayer time ...")
                : engine.getNextPrayerLabel() + " in " + engine.getTimeDifference();
    }
}
