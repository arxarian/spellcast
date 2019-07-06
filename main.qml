import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Particles 2.0

import SpellCast 1.0

Window {
    visible: true
    width: 640
    height: 480

    color: "gold"

    Timer {
        id: timer
        interval: 3000
        onTriggered: {
            text.visible = false
            spellCast.reset()
        }
    }

    SpellCast {
        id: spellCast

        property int size: Math.min(parent.height, parent.width)

        anchors.centerIn: parent

        height: size
        width: size

        source: "qrc:/spells/spell0_ready.svg"

        onSpellStatsChanged: {
            var spellSummary = "accuracy " + Math.round(spellCast.spellStats.covered * 100) + " %\n"
                        + "time " + spellCast.spellStats.time + " ms"

            if (spellCast.spellStats.penalty > 0) {
                spellSummary += "\npenalty " + Math.round(spellCast.spellStats.penalty * 10) + " %"
            }

            text.text = spellSummary
            text.visible = true
        }

        Text {
            id: text
            z: 1
            anchors.centerIn: parent
            font.pixelSize: spellCast.size / 15
            visible: false
        }

        ParticleSystem {
            id: sys
            anchors.fill: parent
            onEmptyChanged: if (empty) sys.pause();

            ImageParticle {
                system: sys
                id: cp
                source: "qrc:///particleresources/glowdot.png"
                colorVariation: 0.4
                color: "#000000FF"
            }

            Emitter {
                //burst on click
                id: bursty
                system: sys
                enabled: false
                emitRate: 2000
                lifeSpan: 2500
                acceleration: AngleDirection {angleVariation: 360; magnitude: 1; }
                size: 30
                endSize: 0
                sizeVariation: 4
            }
        }

        MultiPointTouchArea {
            anchors.fill: parent

            touchPoints: [ TouchPoint { id: point1 } ]

            maximumTouchPoints: 1
            mouseEnabled: true  // debug


            onPressed: {
                spellCast.initSpellPath(Qt.point(point1.x, point1.y))
                bursty.x = point1.x
                bursty.y = point1.y
                sys.resume()
                bursty.enabled = true
            }
            onUpdated: {
                spellCast.updateSpellPath(Qt.point(point1.x, point1.y))
                bursty.x = point1.x
                bursty.y = point1.y
            }
            onReleased: {
                spellCast.finalizeSpellPath()
                bursty.enabled = false
                timer.restart()
            }
            onCanceled: {
                spellCast.finalizeSpellPath()
            }
        }
    }
}
