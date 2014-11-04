#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <QObject>
#include "Ship.h"
#include "Types.h"
#include "Weapon.h"
#include "Action.h"

#define GAMEX 9
#define GAMEY 14
#define GAMETURNS 3
#define SHIPCOUNT 4

class GameEngine : public QObject
{
    Q_OBJECT
public:
    //explicit GameEngine(QObject *parent = 0);
    Q_INVOKABLE static GameEngine* initEngine();

    Q_INVOKABLE qint16 placeShip(QString _id, qint16 _x, qint16 _y);
    Q_INVOKABLE qint16 placeShot(qint16 _x, qint16 _y, QString _fromShip, qint16 _weaponid);
    Q_INVOKABLE QString getDestroyedShipName(qint16 _x, qint16 _y);
    Q_INVOKABLE QString getDestroyedShipImage(qint16 _x, qint16 _y);

    Q_INVOKABLE bool checkPosition(Ship* _ship, qint16 _x, qint16 _y);
    Q_INVOKABLE qint16 gameAreaX();
    Q_INVOKABLE qint16 gameAreaY();

    Q_INVOKABLE qint16 getActivePlayer() const;
    Q_INVOKABLE qint16 getGameState();
    Q_INVOKABLE QString getGameStateString(qint16 _state);
    Q_INVOKABLE void setIActivePlayer(const qint16 &value);

    Ship* getShipAt(qint16 _x, qint16 _y);
    Ship* getMyShipAt(qint16 _x, qint16 _y, Ship* _ignore);

    Ship* getMyShip(QString _id);

    Q_INVOKABLE qint16 getShipX(QString _id);
    Q_INVOKABLE qint16 getShipY(QString _id);

    Q_INVOKABLE qint16 getShipArmor(QString _id);
    Q_INVOKABLE qint16 getShipArmorOriginal(QString _id);

    Q_INVOKABLE QString getShipWeapon(QString _id, qint16 _type);
    Q_INVOKABLE qint16 getShipWeaponAmmo(QString _id, qint16 _type);
    Q_INVOKABLE QString getShipActiveWeapon(QString _id);
    Q_INVOKABLE qint16 getShipWeaponDamage(QString _id, qint16 _weapon);

    Q_INVOKABLE qint16 getShipSpeed(QString _id);
    Q_INVOKABLE QString getShipDirection(QString _id);
    Q_INVOKABLE qint16 getShipOrientation(QString _id);

    Q_INVOKABLE QString getShipType(QString _id);
    Q_INVOKABLE bool getShipMoveState(QString _id);
    Q_INVOKABLE qint16 getShipLength(QString _id);
    Q_INVOKABLE QString getShipImage(QString _id);
    Q_INVOKABLE bool rotateShip(QString _id, qint16 _type);
    Q_INVOKABLE bool canShipRotate(QString _id, qint16 _type);

    Q_INVOKABLE bool gameStateProgress();
    Q_INVOKABLE void gameStatePassTurn();

    Q_INVOKABLE qint16 getAreaAt(qint16 _x, qint16 _y);
    Q_INVOKABLE qint16 getMyAreaAt(qint16 _x, qint16 _y);
    Q_INVOKABLE bool getShipAtArea(qint16 _x, qint16 _y);

    Q_INVOKABLE qint16 getPlayerShot() const;
    Q_INVOKABLE void setActiveShip(QString _name);
    Q_INVOKABLE QString getActiveShip();
    Q_INVOKABLE void setActiveWeapon(qint16 _weapon);
    Q_INVOKABLE qint16 getActiveWeapon();
    Q_INVOKABLE void unsetActive();
    Q_INVOKABLE qint16 getWinner();

    Q_INVOKABLE bool getDebug();
    Q_INVOKABLE void toggleDebug();

signals:

public slots:

private:
    GameEngine(QObject *parent = 0);
    void initGameArea();

    static GameEngine* iEngine;

    qint16 iGameareaX;
    qint16 iGameareaY;

    qint16 iPlayer1Area[GAMEX][GAMEY];
    qint16 iPlayer2Area[GAMEX][GAMEY];

    qint16 iActivePlayer;
    gameState iGamestate;
    qint16 iPlayer1Placed;
    qint16 iPlayer2Placed;
    qint16 iPlayer1Shot;
    qint16 iPlayer2Shot;

    Ship *iPlayer1ships[SHIPCOUNT];
    Ship *iPlayer2ships[SHIPCOUNT];

    Action *iPlayer1Turns[GAMETURNS];
    Action *iPlayer2Turns[GAMETURNS];

    bool gameStatePlace();
    bool gameStateShoot();
    void markShipAsDestroyed(Ship* _ship);
    bool checkIfGameEnded();
    QString weaponType(shotType _type);

    QString iActiveShip;
    qint16 iActiveWeapon;
    qint16 iWinner;

    bool iDebug;
};

#endif // GAMEENGINE_H
