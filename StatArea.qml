import QtQuick 2.9
import QtQuick.Layouts 1.3

Item {
    id: statArea

    Connections {
        target: server
        onMessageReceived: {
            if (message.type === "turnEnd") {
                console.log(JSON.stringify(message))
            }
        }
    }

    ListView {
        id: statView

        property int delegateHeight: statView.height / 6
        property var lifes: [0, 0, 0]

        anchors.topMargin: parent.height / 6
        anchors.leftMargin: parent.width / 10
        anchors.rightMargin: parent.width / 10
        anchors.fill: parent

        interactive: false
        spacing: 10

        model: 2

        delegate: PlayerStatus {
            life: statView.lifes[index]
            height: statView.delegateHeight
            width: statView.width
        }
    }
}
