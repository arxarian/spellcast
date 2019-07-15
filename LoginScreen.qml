import QtQuick 2.9
import QtQuick.Controls 2.2
import Qt.labs.settings 1.0
import QtQuick.Controls 2.5

Item {
    id: loginScreen

    property string ip
    property string port

    property alias username: usernameField.text

    function parseIpAdressPort() {
        var separatorIndex = ipAdrressField.text.indexOf(":")
        if (separatorIndex !== -1) {
            loginScreen.ip = ipAdrressField.text.substr(0, separatorIndex)
            loginScreen.port = ipAdrressField.text.substr(separatorIndex + 1)
        }
    }

    Settings {
        id: settings
        property string ip
        property string port
        property string username
    }

    Column {
        id: column

        property int delegateWidth: parent.width * 0.8

        anchors.centerIn: parent

        spacing: parent.height * 0.05

        TextField {
            id: usernameField
            enabled: server.connected === false && server.connecting === false
            width: column.delegateWidth
            text: settings.username
            placeholderText: qsTr("user name")
            horizontalAlignment: TextInput.AlignHCenter
        }

        TextField {
            id: ipAdrressField
            enabled: server.connected === false && server.connecting === false
            width: column.delegateWidth
            placeholderText: qsTr("ip_adrress:port")
            horizontalAlignment: TextInput.AlignHCenter

            onTextChanged: parseIpAdressPort()
            Component.onCompleted: {
                if (settings.ip.length > 0 && settings.port.length > 0) {
                    text = settings.ip + ":" + settings.port
                }
            }
        }

        Button {
            enabled: ip.length > 0 && port.length > 0 && username.length > 0 &&
                     server.connected === false && server.connecting === false
            text: qsTr("Connect")
            width: column.delegateWidth
            onClicked: {
                settings.ip = loginScreen.ip
                settings.port = loginScreen.port
                settings.username = loginScreen.username

                server.setUserName(username)
                server.connect(loginScreen.ip, loginScreen.port)
            }
        }

        Button {
            visible: server.connectionLost
            text: qsTr("Reconnect")
            width: column.delegateWidth
            onClicked: {
                server.reconnect()
            }
        }
    }
}
