import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

RowLayout {
    spacing: 6
    layoutDirection: main.mirror === true ? "RightToLeft" : "LeftToRight"
}
