import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: pagemain
    SilicaFlickable {
        id: listView
        anchors.fill: parent
        PageHeader {
            title: "Turn of: Player" + GameEngine.getActivePlayer()
        }
        Row {
            spacing: Theme.paddingLarge
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            Button {
                text: "Go to " + GameEngine.getGameStateString(GameEngine.getGameState()) + " mode"
                onClicked: {
                    pageStack.clear()
                    pageStack.push(Qt.resolvedUrl("FirstPage.qml"))
                }
            }

        }
    }
}
