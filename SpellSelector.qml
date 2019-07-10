import QtQuick 2.9
import QtQuick.Controls 2.2

Item {
    id: spellSelector

    signal confirmed()

    property int spellsPerTurn: 0
    property var spellSelectorData
    property var selectedSpells: []

    function updateModel() {
        var count = internal.count
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
        internal.maxCount = spellSelectorData.map(a => a.amount)
        internal.count = spellSelectorData.map(a => a.amount)

        internal.names = spellSelectorData.map(a => a.name)
        console.log(internal.count, internal.names, spellSelector.spellsPerTurn)

        updateModel()

        internal.spellsPerTurn = spellSelector.spellsPerTurn
    }

    QtObject {
        id: internal
        property int spellsPerTurn: 0
        property var maxCount: []
        property var count: []
        property var names: []
        property var displayModel: []
        property var displayTexts: []
    }

    Column {
        anchors.fill: parent
        anchors.margins: parent.width * 0.05
        spacing: 10

        Button {
            id:  confirmButton
            height: parent.height / 10
            width: parent.width
            text: "Confirm"

            onClicked: {
                console.log(selectedSpells)
                confirmed()
            }
        }

        ListView {
            id: spellSlots

            property int delegateHeight: height / 10

            height: parent.height - confirmButton.height - parent.anchors.margins * 2
            width: parent.width

            model: internal.spellsPerTurn

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
                        internal.count[indexOfSpell]++

                        comboBox.model = null
                        updateModel()
                        comboBox.model = internal.displayModel
                    }
                }

                onCurrentIndexChanged: {
                    comboBox.displayText = internal.displayTexts[currentIndex - 1]
                    spellSelector.selectedSpells[index] = comboBox.displayText

                    var indexOfSpell = internal.names.indexOf(comboBox.displayText)
                    internal.count[indexOfSpell]--
                }
            }
        }
    }
}
