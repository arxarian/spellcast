import QtQuick 2.9

Rectangle {
    id: root

    property real to: 100
    property real value: 50
    property color foregroundColor: "green"
    property color backgroundColor: "darkred"

    border.width: 2
    color: root.backgroundColor

    Rectangle {
        x: 2
        y: 2
        height: parent.height - 4
        width: parent.width / root.to * root.value - 4

        color: root.foregroundColor
    }
}
