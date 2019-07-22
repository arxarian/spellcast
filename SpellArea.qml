import QtQuick 2.9
import QtQuick.Particles 2.0

import SpellCast 1.0

Item {
    id: spellArea

    property string spellType: "lightgray"
    property string spellId

    function resetArea() {
        text.visible = false
        touchArea.enabled = true
        spellCast.resetCompleteTime()
        spellCast.resetStats()
    }

    onSpellTypeChanged: {
        if (spellType === "attack") {
            spellCast.background = "darkred"
        }
        else if (spellType === "defense") {
            spellCast.background = "deepskyblue"
        }
        else {
            spellCast.background = "lightgray"
        }
    }

    Connections {
        target: server
        onMessageReceived: {
            if (message.type === "turnStart") {
                spellArea.resetArea()
                if (message.spell !== null) {
                    spellArea.spellId = message.spell.id
                    spellArea.spellType = message.spell.type
                    spellCast.base64source = message.spell.svg

                    timeoutBar.setTimeout(message.timeout)
                }
                else {
                    spellArea.spellId = ""
                    spellCast.reset()
                }
            }
            else if (message.type === "turnEnd") {
                if (spellArea.spellId.length > 0) {
                    text.visible = true
                }
            }
        }
    }

    Column {
        id: column
        z: 1
        spacing: 10
        anchors {
            top: parent.top
            topMargin: parent.width * 0.05
            left: parent.left
            leftMargin: parent.width * 0.05
            right: parent.right
            rightMargin: parent.width * 0.05
            bottom: spellCast.top
        }

        TimeoutBar {
            id: timeoutBar
            height: spellArea.height / 15
            width: parent.width
        }

        StatArea {
            id: statArea

            height: parent.height - parent.spacing - parent.height / 15 - column.parent.width * 0.1
            width: parent.width
        }
    }


    SpellCast {
        id: spellCast

        property int size: Math.min(parent.height, parent.width)

        anchors.centerIn: parent

        height: size
        width: size

        onSpellStatsChanged: {
            if (spellArea.spellId.length > 0) {

                var accuracy = Math.round(spellCast.spellStats.covered * 100)
                var penalty = Math.round(spellCast.spellStats.penalty * 10)
                var timeElapsedSpell = spellCast.spellStats.time
                var timeElapsedComplete = spellCast.spellStats.timeComplete

                var summary = "accuracy " + accuracy + " %\n"
                        + "time " + timeElapsedSpell + " ms"

                if (spellCast.spellStats.penalty > 0) {
                    summary += "\npenalty " + penalty + " %"
                }

                text.text = summary
                text.visible = true

                server.sendSpellCast(spellArea.spellId, accuracy, penalty, timeElapsedComplete, timeElapsedSpell)
            }
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
            id: touchArea

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
                touchArea.enabled = false
            }
            onCanceled: {
                spellCast.finalizeSpellPath()
                touchArea.enabled = false
            }
        }
    }
}
