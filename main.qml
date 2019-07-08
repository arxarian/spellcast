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

        Component.onCompleted: {
            socket.connect("127.0.0.1", 9080)
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
