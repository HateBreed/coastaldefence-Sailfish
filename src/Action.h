#ifndef ACTION_H
#define ACTION_H

#include <QObject>
#include "Types.h"

class Action
{
public:
    Action();
    void setAction(actions _type, QString _ship, qint16 _x, qint16 _y);
    qint16 getType();
    QString getShipType();
    qint16 getX();
    qint16 getY();
    void reset();
private:
    void init();
    actions iActionType;
    QString iShip;
    qint16 iXcoord;
    qint16 iYcoord;

};

#endif // ACTION_H
