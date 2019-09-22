/*
  Copyright (C) 2013 Jolla Ltd.
  Contact: Thomas Perl <thomas.perl@jollamobile.com>
  All rights reserved.

  You may use this file under the terms of BSD license as follows:

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Jolla Ltd nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR
  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

import QtQuick 2.0
import Sailfish.Silica 1.0


Page {
    id: page
    property var active: null
    property string activename: GameEngine.getActiveShip();
    property bool infoenabled: false
    property bool activeclickedtwice: false
    property int player: GameEngine.getActivePlayer();
    property int weapon: GameEngine.getActiveWeapon();
    property int textsize: 25
    property int boxsize: GameEngine.tileWidth
    property var shiplist: ["sub","ship","cruiser","carrier"]
    property bool shipinforetract: false

    onStatusChanged: {
        if(status === PageStatus.Active) {
            if(GameEngine.getActivePlayer() !== player) {
                pageStack.clear()
                if(GameEngine.getGameState() === 4) pageStack.push(Qt.resolvedUrl("Winner.qml"))
                else pageStack.push(Qt.resolvedUrl("PlayerTurn.qml"))
            }
            else if(GameEngine.getActivePlayer() === player) {
                active = null
                GameEngine.unsetActive()
                activename = GameEngine.getActiveShip()
                weapon = GameEngine.getActiveWeapon()

            }

            else if(GameEngine.getGameState() === 4) {
                pageStack.clear()
                pageStack.push(Qt.resolvedUrl("Winner.qml"))
            }
        }
    }

    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaFlickable {
        anchors.fill: parent

        PageHeader {
            title: "Player" + GameEngine.getActivePlayer() + ": " + (GameEngine.getGameStateString(GameEngine.getGameState()))
            visible: shipinfo.visible ? false : true
        }

        // PullDownMenu and PushUpMenu must be declared in SilicaFlickable, SilicaListView or SilicaGridView
        PullDownMenu {
            visible: shipinfo.visible ? false : true
            enabled: shipinfo.visible ? false : true
            MenuItem {
                text: "Player" + GameEngine.getActivePlayer() + (GameEngine.getGameState() === 3 ? ", stop shooting" :", end turn")
                onClicked: {
                    /*for (var i = 0; i < elements.count ; i++) {
                        var xp = 30 + 60 * (i % 9)
                        var yp = 30 + Math.floor( i / 9) * 60

                        maingrid.childAt(xp,yp).color = "transparent"
                        maingrid.childAt(xp,yp).radius = 0
                    }*/
                    //labeltext.text = "Player" + GameEngine.getActivePlayer + "Placing turn"
                    if(!pageStack.busy) {
                        if(GameEngine.getGameState() === 2) GameEngine.gameStateProgress()
                        else if(GameEngine.getGameState() === 3) GameEngine.gameStatePassTurn()
                        pageStack.clear();
                        if(GameEngine.getGameState() === 4) pageStack.push(Qt.resolvedUrl("Winner.qml"))
                        else pageStack.push(Qt.resolvedUrl("PlayerTurn.qml"))
                    }

                }
            }
        }
//        Dialog {
//            canAccept: true
//            canNavigateForward: true
//            onAccepted: {
//                if(GameEngine.getGameState() === 2) GameEngine.gameStateProgress()
//                else if(GameEngine.getGameState() === 3) GameEngine.gameStatePassTurn()
//                pageStack.clear();
//                if(GameEngine.getGameState() === 4) pageStack.push(Qt.resolvedUrl("Winner.qml"))
//                else pageStack.push(Qt.resolvedUrl("PlayerTurn.qml"))
//            }
//        }

//        PushUpMenu {
//            MenuItem {
//                text: "Player" + GameEngine.getActivePlayer() + (GameEngine.getGameState() === 3 ? ", stop shooting" :", end turn")
//                onClicked: {
//                    if(GameEngine.getGameState() === 2) GameEngine.gameStateProgress()
//                    else if(GameEngine.getGameState() === 3) GameEngine.gameStatePassTurn()
//                    pageStack.clear();
//                    pageStack.push(Qt.resolvedUrl("PlayerTurn.qml"))

//                }
//            }
//        }

        // Tell SilicaFlickable the height of its content.
        contentHeight: GameEngine.boardHeight + 2 * GameEngine.tileHeight
        contentWidth: GameEngine.boardWidth
        //VerticalScrollDecorator{}
        //HorizontalScrollDecorator{}

        // Place our content in a Column.  The PageHeader is always placed at the top
        // of the page, followed by our content.
        Column {
            id: gridcolumn
            x: 0
            y: 2 * GameEngine.tileHeight
            Grid {
                id: maingrid
                columns: GameEngine.gameAreaX
                rows: GameEngine.gameAreaY
                spacing: 0
                Repeater {
                    id: elements
                    model: GameEngine.gameAreaX * GameEngine.gameAreaY
                    delegate: PlacingBlock {
                    }
                }
            }
            Item {
                Repeater {
                    id: ships
                    model: page.shiplist
                    delegate: Ship {
                        name: page.shiplist[index]
                    }
                }
            }
        }
        ShipInfo {
            id: shipinfo
        }
    }
}
