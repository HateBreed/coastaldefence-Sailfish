import QtQuick 2.0
import Sailfish.Silica 1.0

Column {
    width: parent.width/2
    height: parent.height
    property int itemtype
    property int weaponid: index+1
    property string rotatetext
    property int ammocount: activename.length > 0 ? GameEngine.getShipWeaponAmmo(activename,weaponid) : 0
    Button {
        id: w1
        text: itemtype === 0 ? GameEngine.getShipWeapon(activename,weaponid) : rotatetext
        width: parent.width
        height: boxsize
        opacity: 1.0
        enabled: itemtype === 0 ? (GameEngine.getGameState() === 3 && page.weapon !== weaponid && GameEngine.getShipWeaponAmmo(page.activename,weaponid) !== 0) :
                 active !== null && GameEngine.canShipRotate(page.activename,index)

        onClicked: {
            if(gamestate === 3 && itemtype === 0) {
                page.weapon = weaponid
                GameEngine.setActiveWeapon(weaponid)
            }
            else if(gamestate === 3 && itemtype === 1) {
                if(GameEngine.rotateShip(active.name,index)){
                    page.active.timerenabled = true
                    page.activeclickedtwice = true
                }
                retract.start()
            }

            else if (gamestate === 2 && itemtype === 1) {
                if(GameEngine.rotateShip(active.name,index)){
                    page.active.timerenabled = true
                    page.activeclickedtwice = true
                }
                retract.start()
            }
        }
    }
    Row {
        spacing: 4
        height: 15
        anchors.horizontalCenter: parent.horizontalCenter
        visible: itemtype === 0
        Repeater {
            model: ammocount === -1 ? 1 : ammocount
            Rectangle {
                height: 15
                width: ammocount === -1 ? 25 : 3
                color: ammocount === -1 ? "transparent" : Theme.primaryColor
                opacity: 1.0
                Text {
                    text: "∞"
                    visible: ammocount === -1
                    font.pixelSize: Theme.fontSizeSmall
                    font.bold: true
                    color: Theme.primaryColor
                    anchors.horizontalCenter: parent.horizontalCenter
                    y: -10
                }
            }
        }
    }

    Text {
        visible: itemtype === 0
        anchors.horizontalCenter: parent.horizontalCenter
        opacity: 1.0
        text:"Damage: " + GameEngine.getShipWeaponDamage(page.activename,weaponid)
        color: Theme.primaryColor
        font.pixelSize: Theme.fontSizeExtraSmall
    }
}
