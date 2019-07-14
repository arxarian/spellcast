import QtQuick 2.9
import QtQuick.Controls 2.5
import QtQuick.Window 2.2

Window {
    visible: true
    height: 960
    width: 540

    color: server.connected ? "gold" : "darkred"

    readonly property int splashScreenIndex: 0
    readonly property int loginScreenIndex: 1
    readonly property int selectorScreenIndex: 2
    readonly property int castScreenIndex: 3

    SwipeView {
        id: screensView
        anchors.fill: parent
        currentIndex: loginScreenIndex
        interactive: false

        SplashScreen {
            id: splashScreen
        }

        LoginScreen {
            id: loginScreen
        }

        SpellSelector {
            id: spellSelector
        }

        SpellArea {
            id: spellArea
        }
    }

    Connections {
        target: server
        onConnectionLostChanged: {
            if (server.connectionLost) {
                screensView.currentIndex = loginScreen
            }
        }
        onMessageReceived: {
            console.log("message received", message.type)

            if (message.type === "prepareSpells") {
                screensView.currentIndex = selectorScreenIndex
            }
            else if (message.type === "turnStart" ||
                     message.type === "turnEnd") {
                screensView.currentIndex = castScreenIndex
            }
        }
    }
}
