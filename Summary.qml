import QtQuick 2.9
import QtQuick.Controls 2.5

Item {
    id: summary

    signal close()
    property string playerName
    property bool winner: false

    Text {
        id: text
        anchors.centerIn: parent
        visible: false

        color: summary.winner ? "green" : "black"
        font.pixelSize: summary.width / 15
    }

    Button {
        id: button

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20

        text: "Close"
        height: parent.height / 15
        width: parent.width - 40

        onClicked: summary.close()
    }

    Connections {
        target: server
        onMessageReceived: {
            if (message.type === "sessionEnd") {
                text.visible = false
                text.text = "You are a loser!" + summary.playerName

                var players = message.players

                for (var i = 0; i < players.length; ++i) {
                    if (players[i].name === summary.playerName && players[i].winner) {
                        summary.winner = true
                        text.text = "You are a winner!\n\nLifes left: " + players[i].life
                        break
                    }
                }
                text.visible = true
            }
        }
    }
}
