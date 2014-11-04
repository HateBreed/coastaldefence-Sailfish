import QtQuick 2.0
import Sailfish.Silica 1.0

Rectangle {
    width: 60
    height: 60
    property int xcoordinate: index % GameEngine.gameAreaX()
    property int ycoordinate: Math.floor(index / GameEngine.gameAreaX())
    color: (GameEngine.getMyAreaAt(xcoordinate,ycoordinate) === 1 ? "red" : GameEngine.getMyAreaAt(xcoordinate,ycoordinate) === 2 ? "black" : GameEngine.getMyAreaAt(xcoordinate,ycoordinate) === 3 ? Theme.highlightColor : "transparent")
    border.color: "white"
    border.width: 1
    opacity: GameEngine.getMyAreaAt(xcoordinate,ycoordinate) < 1 ? 0.1 : 0.5
    radius: 0

    property string hittype
    property int player: GameEngine.getActivePlayer();
    property int placement: 0

    MouseArea {
        anchors.fill: parent
        onClicked: {
            if(page.activename.length > 0)
            {
                if(GameEngine.getGameState() === 2 ) {
                    if((placement = GameEngine.placeShip(page.activename,xcoordinate,ycoordinate)) === 0) {
                        switch(GameEngine.getShipDirection(activename)) {
                                    case "north":
                                    //case "east":
                                    case "west":
                                        page.active.x = parent.x
                                        page.active.y = parent.y
                                        break
                                    case "south":
                                        page.active.x = parent.x
                                        page.active.y = parent.y - (GameEngine.getShipLength(activename)-1)*boxsize
                                        break
                                    //case "west":
                                    case "east":
                                        page.active.x = parent.x - (GameEngine.getShipLength(activename)-1)*boxsize
                                        page.active.y = parent.y
                                        break
                                    }
                        console.log(page.activename + " coords:" + xcoordinate + "," + ycoordinate + " direction:" +
                                GameEngine.getShipDirection(page.activename) + " " + GameEngine.getShipType(page.activename) +
                                ": " + GameEngine.getShipX(page.activename) + "," + GameEngine.getShipY(page.activename))
                    }
                    else console.log("Cannot set: (" + placement + ")" + page.activename + " coords:" + xcoordinate + "," +
                                     ycoordinate + " direction:" + GameEngine.getShipDirection(page.activename))
                }
                if(GameEngine.getGameState() === 3 ) {
                    shipinforetract = true
                    activeclickedtwice = true
                }
                else if(GameEngine.getGameState() === 2 ) {
                    shipinforetract = true
                    activeclickedtwice = true
                }
            }
        }
    }
}
