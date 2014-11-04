import QtQuick 2.0
import Sailfish.Silica 1.0

Rectangle {
    width: 60
    height: 60
    property int xcoordinate: index % GameEngine.gameAreaX()
    property int ycoordinate: Math.floor(index / GameEngine.gameAreaX())
    color: (GameEngine.getAreaAt(xcoordinate,ycoordinate) === 1 ? "red" : GameEngine.getAreaAt(xcoordinate,ycoordinate) === 2 ? "black" : GameEngine.getAreaAt(xcoordinate,ycoordinate) === 3 ? Theme.highlightColor : "transparent")
    border.color: "white"
    border.width: 1
    opacity: GameEngine.getAreaAt(xcoordinate,ycoordinate) < 1 ? 0.1 : 0.5
    radius: 0

    property int hitdamage: 0
    property string hittype: ""
    property string weapon: ""
    property int player: GameEngine.getActivePlayer();

    onColorChanged: {
        if(color === "black") console.log(xcoordinate + "," + ycoordinate + " color change to black")
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {

            //number.text = index
            if( shot === false)
            {
                switch(GameEngine.getGameState()) {
                case 2:
                    infotext2.text = "In game state 2 - should not happen"
                    break
                case 3:
                    weapon = GameEngine.getShipActiveWeapon(GameEngine.getActiveShip())

                    hitdamage = GameEngine.placeShot(xcoordinate,ycoordinate,GameEngine.getActiveShip(),GameEngine.getActiveWeapon());

                    textchangetimer.running = true

                    switch(hitdamage) {
                    case -3:
                        hittype = "NO SHIP TO SHOOT WITH"
                        console.log("No ship selected, should not happen")
                        break
                    case -2:
                        hittype = "YARR THIS SHIP BE BROKEN"
                        console.log("Tried to shoot with incapacited ship")
                        break
                    case -1:
                        hittype = "*PUFF* NO AMMO"
                        break
                    case 0:
                        hittype = "MISS"
                        parent.color = Theme.highlightColor
                        parent.opacity = 0.5
                        break
                    case 666:
                        hittype = "SHIP WAS DESTROYED"
                        parent.color = "black"
                        parent.opacity = 0.5
                        page2.destroyed = true
                        break
                    default:
                        hittype = "HIT"
                        parent.color = "red"
                        parent.opacity = 0.5
                        coloranimation.running = true
                        break;
                    }

                    shot = true

                    if(GameEngine.getActivePlayer() === player) {
                        if(GameEngine.getGameState() === 4) changeplayertimer.running = true
                        else gobacktimer.running = true
                    }
                    else changeplayertimer.running = true
                    break
                case 4:
                    changeplayertimer.running = true
                    break
                }
            }
        }
    }

    Behavior on color { ColorAnimation { duration: 500 } }

    Timer {
        id: textchangetimer
        interval: 800
        running: false
        repeat: false
        onTriggered: {
            shotinfotext.text = "Player" + player + " shot at " + xcoordinate + "," + ycoordinate
            shotinfohit.text = hitdamage === 666 ? hittype + " with " + weapon : hittype + " with " + weapon + " causing " + (hitdamage > 0 ? hitdamage : "no") + " damage"
            shotinfo.visible = true
        }
    }

    Timer {
        id: coloranimation
        interval: 600
        running: GameEngine.getShipAtArea(xcoordinate,ycoordinate)
        repeat: true
        property bool change: false
        onTriggered: {
            parent.color = change ? "orange" : "red"
            change = change ? false : true
        }
        onRunningChanged: {
            if(running === false) console.log("stopped")
        }
    }
}
