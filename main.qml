import QtQuick 2.9
import QtQuick.Window 2.2

Window {
    visible: true
    width: 640
    height: 480

    color: "gold"

    Component.onCompleted: {
//            socket.connect("162.168.43.134", 9080)
        server.connect("127.0.0.1", 9080)
    }

    Connections {
        target: server
        onMessageReceived: {
            console.log("message received", message.type)
        }
    }

    SpellSelector {
        id: spellSelector
        z: 1
        visible: false
        anchors.fill: parent

        onConfirmed: {
            server.selectedSpells(selectedSpells)
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
