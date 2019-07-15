import QtQuick 2.9

Progressbar {
    id: timeoutBar

    function setTimeout(timeout_ms) {
        behavior.enabled = false
        timeoutBar.value = 0
        behavior.duration = timeout_ms
        behavior.enabled = true
        timeoutBar.value = timeoutBar.to
    }

    to: 1
    foregroundColor: "gold"
    backgroundColor: "lightgray"

    Behavior on value {
        id: behavior
        property real duration
        NumberAnimation {
            duration: behavior.duration
        }
    }
}
