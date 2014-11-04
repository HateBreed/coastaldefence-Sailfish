#include "Ship.h"
#include <qmath.h>
#include <stdio.h>

Ship::Ship(QObject *parent) :
    QObject(parent)
{
    iXcoordinate = 0;
    iYcoordinate = 0;
    iDirection = EAST;
    iMoving = false;
    iHits = 0;
}

Ship::Ship(shipType _type) {
    switch(_type) {
    case SUBMARINE:
        iType = _type;
        iWeapon1 = new Weapon(TORPEDO);
        iWeapon1->setAmmo(10);
        iWeapon2 = new Weapon(COASTAL);
        iWeapon2->setAmmo(5);
        iSpeed = 3;
        iArmorStrength = 150;
        iArmorOriginal = 150;
        iLength = 2;
        iIdentification = "sub";
        break;
    case BATTLESHIP:
        iType = _type;
        iWeapon1 = new Weapon(MACHINEGUN);
        iWeapon1->setAmmo(-1);
        iWeapon2 = new Weapon(CANNON);
        iWeapon2->setAmmo(15);
        iSpeed = 2;
        iArmorStrength = 170;
        iArmorOriginal = 170;
        iLength = 3;
        iIdentification = "ship";
        break;
    case CRUISER:
        iType = _type;
        iWeapon1 = new Weapon(TORPEDO);
        iWeapon1->setAmmo(5);
        iWeapon2 = new Weapon(CANNON);
        iWeapon2->setAmmo(20);
        iSpeed = 4;
        iArmorStrength = 200;
        iArmorOriginal = 200;
        iLength = 3;
        iIdentification = "cruiser";
        break;
    case CARRIER:
        iType = _type;
        iWeapon1 = new Weapon(ARTILLERY);
        iWeapon1->setAmmo(5);
        iWeapon2 = new Weapon(CANNON);
        iWeapon2->setAmmo(-1);
        iSpeed = 1;
        iArmorStrength = 250;
        iArmorOriginal = 250;
        iLength = 4;
        iIdentification = "carrier";
        break;
    }
    iXcoordinate = iIdentification == "cruiser" ? iLength*2-1 : iLength-1 ;
    iYcoordinate = iLength-1;
    iDirection = EAST;
    iMoving = false;
    iImage = "battleship-ger.png";
    iImageRot = "battleship-ger_h.png";
    iHits = 0;
}

QString Ship::getName() {
    return iIdentification;
}

QString Ship::getVisibleName()
{
    switch(iType) {
    case SUBMARINE:
        return "U-boat";
    case BATTLESHIP:
        return "Battleship";
    case CRUISER:
        return "Cruiser";
    case CARRIER:
        return "Aircraft Carrier";
    }
    return "";
}

shipType Ship::getType() {
    return iType;
}

Weapon* Ship::getWeapon(qint16 _weaponid) {
    switch(_weaponid) {
    case 1:
        return iWeapon1;
    case 2:
        return iWeapon2;
    default:
        return 0;
    }
}

qint16 Ship::getWeaponAmmo(qint16 _weaponid) {
    switch(_weaponid) {
    case 1:
        return iWeapon1->getAmmo();
    case 2:
        return iWeapon2->getAmmo();
    default:
        return 0;
    }
}

qint16 Ship::getSpeed()  {
    return iSpeed;
}

qint16 Ship::getArmor()  {
    return iArmorStrength;
}

qint16 Ship::getArmorOriginal()
{
    return iArmorOriginal;
}

qint16 Ship::getX()  {
    return iXcoordinate;
}

qint16 Ship::getY()  {
    return iYcoordinate;
}

qint16 Ship::getLength()  {
    return iLength;
}

qint16 Ship::hit(hitType _hit, Weapon* _weapon,qint16 _x, qint16 _y) {
    double multiplicator = 1.0;
    qint16 damage = 0;
    qint16 blockid = getBlockId(_x,_y);

    switch(_hit) {
    case NEAR:
        if(_weapon->getType() != MACHINEGUN) multiplicator = 0.5;
        break;
    case HITAGAIN:
        multiplicator = 0.75 - getHits(blockid) * 0.1;
        break;
    case MISS:
    case HIT:
    case DESTROYED:
        break;
    }

    damage = qCeil(_weapon->getDamage() * multiplicator);
    iArmorStrength -= damage;
    addHit(blockid);

    if (iArmorStrength <= 0) {
        iArmorStrength = 0;
        return -1;
    }
    return damage;
}

void Ship::place(qint16 _x, qint16 _y) {
    iXcoordinate = _x;
    iYcoordinate = _y;

}

void Ship::rotate(qint16 _direction) {
    switch(_direction) {
    case 0: // CCW
        if(iDirection == NORTH) iDirection = WEST;
        else if(iDirection == EAST) iDirection = NORTH;
        else if(iDirection == SOUTH) iDirection = EAST;
        else iDirection = SOUTH;
        break;
    case 1: // CW
        if(iDirection == NORTH) iDirection = EAST;
        else if(iDirection == EAST) iDirection = SOUTH;
        else if(iDirection == SOUTH) iDirection = WEST;
        else iDirection = NORTH;
        break;
    }

}

qint16 Ship::getBlockId(qint16 _x, qint16 _y)
{
    for(int i = 0; i < iLength; i++) {
        switch(iDirection) {
        case NORTH:
            if(iXcoordinate == _x && iYcoordinate+i == _y) return i;
            break;
        case EAST:
            if(iXcoordinate-i == _x && iYcoordinate == _y) return i;
            break;
        case WEST:
            if(iXcoordinate+i == _x && iYcoordinate == _y) return i;
            break;
        case SOUTH:
            if(iXcoordinate == _x && iYcoordinate-1 == _y) return i;
            break;
        }
    }
    return -1;
}

quint8 Ship::getHits(qint16 _blockid)
{
    quint8 bitmask = 3;
    quint8 result = 0;

    switch(_blockid) {
    case 0:
        result = iHits >> 6;
        break;
    case 1:
        result = (iHits >> 4) & bitmask;
        break;
    case 2:
        result = (iHits >> 2) & bitmask;
        break;
    case 3:
        result = iHits & bitmask;
        break;
    }
    return result;
}

void Ship::addHit(qint16 _blockid)
{
    quint8 clean = 0;
    quint8 hitcount = getHits(_blockid);
    if(hitcount < 3) hitcount++;
    switch (_blockid) {
    case 0:
        iHits = hitcount << 6;
        break;
    case 1:
        iHits &= clean << 4;
        iHits |= hitcount << 4;
        break;
    case 2:
        iHits &= clean << 2;
        iHits |= hitcount << 2;
        break;
    case 3:
        iHits |= hitcount;
        break;
    }
}

void Ship::setCoordinates(qint16 _x, qint16 _y)
{
    iXcoordinate = _x;
    iYcoordinate = _y;
}

bool Ship::getMoving() {
    return iMoving;
}
void Ship::setMoving() {
    if(iMoving) {
        switch(iDirection) {
        case NORTH: // up
            iYcoordinate -= iSpeed;
            break;
        case EAST: // right
            iXcoordinate += iSpeed;
            break;
        case SOUTH: // down
            iYcoordinate += iSpeed;
            break;
        case WEST: // left
            iXcoordinate -= iSpeed;
            break;
        }
    }
}

direction Ship::getDirection() {
    return iDirection;
}

QString Ship::getImage()
{
    switch (iDirection) {
    case NORTH:
    case SOUTH:
        return iImageRot;
    case EAST:
    case WEST:
    default:
        return iImage;
    }
}

QString Ship::getDestroyedImage()
{
    return getImage();
}
