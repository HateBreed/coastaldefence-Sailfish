#include "Weapon.h"

Weapon::Weapon(QObject *parent) :
    QObject(parent)
{
}

Weapon::Weapon(shotType _type)
{
    switch(_type) {
    case CANNON:
        iDamage = 25;
        iSpeed = 40;
        iName = "Deck cannon";
        iType = _type;
        break;
    case COASTAL:
        iDamage = 50;
        iSpeed = 30;
        iName = "Coastal gun";
        iType = _type;
        break;
    case ARTILLERY:
        iDamage = 100;
        iSpeed = 10;
        iName = "Bomber fleet";
        iType = _type;
        break;
    case MACHINEGUN:
        iDamage = 10;
        iSpeed = 100;
        iName = "Machine gun";
        iType = _type;
        break;
    case TORPEDO:
        iDamage = 50;
        iSpeed = 20;
        iName = "Torpedo";
        iType = _type;
        break;
    }
    iAmmo = 0;
}

shotType Weapon::getType()
{
    return iType;
}

qint16 Weapon::getDamage()
{
    return iDamage;
}

QString Weapon::getName()
{
    return iName;
}

qint16 Weapon::getSpeed()
{
    return iSpeed;
}

void Weapon::setAmmo(qint16 _ammocount)
{
    iAmmo = _ammocount;
}

qint16 Weapon::getAmmo()
{
    return iAmmo;
}

bool Weapon::shoot()
{
    if(iAmmo == 0) return false;
    else {
        if(iAmmo != -1) iAmmo--;
        return true;
    }
}
