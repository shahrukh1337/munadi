import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import Qt.labs.settings 1.0

ScrollView {
    id: sv

    Component.onDestruction: {
        engine.refreshSettingsCache();
    }

    ColumnLayout {
        spacing: 20
        width: sv.width - 30    // So the vertical scroll bar doesn't appear
        x: 10
        anchors.bottomMargin: 20
        anchors.topMargin: 20

        SettingEntry {} // Top padding

        SettingEntry {
            ColumnLayout {
                SettingTitle {
                    text: qsTr("Show on Athan?")
                }

                SettingInfo {
                    text: qsTr("Munadi window will pop up during Athan if on.")
                }
            }

            Switch {
                id: soa
                checked: true
            }
        }

        Settings {
            category: "Window"
            property alias showOnAthan: soa.checked
        }

        SettingEntry {
            ColumnLayout {
                SettingTitle {
                    text: qsTr("Start up?")
                }

                SettingInfo {
                    text: qsTr("Munadi will open when computer starts if on.")
                }
            }

            Switch {
                id: asu
                checked: true
            }
        }

        SettingEntry {
            ColumnLayout {
                SettingTitle {
                    text: qsTr("Check for updates")
                }

                SettingInfo {
                    text: qsTr("Munadi will check for any updates when it starts.")
                }
            }

            Switch {
                id: cfu
                checked: true
            }
        }

        Settings {
            category: "General"
            property alias autoStartUp: asu.checked
            property alias checkForUpdates: cfu.checked

            onAutoStartUpChanged: {
                engine.setStartup(asu.checked);
            }
        }

        SettingEntry {
            ColumnLayout {
                SettingTitle {
                    text: qsTr("Volume")
                }

                SettingEntry {
                    CheckBox {
                        id: muted
                        text: qsTr("Mute Athan")
                        onCheckedChanged: engine.setVolume(vol.value, muted.checked)
                    }

                    Slider {
                        id: vol
                        Layout.fillWidth: true
                        stepSize: 1
                        minimumValue: 0
                        maximumValue: 100
                        onValueChanged: engine.setVolume(vol.value, muted.checked)
                    }
                    Text {
                        text: vol.value + "%"
                    }
                }
            }
        }

        SettingEntry {
            ColumnLayout {
                SettingTitle {
                    text: qsTr("Prayer time adjustments")
                }

                RowLayout {
                    CheckBox {
                        id: fajrEnabled
                        Layout.fillWidth: true
                        text: qsTr("Fajr")
                        checked: true
                    }
                    SpinBox {
                        id: fajrAdjustment
                        minimumValue: -99
                        maximumValue: 99
                        suffix: qsTr(" Minutes")
                    }
                }

                RowLayout {
                    CheckBox {
                        id: duhrEnabled
                        Layout.fillWidth: true
                        text: qsTr("Duhr")
                        checked: true
                    }
                    SpinBox {
                        id: duhrAdjustment
                        minimumValue: -99
                        maximumValue: 99
                        suffix: qsTr(" Minutes")
                    }
                }

                RowLayout {
                    CheckBox {
                        id: asrEnabled
                        Layout.fillWidth: true
                        text: qsTr("Asr")
                        checked: true
                    }
                    SpinBox {
                        id: asrAdjustment
                        minimumValue: -99
                        maximumValue: 99
                        suffix: qsTr(" Minutes")
                    }
                }

                RowLayout {
                    CheckBox {
                        id: magribEnabled
                        Layout.fillWidth: true
                        text: qsTr("Magrib")
                        checked: true
                    }
                    SpinBox {
                        id: magribAdjustment
                        minimumValue: -99
                        maximumValue: 99
                        suffix: qsTr(" Minutes")
                    }
                }

                RowLayout {
                    CheckBox {
                        id: ishaEnabled
                        Layout.fillWidth: true
                        text: qsTr("Isha")
                        checked: true
                    }
                    SpinBox {
                        id: ishaAdjustment
                        minimumValue: -99
                        maximumValue: 99
                        suffix: qsTr(" Minutes")
                    }
                }
            }
        }

        Settings {
            category: "Athan"
            property alias fajrEnabled: fajrEnabled.checked
            property alias duhrEnabled: duhrEnabled.checked
            property alias asrEnabled: asrEnabled.checked
            property alias magribEnabled: magribEnabled.checked
            property alias ishaEnabled: ishaEnabled.checked

            property alias fajrAdjustment: fajrAdjustment.value
            property alias duhrAdjustment: duhrAdjustment.value
            property alias asrAdjustment: asrAdjustment.value
            property alias magribAdjustment: magribAdjustment.value
            property alias ishaAdjustment: ishaAdjustment.value

            property alias volume: vol.value
            property alias muted: muted.checked
        }

        SettingEntry {
            ColumnLayout {
                SettingTitle {
                    text: qsTr("Location")
                }

                ComboBox {
                    id: cbCountries
                    Layout.fillWidth: true
                    textRole: 'name'

                    model: ListModel {
                        id: countries

                        Component.onCompleted: {
                            var arr = settingsCache.getCountries();

                            for(var i=0; i < arr.length; i++) {
                                var line = arr[i].split(",");
                                var name = line[0];
                                var code = line[1];
                                countries.append({"name": name, "code": code});
                            }
                            console.debug("populated list of countries");
                        }
                    }

                    onCurrentIndexChanged: {
                        cities.clear();
                        var arr = settingsCache.getCities(countries.get(currentIndex).code);

                        for(var i=0; i < arr.length; i++) {
                            var line = arr[i].split(",");
                            var name = line[1];
                            var lat = line[2];
                            var lon = line[3];
                            var gmt = line[4];
                            cities.append({"name": name, "lat": lat, "lon": lon, "gmt": gmt, "country": countries.get(currentIndex).name});
                        }
                        console.debug("populated list of cities for country " + countries.get(currentIndex).name);
                    }
                }

                ComboBox {
                    id: cbCities
                    Layout.fillWidth: true
                    textRole: 'name'
                    model: ListModel { id: cities }

                    Component.onCompleted: cbCities.currentIndex = -1

                    onCurrentIndexChanged: {
                        if(cbCities.currentIndex != -1) {
                            tfLat.text = cities.get(currentIndex).lat;
                            tfLon.text = cities.get(currentIndex).lon;
                            tfGmt.value = cities.get(currentIndex).gmt;
                            tfLocation.text = cities.get(currentIndex).name
                                    + ", " + cities.get(currentIndex).country;
                        }
                    }
                }

                GridLayout {
                    columns: 2

                    Text {
                        text: qsTr("Location")
                    }
                    TextField {
                        id: tfLocation
                        Layout.fillWidth: true
                        text: qsTr("Mecca, Saudi Arabia")
                    }
                    Text {
                        text: qsTr("Latitude")
                    }
                    TextField {
                        id: tfLat
                        text: qsTr("21.42667")
                    }
                    Text {
                        text: qsTr("Longitude")
                    }
                    TextField {
                        id: tfLon
                        text: qsTr("39.82611")
                    }
                    Text {
                        text: qsTr("GMT Offset")
                    }
                    SpinBox {
                        id: tfGmt
                        value: 3.0
                        stepSize: 0.5
                        decimals: 1
                        maximumValue: 14.0
                        minimumValue: -12.0
                    }
                    Text {
                        text: qsTr("Day Light Saving");
                    }
                    Switch {
                        id: dst
                        checked: false
                    }
                }
            }
        }

        Settings {
            category: "Location"
            property alias name: tfLocation.text
            property alias latitude: tfLat.text
            property alias longitude: tfLon.text
            property alias gmtOffset: tfGmt.value
            property alias dst: dst.checked
        }

        SettingEntry {
            GridLayout {
                columns: 2
                SettingTitle {
                    text: qsTr("Method")
                    Layout.columnSpan: 2
                }
                Text {
                    text: qsTr("Mathhab");
                }
                ComboBox {
                    id: mathhab
                    model: ListModel {
                        ListElement {
                            text: "Default"
                        }
                        ListElement {
                            text: "Hanafi"
                        }
                    }
                }
                Text {
                    text: qsTr("Algorithm");
                }
                ComboBox {
                    id: algo
                    Layout.fillWidth: true
                    currentIndex: 4

                    model: ListModel {
                        ListElement {
                            text: "None"
                        }
                        ListElement {
                            text: "Egyptian General Authority of Survey"
                        }
                        ListElement {
                            text: "University of Islamic Sciences, Karachi (Shaf'i)"
                        }
                        ListElement {
                            text: "University of Islamic Sciences, Karachi (Hanafi)"
                        }
                        ListElement {
                            text: "Islamic Society of North America"
                        }
                        ListElement {
                            text: "Muslim World League (MWL)"
                        }
                        ListElement {
                            text: "Umm Al-Qurra, Saudi Arabia"
                        }
                        ListElement {
                            text: "Fixed Ishaa Interval (always 90)"
                        }
                        ListElement {
                            text: "Egyptian General Authority of Survey (Egypt)"
                        }
                    }
                }
            }
        }

        Settings {
            category: "Method"
            property alias mathhab: mathhab.currentIndex
            property alias calcMethod: algo.currentIndex
        }

        SettingEntry {
            ColumnLayout {
                SettingTitle {
                    text: qsTr("About Munadi")
                }
                SettingInfo {
                    text: "<p>Munadi - Simple Athan Program.<br/>" +
                          "For more info, visit <a href=\"http://munadi.org\">Munadi.org</a></p>" +
                          "<br/>" + "Version: " + engine.getVersionNo() + "<br/>"
                }
            }
        }
    }
}
