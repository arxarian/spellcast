import QtQuick 2.9
import QtQuick.Controls 2.5

Item {
    Column {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        Flickable {
            height: parent.height * 14 / 15 - 20
            width: parent.width
            TextArea.flickable: TextArea {
                readOnly: true
                background: Rectangle { color: "lightgray" }
                text: logs.text
            }
        }

        Button {
            id: button
            text: "Clear logs"
            height: parent.height / 15
            width: parent.width

            onClicked: logs.clear()
        }
    }
}
