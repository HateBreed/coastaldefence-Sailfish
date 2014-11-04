import QtQuick 2.0
import Sailfish.Silica 1.0

Rectangle {
    width: battleship.width
    height: battleship.height
    x: GameEngine.getShipDirection(name) === "east" ? GameEngine.getShipX(name)*boxsize - (shiplength-1)*boxsize : GameEngine.getShipX(name)*boxsize
    y: GameEngine.getShipDirection(name) === "south" ? GameEngine.getShipY(name)*boxsize - (shiplength-1)*boxsize :GameEngine.getShipY(name)*boxsize
    color: "transparent"
    rotation: GameEngine.getShipDirection(name) === "south" ? 180 : 0
    border.width: 2
    border.color: activename === name ? Theme.highlightColor : "transparent"
    visible: GameEngine.getShipArmor(name) > 0

    property string name
    property bool timerenabled: false
    property int orientation: GameEngine.getShipOrientation(name)
    property int shiplength: GameEngine.getShipLength(name)
    Image {
        id: battleship
        source: orientation === 0 ? "../pics/" + GameEngine.getShipImage(name) : "../pics/" + GameEngine.getShipImage(name)
        width: orientation === 0 ? shiplength*boxsize : boxsize
        height: orientation === 0 ? boxsize : shiplength*boxsize
        rotation: GameEngine.getShipDirection(name) === "south" ? 180 : 0
        mirror: GameEngine.getShipDirection(name) === "east" ? true : false
        onRotationChanged: {
            console.log( parent.name + " my r: " + battleship.rotation + " x:" + battleship.x + " y: " + battleship.y)
        }
    }

    onRotationChanged: {
        console.log( name + " parent r:" + page.active.rotation + " x:" + page.active.x + " y: " + page.active.y)

    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            if(GameEngine.getShipArmor(parent.name) > 0) {
                if(activename === parent.name) {
                    if(activeclickedtwice) {
                        activeclickedtwice = false
                        infoenabled = true
                    }
                    else {
                        activeclickedtwice = true
                        shipinforetract = true
                    }

                }
                else {
                    active = parent
                    activename = name
                    GameEngine.setActiveShip(name)
                    activeclickedtwice = false
                    infoenabled = true
                }
            }
            //else labeltext.text = "INCAPACITED: " + labeltext.text
            console.log(name + " coordinates: " + GameEngine.getShipX(name) + "," + GameEngine.getShipY(name)  + "rotation: " + battleship.rotation + " " + GameEngine.getShipDirection(name) + (battleship.mirror ? " mirrored" : "normal"))
        }
        onDoubleClicked: {
            console.log("Doubleclicked " + name)
        }
    }

    Timer {
        id: imagechangetimer
        repeat: false
        running: timerenabled
        interval: 50
        onTriggered: {
            battleship.source = "../pics/" + GameEngine.getShipImage(name)
            battleship.width = GameEngine.getShipOrientation(name) === 0 ? shiplength*boxsize : boxsize
            battleship.height = GameEngine.getShipOrientation(name) === 0 ? boxsize : shiplength*boxsize
            switch(GameEngine.getShipDirection(name)) {
            case "north":
            //case "east":
            case "west":
                battleship.mirror = false
                //battleship.rotation = 0
                parent.rotation = 0
                parent.x = GameEngine.getShipX(name)*boxsize
                parent.y = GameEngine.getShipY(name)*boxsize
                break
            case "south":
                battleship.mirror = false
                //battleship.rotation = 180
                parent.rotation += 180
                parent.x = GameEngine.getShipX(name)*boxsize
                parent.y = GameEngine.getShipY(name)*boxsize - (shiplength-1)*boxsize
                break
            //case "west":
            case "east":
                battleship.mirror = true
                //battleship.rotation = 0
                parent.rotation = 0
                parent.x = GameEngine.getShipX(name)*boxsize - (shiplength-1)*boxsize
                parent.y = GameEngine.getShipY(name)*boxsize
                break
            }

            //battleship.update()
            timerenabled = false
        }
    }
}
