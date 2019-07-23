import QtQuick 2.9
import QtQuick.Layouts 1.3

Item {
    id: playerStatus

    property string playerId
    property string name
    property int lifeMax: 200
    property int life: 100

    RowLayout {
        anchors.fill: parent
        spacing: 10

        Item {
            Layout.fillHeight: true
            Layout.preferredWidth: parent.width / 4

            Text {
                anchors.centerIn: parent

                text: playerStatus.name

                elide: Text.ElideRight
                font.bold: true
                font.pixelSize: parent.height / 2
            }
        }

        Progressbar {
            value: playerStatus.life
            to: playerStatus.lifeMax

            Layout.fillHeight: true
            Layout.fillWidth: true

            Behavior on value {
                NumberAnimation {
                    easing.type: Easing.OutQuint
                    duration: 1500
                }
            }
        }
    }
}
