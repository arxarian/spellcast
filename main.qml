import QtQuick 2.9
import QtQuick.Window 2.2

import SpellCast 1.0

Window {
    visible: true
    width: 640
    height: 480

    MultiPointTouchArea {
        anchors.fill: parent

        touchPoints: [ TouchPoint { id: point1 } ]

        maximumTouchPoints: 1
        mouseEnabled: true  // debug

        onPressed: {
            console.log(point1.x, point1.y)
        }
        onReleased: {
            console.log(point1.x, point1.y)
        }
        onUpdated: {
            console.log(point1.x, point1.y)
        }
        onCanceled: {
            //
        }
    }

    SpellCast {
        anchors.fill: parent

        source: "qrc:/spells/spell0_ready.svg"
    }
}
