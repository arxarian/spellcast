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

        onConnectedChanged: socket.joinGame("ar")
        onMessageReceived: {
            console.log("message received", message.type)
            if (message.type === "prepareSpells") {
                spellSelector.spellSelectorData = message.data
            }
        }

        Component.onCompleted: {
            socket.connect("127.0.0.1", 9080)
        }
    }

    SpellSelector {
        id: spellSelector
        z: 2
        anchors.fill: parent

        onConfirmed: {
            socket.selectedSpells(selectedSpells)
        }
    }

    SplashScreen {
        id: splashScreen

        anchors.fill: parent
    }

    SpellArea {
        id: spellArea

        anchors.fill: parent
    }
}
