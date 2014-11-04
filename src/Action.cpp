#include "Action.h"

Action::Action()
{
    init();
}

void Action::setAction(actions _type, QString _ship, qint16 _x, qint16 _y)
{
    iActionType = _type;
    iShip = QString(_ship);
    iXcoord = _x;
    iYcoord = _y;
}

qint16 Action::getType()
{
    return (qint16)iActionType;
}

QString Action::getShipType()
{
    return iShip;
}

qint16 Action::getX()
{
    return iXcoord;
}

qint16 Action::getY()
{
    return iYcoord;
}

void Action::reset()
{
    init();
}

void Action::init()
{
    iActionType = EMPTY;
    iShip = "";
    iXcoord = -1;
    iYcoord = -1;
}
