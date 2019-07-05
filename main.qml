import QtQuick 2.9
import QtQuick.Window 2.2

import SpellCast 1.0

Window {
    visible: true
    width: 640
    height: 480

    color: "gold"

    Timer {
        id: timer

        property int spellIndex: 0

        interval: 3000
        onTriggered: {
            text.visible = false
            spellCast.source = "qrc:/spells/spell" + spellIndex + "_ready.svg"

            spellIndex = (spellIndex + 1) % 2
        }
    }

    SpellCast {
        id: spellCast

        property int size: Math.min(parent.height, parent.width)

        anchors.centerIn: parent

        height: size
        width: size

        source: "qrc:/spells/spell0_ready.svg"

        onCompletedChanged: {
            text.text = "accuracy " + Math.round(completed * 100) + " %\n"
                        + "time " + time + " ms"
            text.visible = true
        }

        Text {
            id: text
            anchors.centerIn: parent
            font.pixelSize: spellCast.size / 15
            visible: false
        }

        MultiPointTouchArea {
            anchors.fill: parent

            touchPoints: [ TouchPoint { id: point1 } ]

            maximumTouchPoints: 1
            mouseEnabled: true  // debug


            onPressed: {
                spellCast.initSpellPath(Qt.point(point1.x, point1.y))
            }
            onUpdated: {
                spellCast.updateSpellPath(Qt.point(point1.x, point1.y))
            }
            onReleased: {
                spellCast.finalizeSpellPath()
                timer.restart()
            }
            onCanceled: {
                spellCast.finalizeSpellPath()
            }
        }
    }
}
