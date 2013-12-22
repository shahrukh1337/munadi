import QtQuick 2.2
import QtQuick.Layouts 1.1

Table {
    id: table
    Layout.column: main.mirror === true ? 1 : 0
}
