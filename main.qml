import QtQuick 2.9
import QtQuick.Window 2.2

Window {
    visible: true
    width: 640
    height: 480

    color: "gold"

    SplashScreen {
        id: splashScreen

        anchors.fill: parent
    }

    SpellArea {
        id: spellArea

        anchors.fill: parent
    }
}
