#ifndef WEAPON_H
#define WEAPON_H

#include <QObject>
#include "Types.h"

class Weapon : public QObject
{
    Q_OBJECT
public:

    Weapon(shotType _type);

    shotType getType();

    qint16 getDamage();

    QString getName();

    qint16 getSpeed();

    void setAmmo(qint16 _ammocount);
    qint16 getAmmo();

    bool shoot();

signals:

public slots:

private:
    Weapon(QObject *parent = 0);
    shotType iType;
    qint16 iDamage;
    QString iName;
    qint16 iSpeed;
    qint16 iAmmo;
    //qint16 iSpreadMatrix[5][5];
    //qint16 iCondition;

};

#endif // WEAPON_H
