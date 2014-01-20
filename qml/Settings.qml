import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import Qt.labs.settings 1.0

ScrollView {
    id: sv
    flickableItem.flickableDirection: Flickable.VerticalFlick
    Component.onDestruction: {
        engine.refreshSettingsCache();
    }

    ColumnLayout {
        spacing: 20
        width: sv.width - 30    // So the vertical scroll bar doesn't appear
        x: 10   //left padding didn't work
        anchors.bottomMargin: 20
        anchors.topMargin: 20

        RowLayout {} // Top padding

        RowLayout {
            ColumnLayout {
                SettingTitle {
                    text: qsTr("Show on Athan?")
                }

                SettingInfo {
                    text: qsTr("Munadi window will pop up during Athan if on")
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

        RowLayout {
            ColumnLayout {
                SettingTitle {
                    text: qsTr("Start up?")
                }

                SettingInfo {
                    text: qsTr("Munadi will open when computer starts if on")
                }
            }

            Switch {
                id: asu
                checked: true
            }
        }

        RowLayout {
            ColumnLayout {
                SettingTitle {
                    text: qsTr("Check for updates")
                }

                SettingInfo {
                    text: qsTr("Munadi will check for any updates when it starts")
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

        RowLayout {
            ColumnLayout {
                SettingTitle {
                    text: qsTr("Volume")
                }

                RowLayout {
                    Text {
                        text: vol.value + " %"
                        Layout.minimumWidth: font.pixelSize * 3 //Stops sliders from shivering when changing val
                    }
                    Slider {
                        id: vol
                        Layout.fillWidth: true
                        stepSize: 1
                        minimumValue: 0
                        maximumValue: 100
                        onValueChanged: engine.setVolume(vol.value, muted.checked)
                    }
                }

                RowLayout {
                    Text {
                        text: qsTr("Mute Athan")
                        Layout.fillWidth: true
                    }
                    Switch {
                        id: muted
                        onCheckedChanged: engine.setVolume(vol.value, muted.checked)
                    }
                }
            }
        }

        RowLayout {
            ColumnLayout {
                SettingTitle {
                    text: qsTr("Adjustment")
                }

                GridLayout {
                    columns: 4

                    Text {
                        text: qsTr("Fajr: ")
                    }
                    Text {
                        text: fajrAdjustment.value.toFixed() > 0
                              ? "+" + fajrAdjustment.value.toFixed()
                              : fajrAdjustment.value.toFixed()
                        Layout.minimumWidth: font.pixelSize * 2 //Stops sliders from shivering when changing val, 1 enough for all 5!
                    }
                    Slider {
                        id: fajrAdjustment
                        Layout.fillWidth: true
                        minimumValue: -20
                        maximumValue: 20
                        stepSize: 1

                    }
                    Switch {
                        id: fajrEnabled
                        checked: true
                    }

                    Text {
                        text: qsTr("Duhr: ")
                    }
                    Text {
                        text: duhrAdjustment.value.toFixed() > 0
                              ? "+" + duhrAdjustment.value.toFixed()
                              : duhrAdjustment.value.toFixed()
                    }
                    Slider {
                        id: duhrAdjustment
                        Layout.fillWidth: true
                        minimumValue: -20
                        maximumValue: 20
                        stepSize: 1
                    }
                    Switch {
                        id: duhrEnabled
                        checked: true
                    }


                    Text {
                        text: qsTr("Asr: ")
                    }
                    Text {
                        text: asrAdjustment.value.toFixed() > 0
                              ? "+" + asrAdjustment.value.toFixed()
                              : asrAdjustment.value.toFixed()
                    }
                    Slider {
                        id: asrAdjustment
                        Layout.fillWidth: true
                        minimumValue: -20
                        maximumValue: 20
                        stepSize: 1
                    }
                    Switch {
                        id: asrEnabled
                        checked: true
                    }


                    Text {
                        text: qsTr("Magrib: ")
                    }
                    Text {
                        text: magribAdjustment.value.toFixed() > 0
                              ? "+" + magribAdjustment.value.toFixed()
                              : magribAdjustment.value.toFixed()
                    }
                    Slider {
                        id: magribAdjustment
                        Layout.fillWidth: true
                        minimumValue: -20
                        maximumValue: 20
                        stepSize: 1
                    }
                    Switch {
                        id: magribEnabled
                        checked: true
                    }

                    Text {
                        text: qsTr("Isha: ")
                    }
                    Text {
                        text: ishaAdjustment.value.toFixed() > 0
                              ? "+" + ishaAdjustment.value.toFixed()
                              : ishaAdjustment.value.toFixed()
                    }
                    Slider {
                        id: ishaAdjustment
                        Layout.fillWidth: true
                        minimumValue: -20
                        maximumValue: 20
                        stepSize: 1
                    }
                    Switch {
                        id: ishaEnabled         
                        checked: true
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

        RowLayout {
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
                            countries.append({"name": "", "code": ""}); //Speed up and cleaner, so no auto populate cities

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

                    Component.onCompleted: {
                        cbCountries.currentIndex = -1
                        cbCities.currentIndex = -1
                    }

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
                        text: "Mecca, Saudi Arabia"
                    }
                    Text {
                        text: qsTr("Latitude")
                    }
                    TextField {
                        id: tfLat
                        text: "21.42667"
                    }
                    Text {
                        text: qsTr("Longitude")
                    }
                    TextField {
                        id: tfLon
                        text: "39.82611"
                    }
                    Text {
                        text: qsTr("GMT Offset")
                    }
                    TextField {
                        id: tfGmt
                        property real value: 3.0
                        text: value
                        onTextChanged: value = text
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

        RowLayout {
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
                    Layout.fillWidth: true
                    model: ListModel {
                        ListElement {
                            text: "Majority - الاكثرية"
                        }
                        ListElement {
                            text: "Hanafi - حنفي"
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
                            text: "Egyptian GAS"
                        }
                        ListElement {
                            text: "Uni of Islamic Sciences (Shaf'i)"
                        }
                        ListElement {
                            text: "Uni of Islamic Sciences (Hanafi)"
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
                            text: "Fixed Isha Interval (always 90)"
                        }
                        ListElement {
                            text: "Egyptian GAS (Egypt)"
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

        RowLayout {
            ColumnLayout {
                SettingTitle {
                    text: qsTr("About Munadi")
                }
                SettingInfo {
                    property string link: "http://munadi.org/"
                    text: qsTr("Munadi - Simple Athan Program.")
                          + "<br/>"
                          + qsTr("For more info, visit: ")
                          + "<a href=\"http://munadi.org/\">Munadi.org</a>\n"
                          + "<br/>"
                          + qsTr("Version: ") + engine.getVersionNo();
                    onLinkActivated: Qt.openUrlExternally(link);
                }
            }
        }

        RowLayout {} // Bottom padding
    }
}
