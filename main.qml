import QtQuick 2.9
import QtQuick.Controls 2.5
import QtQuick.Window 2.2

ApplicationWindow  {
    id: root

    visible: true
    height: 960
    width: 540

    color: server.connected ? "gold" : "darkred"

    readonly property int splashScreenIndex: 0
    readonly property int loginScreenIndex: 1
    readonly property int selectorScreenIndex: 2
    readonly property int castScreenIndex: 3
    readonly property int logsScreenIndex: 4

    property string requestedScreen
    property int requestredTabButtonIndex: 0

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

        Logs {}
    }

    footer: TabBar {
        id: bar
        width: parent.width

        Repeater {
            id: repeater
            model: ["Connection", "Arena", "Logs"]

            TabButton {
                id: button
                text: modelData
                enabled: server.connected === true || index !== 1
                width: Math.max(100, bar.width / repeater.count)
                onClicked: {
                    if (index === 0) {
                        screensView.currentIndex = loginScreenIndex
                    }
                    else if (index === 1) {
                        if (requestedScreen === "selector") {
                            screensView.currentIndex = selectorScreenIndex
                        }
                        else if (requestedScreen === "arena") {
                            screensView.currentIndex = castScreenIndex
                        }
                    }
                    else if (index === 2) {
                        screensView.currentIndex = logsScreenIndex
                    }
                }

                Connections {
                    target: root
                    onRequestredTabButtonIndexChanged: {
                        if (index === root.requestredTabButtonIndex) {
                            button.checked = true
                        }
                    }
                }
            }
        }
    }

    Connections {
        target: server
        onConnectedChanged: {
            if (server.connected === false) {
                screensView.currentIndex = loginScreenIndex
            }
        }
        onMessageReceived: {
            console.log("message received", message.type)

            if (message.type === "prepareSpells") {
                screensView.currentIndex = selectorScreenIndex
                requestedScreen = "selector"
                requestredTabButtonIndex = 1
                logs.append("prepareSpells")
            }
            else if (message.type === "turnStart" ||
                     message.type === "turnEnd") {
                screensView.currentIndex = castScreenIndex
                requestedScreen = "arena"
                requestredTabButtonIndex = 1
                logs.append("turnStart || turnEnd")
            }
        }
    }
}
