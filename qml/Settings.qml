import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import Qt.labs.settings 1.0

Rectangle {
    color: "black"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 2
        //anchors.top: parent.top

        //width: parent.width
        //height: 300

        Setting {
            RowLayout {
                anchors.fill: parent
                anchors.margins: 6

                ColumnLayout {
                    Text {
                        id: saf
                        text: qsTr("Show on Athan?")
                        font.bold: true
                    }

                    Text {
                        text: qsTr("If on, Munadi window will pop up during Athan.")
                        font.italic: true
                        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    }
                }

                Switch {
                    id: soaSwitch
                    checked: false
                }
            }
        }


        Setting {

            RowLayout {
                anchors.fill: parent

                ColumnLayout {
                    Text {
                        text: qsTr("Auto update?")
                        font.bold: true
                    }

                    Text {
                        text: qsTr("If on, Munadi will check for updates everytime it starts.")
                        font.italic: true
                        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    }
                }

                Switch {
                    id: updaterSwitch
                    checked: false
                }
            }
        }

        Setting {

            RowLayout {
                anchors.fill: parent

                Text {
                    text: qsTr("Auto update1111")
                    font.bold: true
                }

                Switch {
                    checked: false
                }
            }
        }

    }

    Settings {
        property alias showOnAthan: soaSwitch.checked
        property alias checkForUpdates: updaterSwitch.checked;
    }
}
