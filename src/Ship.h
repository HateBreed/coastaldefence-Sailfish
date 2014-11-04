#ifndef SHIP_H
#define SHIP_H

#include <QObject>
#include "Types.h"
#include "Weapon.h"

class Ship : public QObject
{
    Q_OBJECT
public:
    Ship(shipType _type);
    QString getName();
    QString getVisibleName();
    shipType getType();
    Weapon* getWeapon(qint16 _weaponid);
    qint16 getWeaponAmmo(qint16 _weaponid);
    qint16 getSpeed();
    qint16 getArmor();
    qint16 getArmorOriginal();
    qint16 getX();
    qint16 getY();
    qint16 getLength();
    direction getDirection();
    QString getImage();
    QString getDestroyedImage();

    qint16 hit(hitType _hit, Weapon* _weapon, qint16 _x, qint16 _y);
    void place(qint16 _x, qint16 _y);
    void setCoordinates(qint16 _x, qint16 _y);
    bool getMoving();
    void setMoving();
    void rotate(qint16 _direction);

    qint16 getBlockId(qint16 _x, qint16 _y);
    quint8 getHits(qint16 _blockid);
    void addHit(qint16 _blockid);

signals:

public slots:

private:

    Ship(QObject *parent = 0);

    shipType iType;
    Weapon* iWeapon1;
    Weapon* iWeapon2;
    qint16 iSpeed;
    qint16 iArmorStrength;
    qint16 iArmorOriginal;
    qint16 iXcoordinate;
    qint16 iYcoordinate;
    qint16 iLength;
    QString iIdentification;
    direction iDirection;
    bool iMoving;
    QString iImage;
    QString iImageRot;
    qint16 iWeapon1Ammo;
    qint16 iWeapon2Ammo;
    quint8 iHits;
};

#endif // SHIP_H
