import QtQuick 2.9
import QtQuick.Layouts 1.3

Item {
    id: statArea

    Connections {
        target: server
        onMessageReceived: {
            if (message.type === "prepareSpells") {
                var ids = message.players.map(a => a.id)
                statView.ids = ids

                var names = message.players.map(a => a.name)
                statView.names = names

                var lifes = message.players.map(a => a.life)
                statView.lifes = lifes

                var lifesMax = message.players.map(a => a.lifeMax)
                statView.lifesMax = lifesMax

                statView.model = message.players.length

                console.log(statView.ids)
            }
            else if (message.type === "turnEnd") {
                var lifesUpdate = []
                for (var i = 0; i < message.players.length; ++i) {
                    var id = message.players[i].id
                    var playerIndex = statView.ids.indexOf(id)

                    if (playerIndex !== -1) {
                        lifesUpdate[playerIndex] = message.players[i].life
                    }
                }
                statView.lifes = lifesUpdate
            }
        }
    }

    Item {
        anchors.fill: parent
        anchors.topMargin: parent.height * 0.1
        anchors.bottomMargin: parent.height * 0.1

        ListView {
            id: statView

            property real delegateHeight: Math.max(0, height / count - spacing / 2)
            property real delegateWidth: statView.width
            property var names: []
            property var lifesMax: []
            property var lifes: []
            property var ids: []    // changed only once on prepareSpells

            anchors.fill: parent

            interactive: false
            spacing: 10

            model: 0

            delegate: PlayerStatus {
                height: statView.delegateHeight
                width: statView.delegateWidth

                playerId: statView.ids[index]
                name: statView.names[index]
                life: Math.max(0, statView.lifes[index])
                lifeMax: statView.lifesMax[index]
            }
        }
    }
}
