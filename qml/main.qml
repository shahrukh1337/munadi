import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import Qt.labs.settings 1.0

Rectangle {
    id: main
    color: "#EFEBD6"

    property bool mirror: engine.isRtl();
    property int margin: width / 50
    LayoutMirroring.enabled: mirror
    LayoutMirroring.childrenInherit: true

    Extras {
        id: utils
        anchors.fill: parent
        anchors.margins: parent.margin
        LayoutMirroring.enabled: true
    }
}
