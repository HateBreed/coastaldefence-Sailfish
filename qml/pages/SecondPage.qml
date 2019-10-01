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
    id: page2
    property int timerdelay: 10000
    property bool shot: false
    property bool destroyed: false
    property int hitx: 0
    property int hity: 0
    property int boxsize: GameEngine.tileWidth

    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaFlickable {
        anchors.fill: parent

        PageHeader {
            title: "Player" + GameEngine.getActivePlayer() + ": action " + GameEngine.getPlayerShot() + "/3"
            visible: shotinfo.visible ? false : true
        }
        SectionHeader {
            y: 60
            text: "Weapon: " + GameEngine.getShipActiveWeapon(GameEngine.getActiveShip())
            visible: shotinfo.visible ? false : true
        }

        // PullDownMenu and PushUpMenu must be declared in SilicaFlickable, SilicaListView or SilicaGridView
        PullDownMenu {
            visible: shotinfo.visible ? false : true
            enabled: shotinfo.visible ? false : true
            MenuItem {
                text: "Player" + GameEngine.getActivePlayer() + " end turn"
                onClicked: {
                    /*for (var i = 0; i < elements.count ; i++) {
                        var xp = 30 + 60 * (i % 9)
                        var yp = 30 + Math.floor( i / 9) * 60

                        maingrid.childAt(xp,yp).color = "transparent"
                        maingrid.childAt(xp,yp).radius = 0
                    }*/
                    if(pageStack.busy === false) {
                        GameEngine.gameStatePassTurn()
                        pageStack.clear()
                        if(GameEngine.getGameState() === 4) pageStack.push(Qt.resolvedUrl("Winner.qml"))
                        else pageStack.push(Qt.resolvedUrl("PlayerTurn.qml"))
                    }
                }
            }
        }

        // Tell SilicaFlickable the height of its content.
        contentHeight: GameEngine.gameAreaY + (2 * GameEngine.tileHeight)
        contentWidth: GameEngine.gameAreaX
        //VerticalScrollDecorator{}

        // Place our content in a Column.  The PageHeader is always placed at the top
        // of the page, followed by our content.
        Column {
            id: gridcolumn2
            x: 0
            y: 2 * GameEngine.tileHeight
            Grid {
                id: maingrid2
                columns: GameEngine.gameAreaX
                rows: GameEngine.gameAreaY
                spacing: 0
                Repeater {
                    id: elements2
                    model: GameEngine.gameAreaX * GameEngine.gameAreaY
                    delegate: ShootingBlock {
                    }
                }
            }
        }
        Rectangle {
            id: shotinfo
            visible: false
            width: parent.width
            height: 3 * GameEngine.tileHeight
            border.width: 0
            radius: 0
            border.color: Theme.secondaryColor
            color: Theme.secondaryHighlightColor
            x: 0
            y: (-2) * GameEngine.tileHeight

            Behavior on visible {
                NumberAnimation { target: shotinfo; property: "y"; from: (-2) * GameEngine.tileHeight; to: 0; duration: 500; easing.type: Easing.InOutQuad }
            }

            Label {
                id: shotinfolocation
                font.pixelSize: Theme.fontSizeExtraSmall
                color: Theme.primaryColor
                Text {
                    id: shotinfomain
                    text: "A shot has been placed"
                    font.pixelSize: Theme.fontSizeExtraSmall
                    color: Theme.secondaryColor
                    x: 5
                    y: 5
                }
                Text {
                    id: shotinfotext
                    text: ""
                    font.pixelSize: Theme.fontSizeExtraSmall
                    color: Theme.primaryColor
                    x: 5
                    y: 65
                }
                Text {
                    id: shotinfohit
                    text: ""
                    font.pixelSize: Theme.fontSizeExtraSmall
                    color: Theme.secondaryColor
                    x: 5
                    y: 125
                }
            }
        }

    }
    Timer {
        id: gobacktimer
        interval: timerdelay
        running: false
        repeat: false
        onTriggered: {
            if(GameEngine.getGameState() === 4) {
                pageStack.clear()
                pageStack.push(Qt.resolvedUrl("Winner.qml"))
            }
            else pageStack.navigateBack();
        }
    }

    Timer {
        id: changeplayertimer
        interval: timerdelay
        running: false
        repeat: false
        onTriggered: {
            if(pageStack.busy === false){
                pageStack.clear()
                if(GameEngine.getGameState() === 4) pageStack.push(Qt.resolvedUrl("Winner.qml"))
                else pageStack.push(Qt.resolvedUrl("PlayerTurn.qml"))
            }
        }
        onRunningChanged: {
            shotinfomain.text = GameEngine.getGameState() === 3 ? "All shots fired, changing player" : "Game ended, continuing to winner screen"
        }
    }

    Timer {
        id: markdestroyed
        interval: 100
        repeat: false
        running: destroyed
        onRunningChanged: {
            var realx = 0
            var realy = 0
            for (var i = 0; i < GameEngine.gameAreaX * GameEngine.gameAreaY ; i++) {

                realx = i % GameEngine.gameAreaX
                realy = Math.floor( i / GameEngine.gameAreaX)
                if(GameEngine.getAreaAt(realx,realy) === 2) {
                    maingrid2.childAt(realx * GameEngine.tileWidth, realy * GameEngine.tileHeight).color = "black"
                    console.log(realx + "," + realy + " is destroyed (" + realx * GameEngine.tileWidth + "," + realy * GameEngine.tileHeight + ") " + maingrid2.childAt(realx * GameEngine.tileWidth, realy * GameEngine.tileHeight).color)
                }
            }
            console.log("destroyrunning")
        }
    }
}
