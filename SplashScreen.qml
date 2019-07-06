import QtQuick 2.9
import SpellCast 1.0

Item {
    id: splashScreen

    readonly property bool active: loader.active

    z: 1

    Timer {
        interval: 2000
        running: true
        onTriggered: {
            splashScreen.z = 0
            loader.active = false
        }
    }

    Loader {
        id: loader
        anchors.centerIn: parent
        sourceComponent: SpellCast {
            id: spellCast

            property int size: Math.min(splashScreen.height, splashScreen.width)

            height: size
            width: size

            source: "qrc:/logo/logo.svg"

            MultiPointTouchArea {
                anchors.fill: parent
            }
        }
    }
}
