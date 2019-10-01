import QtQuick 2.0
import Sailfish.Silica 1.0

Rectangle {
    border.width: 0
    border.color: Theme.secondaryColor
    color: Theme.secondaryHighlightColor
    opacity: 1.0
    enabled: page.infoenabled ? true : false
    visible: page.infoenabled ? true : false
    width: parent.width
    height: gamestate === 3 ? boxsize*(lengthmultiplicator+1) : boxsize*lengthmultiplicator
    radius: 0
    x: 0
    y: boxsize*lengthmultiplicator*(-1)
    property bool hidden: true
    property int lengthmultiplicator: 7
    property int gamestate: GameEngine.getGameState()
    property var rotatestring: ["ROTATE CCW","ROTATE CW"]

    MouseArea {
        anchors.fill: parent
        onClicked: {
            //if(gamestate === 2) {
                page.shipinforetract = true
                page.activeclickedtwice = true
            //}
        }
    }

    Timer {
        id: retract
        interval: 50
        repeat: true
        running: page.shipinforetract
        onTriggered: {
            parent.y -= boxsize
            if(parent.y <= boxsize*lengthmultiplicator*(-1)) {
                stop()
                //page.active = null
                //page.activename = ""
                //page.weapon = 0
                //GameEngine.unsetActive()
                infoenabled = false;
                page.shipinforetract = false
            }
        }
    }

    Behavior on visible {
        NumberAnimation {
            id: nanim
            target: shipinfo
            property: "y"
            from: boxsize*lengthmultiplicator*(-1)
            to: 0
            duration: 500
        }
    }
    Text {
        x: 10
        y: 0
        opacity: 1.0
        text: "Ship: " + GameEngine.getShipType(page.activename)
        color: Theme.primaryColor
        font.pixelSize: Theme.fontSizeExtraSmall
        font.bold: true
    }

    Text {
        x: 10
        y: 30
        opacity: 1.0
        text: "Armor: " + GameEngine.getShipArmor(page.activename) + "/" + GameEngine.getShipArmorOriginal(page.activename);
        color: Theme.primaryColor
        font.pixelSize: Theme.fontSizeExtraSmall
    }


    Column {
        id: iconButtons
        spacing: Theme.paddingLarge
        opacity: 1.0
        x: 10
        y: boxsize
        width: GameEngine.gameAreaX * boxsize
        height: boxsize * 5 + 20
        Label {
            text: "WEAPONS"
        }

        Row {
            width: parent.width
            height: boxsize*2
            Repeater {
                model: 2
                InfoItem {
                    rotatetext: rotatestring[index]
                    itemtype: 0
                }
            }
        }
        Column {
            height: boxsize
            width: parent.width
            visible: gamestate === 3
            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                id: shoot
                text: "SHOOT"
                height: boxsize
                onClicked: {
                    if (gamestate === 3) {
                        //page.active = null
                        //page.activename = ""
                        page.weapon = 0
                        page.activeclickedtwice = true
                        page.shipinforetract = true
                        page.infoenabled = false
                        pageStack.push(Qt.resolvedUrl("SecondPage.qml"))
                    }
                }
                enabled: page.weapon > 0
            }
        }
        Label {
            text: "DIRECTION: " + GameEngine.getShipDirection(activename)
        }

        Row {
            width: parent.width
            height: boxsize*2
            Repeater {
                model: 2
                InfoItem {
                    rotatetext: rotatestring[index]
                    itemtype: 1
                }
            }
        }
    }
}

