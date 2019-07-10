import QtQuick 2.9
import QtQuick.Window 2.2

import SocketCommunication 1.0

Window {
    visible: true
    width: 640
    height: 480

    color: "gold"

    SocketCommunication {
        id: socket

//        onConnectedChanged: socket.joinGame("ar")
        onMessageReceived: {
            console.log("message received", message.type)
            if (message.type === "prepareSpells") {
                spellSelector.visible = true
                spellSelector.spellsAmount = message.spellsAmount
                spellSelector.spellSelectorData = message.spells
            }
            else if (message.type === "turnStart") {
                spellSelector.visible = false
                spellArea.base64source = message.spell.svg
            }
        }

        Component.onCompleted: {
            socket.connect("127.0.0.1", 9080)
        }
    }

    SpellSelector {
        id: spellSelector
        z: 1
        visible: false
        anchors.fill: parent

        onConfirmed: {
            socket.selectedSpells(selectedSpells)
        }
    }

    SplashScreen {
        id: splashScreen
        anchors.fill: parent
        active: false
    }

    SpellArea {
        id: spellArea

        anchors.fill: parent
    }
}
