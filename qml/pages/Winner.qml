import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: pagemain
    SilicaFlickable {
        id: listView
        anchors.fill: parent
        PageHeader {
            title: "Winner of the game: Player" + GameEngine.getWinner()
        }
        Row {
            spacing: Theme.paddingLarge
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            Button {
                text: "Click to quit"
                onClicked: {
                    pageStack.clear()
                }
            }

        }
    }
}
