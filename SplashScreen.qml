import QtQuick 2.9
import SpellCast 1.0

Item {
    id: splashScreen

    Loader {
        id: loader
        anchors.centerIn: parent
        sourceComponent: SpellCast {
            id: spellCast

            property int size: Math.min(splashScreen.height, splashScreen.width)

            height: size
            width: size

            source: "qrc:/logo/logo.svg"
        }
    }
}
