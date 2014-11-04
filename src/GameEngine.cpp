#include <assert.h>
#include <QtDebug>
#include "GameEngine.h"
#include <stdio.h>

#define __SHIP_HIT 1
#define __SHIP_DESTROYED 2
#define __SHIP_MISS 3

#define __PLACE_OK 0
#define __PLACE_NO_SHIP 1
#define __PLACE_POSITION_TAKEN 2

#define __HIT_NOSHIP -3
#define __HIT_INCAPACITED -2
#define __HIT_NOAMMO -1
#define __HIT_MISS 0
#define __HIT_DESTROYED 666

GameEngine* GameEngine::iEngine = NULL;

GameEngine::GameEngine(QObject *parent) :
    QObject(parent)
{
        iGameareaX = GAMEX;
        iGameareaY = GAMEY;

        for(int i = 0; i < GAMEX; i++) {
            for(int j = 0; j < GAMEY; j++)
                iPlayer1Area[i][j] = iPlayer2Area[i][j] = 0;
        }

         iPlayer1ships[0] = new Ship(BATTLESHIP);
         iPlayer1ships[1] = new Ship(SUBMARINE);
         iPlayer1ships[2] = new Ship(CRUISER);
         iPlayer1ships[3] = new Ship(CARRIER);

         iPlayer2ships[0] = new Ship(BATTLESHIP);
         iPlayer2ships[0]->setCoordinates(iPlayer2ships[0]->getX(),GAMEY-iPlayer2ships[0]->getY()-1);
         iPlayer2ships[1] = new Ship(SUBMARINE);
         iPlayer2ships[1]->setCoordinates(iPlayer2ships[1]->getX(),GAMEY-iPlayer2ships[1]->getY()-1);
         iPlayer2ships[2] = new Ship(CRUISER);
         iPlayer2ships[2]->setCoordinates(iPlayer2ships[2]->getX(),GAMEY-iPlayer2ships[2]->getY()-1);
         iPlayer2ships[3] = new Ship(CARRIER);
         iPlayer2ships[3]->setCoordinates(iPlayer2ships[3]->getX(),GAMEY-iPlayer2ships[3]->getY()-1);


         for(int i = 0; i < GAMETURNS; i++) {
             iPlayer1Turns[i] = new Action();
             iPlayer2Turns[i] = new Action();
         }

         iActivePlayer = 1;
         iGamestate = STARTED;

         iPlayer1Placed = 0;
         iPlayer2Placed = 0;
         iPlayer1Shot = 0;
         iPlayer2Shot = 0;
         iActiveWeapon = 0;
         iActiveShip = "";
         iWinner = 0;
#ifdef QT_QML_DEBUG
         iDebug = true;
#else
         iDebug = false;
#endif
}

GameEngine* GameEngine::initEngine() {
    if(!iEngine) iEngine = new GameEngine;
    return iEngine;
}

qint16 GameEngine::placeShip(QString _id, qint16 _x, qint16 _y) {
    Ship* ship = getMyShip(_id);
    if(ship) {
        if(!checkPosition(ship,_x,_y)) return __PLACE_POSITION_TAKEN;
        else {
            ship->place(_x,_y);
            return __PLACE_OK;
        }
    }
    return __PLACE_NO_SHIP;
}

Ship* GameEngine::getMyShip(QString _id) {
    for(qint16 i = 0; i < SHIPCOUNT; i++) {
        Ship* ship = NULL;
        if(iActivePlayer == 1) ship = iPlayer1ships[i];
        else ship = iPlayer2ships[i];
        if(_id == ship->getName()) return ship;
    }
    return NULL;
}

qint16 GameEngine::getShipX(QString _id)
{
    Ship* ship = getMyShip(_id);
    if(ship) return ship->getX();
    return 0;
}

qint16 GameEngine::getShipY(QString _id)
{
    Ship* ship = getMyShip(_id);
    if(ship) return ship->getY();
    return 0;
}

qint16 GameEngine::getShipArmor(QString _id)
{
    Ship* ship = getMyShip(_id);
    if(ship) return ship->getArmor();
    return 0;
}

qint16 GameEngine::getShipArmorOriginal(QString _id)
{
    Ship* ship = getMyShip(_id);
    if(ship) return ship->getArmorOriginal();
    return 0;
}

QString GameEngine::getShipActiveWeapon(QString _id) {
    return getShipWeapon(_id,iActiveWeapon);
}

qint16 GameEngine::getShipWeaponDamage(QString _id, qint16 _weapon)
{
    Ship *ship = getMyShip(_id);
    if(ship) return ship->getWeapon(_weapon)->getDamage();
    return 0;
}

QString GameEngine::getShipWeapon(QString _id, qint16 _type)
{
    Ship* ship = getMyShip(_id);
    if(ship) return ship->getWeapon(_type)->getName();
    return "";
}

qint16 GameEngine::getShipWeaponAmmo(QString _id, qint16 _type)
{
    Ship* ship = getMyShip(_id);
    if(ship) return ship->getWeaponAmmo(_type);
    return 0;
}

qint16 GameEngine::getShipSpeed(QString _id)
{
    Ship* ship = getMyShip(_id);
    if(ship) return ship->getSpeed();
    return 0;
}

QString GameEngine::getShipDirection(QString _id)
{
    Ship* ship = getMyShip(_id);
    if(ship) {
        switch(ship->getDirection()) {
        case NORTH:
            return "north";
        case EAST:
            return "east";
        case SOUTH:
            return "south";
        case WEST:
            return "west";
        default:
            return "no direction???" + QString(ship->getDirection());
        }

    }
    return "ship null";
}

qint16 GameEngine::getShipOrientation(QString _id)
{
    Ship* ship = getMyShip(_id);
    if(ship) {
        switch(ship->getDirection()) {
        case NORTH:
        case SOUTH:
            return 1;
        case EAST:
        case WEST:
            return 0;
        }
    }
    return 0;

}

QString GameEngine::getShipType(QString _id)
{
    Ship* ship = getMyShip(_id);
    if(ship) return ship->getVisibleName();
    return "";
}

bool GameEngine::getShipMoveState(QString _id)
{
    Ship* ship = getMyShip(_id);
    return ship->getMoving();
}

qint16 GameEngine::getShipLength(QString _id)
{
    Ship* ship = getMyShip(_id);
    if(ship) return ship->getLength();
    else return 0;
}

QString GameEngine::getShipImage(QString _id)
{
    Ship* ship = getMyShip(_id);
    if(ship) return ship->getImage();
    else return "battleship-ger.png";
}

bool GameEngine::rotateShip(QString _id, qint16 _type)
{
    Ship* ship = getMyShip(_id);

    if(ship) {
        if(canShipRotate(_id, _type)) {
            ship->rotate(_type);
            return true;
        }
    }
    return false;
}

bool GameEngine::canShipRotate(QString _id, qint16 _type)
{
    Ship* ship = getMyShip(_id);
    qint16 rotateback = _type == 0 ? 1 : 0;
    bool rval = true;
    if(ship) {
        ship->rotate(_type);
        if(!checkPosition(ship,ship->getX(),ship->getY())) rval = false;
        ship->rotate(rotateback);
    }
    return rval;
}


qint16 GameEngine::placeShot(qint16 _x, qint16 _y, QString _fromShip, qint16 _weaponid) {

    Ship* fromship = getMyShip(_fromShip);
    Weapon* weapon = fromship->getWeapon(_weaponid);
    hitType hittype = MISS;

    qint16 rval = 0;
    qint16 mapval = 0;

    if(fromship) {
        if(fromship->getArmor() <= 0) return __HIT_INCAPACITED;
        else if(weapon->getAmmo() == 0) return __HIT_NOAMMO;
        else {
            Ship* ship = NULL;
            weapon->shoot();
            if((ship = getShipAt(_x,_y)) == NULL) {
                rval = __HIT_MISS;
                mapval = __SHIP_MISS;
            }
            else {
                qint16 shipblockid = ship->getBlockId(_x,_y);
                printf("block: %d, hits:%hu\n",shipblockid,ship->getHits(shipblockid));
                if(ship->getHits(shipblockid) == 0) hittype = HIT;
                else hittype = HITAGAIN;

                qint16 damage = ship->hit(hittype,weapon,_x,_y);
                if(damage == -1) {
                    rval = __HIT_DESTROYED;
                    mapval = __SHIP_DESTROYED;
                    markShipAsDestroyed(ship);
                }
                else {
                    rval = damage;
                    mapval = __SHIP_HIT;
                }
            }
        }
    }
    else return __HIT_NOSHIP;

    if(iActivePlayer == 1 ) iPlayer1Area[_x][_y] = mapval;
    else iPlayer2Area[_x][_y] = mapval;
    gameStateShoot();

    return rval;
}

QString GameEngine::getDestroyedShipImage(qint16 _x, qint16 _y)
{
    Ship* ship = getShipAt(_x,_y);
    if(ship->getArmor() <= 0) return ship->getDestroyedImage();
    return "";
}

QString GameEngine::getDestroyedShipName(qint16 _x, qint16 _y)
{
    Ship* ship = getShipAt(_x,_y);
    if(ship->getArmor() <= 0) return ship->getVisibleName();
    return "";
}

bool GameEngine::checkPosition(Ship* _ship, qint16 _x, qint16 _y) {
    switch(_ship->getDirection()) {
    case NORTH: //up
        if(_y + _ship->getLength() > GAMEY) return false;
        for(int i = 0; i < _ship->getLength(); i++) {
            if(getMyShipAt(_x, _y+i,_ship)) return false;
        }
        break;
    //case EAST: //left
    case WEST:
        if(_x + _ship->getLength() > GAMEX) return false;
        for(int i = 0; i < _ship->getLength(); i++) {
            if(getMyShipAt(_x+i, _y,_ship)) return false;
        }
        break;
    case SOUTH: //down
        if(_y+1 - _ship->getLength() < 0) return false;
        for(int i = 0; i < _ship->getLength(); i++) {
            if(getMyShipAt(_x, _y-i,_ship)) return false;
        }
        break;
    //case WEST: //right
    case EAST:
        if(_x+1 - _ship->getLength() < 0) return false;
        for(int i = 0; i < _ship->getLength(); i++) {
            if(getMyShipAt(_x-i, _y,_ship)) return false;
        }
        break;
    }
    return true;
}

void GameEngine::initGameArea() {

}
qint16 GameEngine::getPlayerShot() const
{
    if(iActivePlayer == 1) return iPlayer1Shot+1;
    else return iPlayer2Shot+1;
}

void GameEngine::setActiveShip(QString _name)
{
    iActiveShip = QString(_name);
}

QString GameEngine::getActiveShip()
{
    return iActiveShip;
}

void GameEngine::setActiveWeapon(qint16 _weapon)
{
    iActiveWeapon = qint16(_weapon);
}

qint16 GameEngine::getActiveWeapon()
{
    return iActiveWeapon;
}

void GameEngine::unsetActive()
{
    iActiveShip = "";
    iActiveWeapon = 0;
}

qint16 GameEngine::getWinner()
{
    return iWinner;
}

bool GameEngine::getDebug()
{
    return iDebug;
}

void GameEngine::toggleDebug()
{
    iDebug = (iDebug ? false : true);
}

qint16 GameEngine::getActivePlayer() const
{
    return iActivePlayer;
}

qint16 GameEngine::getGameState()
{
    switch(iGamestate) {
    case STARTED:
        return 1;
    case PLACE1:
    case PLACE2:
        return 2;
    case PLAYING:
        return 3;
    case ENDED:
        return 4;
    }
    return 0;
}

QString GameEngine::getGameStateString(qint16 _state)
{
    switch(_state) {
    case 0:
        return "invalid";
    case 1:
        return "started";
    case 2:
        return "placing";
    case 3:
        return "shooting";
    case 4:
        return "ended";
    }
    return "";
}

void GameEngine::setIActivePlayer(const qint16 &value)
{
    iActivePlayer = value;
}


qint16 GameEngine::gameAreaX() {
    return iGameareaX;
}

qint16 GameEngine::gameAreaY() {
    return iGameareaY;
}

Ship* GameEngine::getShipAt(qint16 _x, qint16 _y) {
    Ship* curr = NULL;

    for(int i = 0; i < SHIPCOUNT; i++) {
        if(iActivePlayer == 1) curr = iPlayer2ships[i];
        else curr = iPlayer1ships[i];
        for(int j = 0; j < curr->getLength(); j++) {
            switch(curr->getDirection()) {
            case NORTH: //up
                if(_x == curr->getX() && _y == curr->getY()+j) return curr;
                break;
            //case EAST: //left
            case WEST:
                if(_x == curr->getX()+j && _y == curr->getY()) return curr;
                break;
            case SOUTH: //down
                if(_x == curr->getX() && _y == curr->getY()-j) return curr;
                break;
            //case WEST: //right
            case EAST:
                if(_x == curr->getX()-j && _y == curr->getY()) return curr;
                break;
            }


        }
    }
    return NULL;
}

Ship* GameEngine::getMyShipAt(qint16 _x, qint16 _y, Ship* _ignore) {
    Ship* curr = NULL;

    for(int i = 0; i < SHIPCOUNT; i++) {
        if(iActivePlayer == 1) curr = iPlayer1ships[i];
        else curr = iPlayer2ships[i];
        if(curr->getName() != _ignore->getName()) {
            for(int j = 0; j < curr->getLength(); j++) {
                switch(curr->getDirection()) {
                case NORTH: //up
                    if(_x == curr->getX() && _y == curr->getY()+j) return curr;
                    break;
                //case EAST: //left
                case WEST:
                    if(_x == curr->getX()+j && _y == curr->getY()) return curr;
                    break;
                case SOUTH: //down
                    if(_x == curr->getX() && _y == curr->getY()-j) return curr;
                    break;
                //case WEST: //right
                case EAST:
                    if(_x == curr->getX()-j && _y == curr->getY()) return curr;
                    break;
                }


            }
        }
    }
    return NULL;
}

bool GameEngine::gameStateProgress() {
    if(iGamestate == STARTED) {
        iGamestate = PLACE1;
        iActivePlayer = 1;
        return true;
    }
    else {
        return gameStatePlace();
    }
}

void GameEngine::gameStatePassTurn()
{
    if(iActivePlayer == 1) iActivePlayer = 2;
    else iActivePlayer = 1;
    iActiveWeapon = 0;
    iActiveShip = "";
}

qint16 GameEngine::getMyAreaAt(qint16 _x, qint16 _y)
{
    if(iActivePlayer == 1) return iPlayer2Area[_x][_y];
    else return iPlayer1Area[_x][_y];
}

bool GameEngine::getShipAtArea(qint16 _x, qint16 _y)
{
    if(iActivePlayer == 1) {
        if(iPlayer1Area[_x][_y] == __SHIP_HIT) {
            if(getShipAt(_x,_y)) return true;
        }
    }
    else {
        if(iPlayer2Area[_x][_y] == __SHIP_HIT) {
            if(getShipAt(_x,_y)) return true;
        }
    }
    return false;
}

qint16 GameEngine::getAreaAt(qint16 _x, qint16 _y)
{
    if(iActivePlayer == 1) return iPlayer1Area[_x][_y];
    else return iPlayer2Area[_x][_y];
}

bool GameEngine::gameStatePlace()
{
    if(iGamestate == PLACE1) {
        iGamestate = PLACE2;
        iActivePlayer = 2;
        iActiveWeapon = 0;
        iActiveShip = "";
    }
    else if(iGamestate == PLACE2) {
        iGamestate = PLAYING;
        iActivePlayer = 1;
        iActiveWeapon = 0;
        iActiveShip = "";
    }

    if(iGamestate == PLAYING) return false;
    else return true;
}

bool GameEngine::gameStateShoot()
{
    if(iActivePlayer == 1) {
       iPlayer1Shot++;
       if(iPlayer1Shot == GAMETURNS) {
           iPlayer1Shot = 0;
           iActivePlayer = 2;
           iActiveWeapon = 0;
           iActiveShip = "";
           return false;
       }
    }
    else {
        iPlayer2Shot++;
        if(iPlayer2Shot == GAMETURNS) {
            iPlayer2Shot = 0;
            iActivePlayer = 1;
            iActiveWeapon = 0;
            iActiveShip = "";
            return false;
        }
    }
    if(checkIfGameEnded()) iGamestate = ENDED;
    return true;
}

void GameEngine::markShipAsDestroyed(Ship *_ship)
{
    if(!_ship) return;

    qint16 x = _ship->getX();
    qint16 y = _ship->getY();

    switch(_ship->getDirection()) {
    case NORTH: //up
        for(int i = 0; i < _ship->getLength(); i++) {
            if(iActivePlayer == 1) iPlayer1Area[x][y+i] = __SHIP_DESTROYED;
            else iPlayer2Area[x][y+i] = __SHIP_DESTROYED;
        }
        break;
    //case EAST: //left
    case WEST:
        for(int i = 0; i < _ship->getLength(); i++) {
            if(iActivePlayer == 1) iPlayer1Area[x+i][y] = __SHIP_DESTROYED;
            else iPlayer2Area[x+i][y] = __SHIP_DESTROYED;
        }
        break;
    case SOUTH: //down
        for(int i = 0; i < _ship->getLength(); i++) {
            if(iActivePlayer == 1) iPlayer1Area[x][y-i] = __SHIP_DESTROYED;
            else iPlayer2Area[x][y-i] = __SHIP_DESTROYED;
        }
        break;
    //case WEST: //right
    case EAST:
        for(int i = 0; i < _ship->getLength(); i++) {
            if(iActivePlayer == 1) iPlayer1Area[x-i][y] = __SHIP_DESTROYED;
            else iPlayer2Area[x-i][y] = __SHIP_DESTROYED;
        }
    }
    if(getDebug()) {
        for(int i = 0; i < GAMEY; i++) {
            for(int j = 0; j < GAMEX; j++) {
                if(iActivePlayer == 1) {
                    printf("%hd",iPlayer1Area[j][i]);
                }
                else {printf("%hd",iPlayer2Area[j][i]);}
            }
            printf("\n");
        }
    }
}

bool GameEngine::checkIfGameEnded()
{
    qint16 p1 = 0;
    qint16 p2 = 0;
    qint16 rval = 0;
    for(int i = 0; i < SHIPCOUNT; i++) {
        if(iPlayer1ships[i]->getArmor() > 0) p1++;
        if(iPlayer2ships[i]->getArmor() > 0) p2++;
    }
    if(p1 == 0) rval = 2;
    if(p2 == 0) {
        if(rval == 1) {
            if(iActivePlayer == 1) rval = 1;
            else rval = 2;
        }
        else rval = 1;
    }
    iWinner = rval;
    return (iWinner > 0) ? true : false;
}

QString GameEngine::weaponType(shotType _type)
{
    switch(_type) {
    case COASTAL:
        return "Coastal gun";
    case CANNON:
        return "Deck cannon";
    case TORPEDO:
        return "Torpedo";
    case ARTILLERY:
        return "Artillery support";
    case MACHINEGUN:
        return "Peashooter";
    }
    return "no weapon";
}

