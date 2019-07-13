import QtQuick 2.9
import QtQuick.Layouts 1.3

Item {
    id: playerStatus

    property string playerId
    property string name
    property int lifeMax: 200
    property int life: (index + 1) * 50

    RowLayout {
        anchors.fill: parent
        spacing: 10

        Text {
            text: playerStatus.name

            elide: Text.ElideRight
            font.bold: true
            font.pixelSize: parent.height / 2
            horizontalAlignment: Text.AlignVCenter
            verticalAlignment: Text.AlignHCenter

            Layout.fillHeight: true
            Layout.preferredWidth: parent.width / 5
        }

        Rectangle {
            border.width: 2
            color: "darkred"

            Layout.fillHeight: true
            Layout.fillWidth: true

            Rectangle {
                x: 2
                y: 2
                height: parent.height - 4
                width: parent.width / playerStatus.lifeMax * playerStatus.life - 4

                color: "green"

                Behavior on width {
                    NumberAnimation {
                        easing.type: Easing.OutQuint
                        duration: 1500
                    }
                }
            }
        }
    }
}
