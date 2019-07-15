import QtQuick 2.9
import QtQuick.Controls 2.2

Item {
    id: spellSelector

    property int spellsAmount: 0
    property var spellSelectorData
    property var selectedSpells: []

    function updateModel() {
        var count = internal.amount
        var names = []
        for (var i = 0; i < count.length; ++i) {
            if (count[i] > 0) {
                names.push(internal.names[i])
            }
        }

        internal.displayTexts = names.slice()

        for (var j = 0; j < names.length; ++j) {
            var indexOfSpell = internal.names.indexOf(names[j])
            if (indexOfSpell !== -1) {
                names[j] = "(" + count[indexOfSpell] + "x) " + names[j]
            }
        }

        names.unshift("NONE")
        internal.displayModel = names
    }

    onSpellSelectorDataChanged: {
        selectedSpells = []
        selectedSpells.length = spellSelector.spellsAmount

        internal.maxAmount = spellSelectorData.map(a => a.amount)
        internal.amount = spellSelectorData.map(a => a.amount)
        internal.ids = spellSelectorData.map(a => a.id)
        internal.names = spellSelectorData.map(a => a.name)

        updateModel()

        internal.spellsAmount = spellSelector.spellsAmount
    }

    Connections {
        target: server
        onMessageReceived: {
            if (message.type === "prepareSpells") {
                spellSelector.spellsAmount = message.spellsAmount
                spellSelector.spellSelectorData = message.spells

                timeoutBar.setTimeout(message.timeout)
            }
        }
    }

    QtObject {
        id: internal
        property int spellsAmount: 0
        property var maxAmount: []
        property var amount: []
        property var ids: []
        property var names: []
        property var displayModel: []
        property var displayTexts: []
    }

    Column {
        anchors.fill: parent
        anchors.margins: parent.width * 0.05
        spacing: 10

        TimeoutBar {
            id: timeoutBar

            height: parent.height / 15
            width: parent.width
        }

        ListView {
            id: spellSlots

            property int delegateHeight: height / 10

            height: parent.height - parent.anchors.margins * 2
            width: parent.width

            model: internal.spellsAmount

            spacing: 10
            delegate: ComboBox {
                id: comboBox
                height: spellSlots.delegateHeight
                width: spellSlots.width

                model: internal.displayModel
                currentIndex: 0

                onPressedChanged: {
                    if (pressed) {
                        var indexOfSpell = internal.names.indexOf(comboBox.displayText)
                        internal.amount[indexOfSpell]++

                        comboBox.model = null
                        updateModel()
                        comboBox.model = internal.displayModel
                    }
                }

                onCurrentIndexChanged: {
                    comboBox.displayText = internal.displayTexts[currentIndex - 1]

                    var indexOfSpell = internal.names.indexOf(comboBox.displayText)
                    internal.amount[indexOfSpell]--
                    spellSelector.selectedSpells[index] = internal.ids[indexOfSpell]

                    if (currentIndex > 0) {
                        server.sendSelectedSpells(selectedSpells)
                    }
                }
            }
        }
    }
}
