#ifndef TYPES_H
#define TYPES_H

enum shotType {
    COASTAL=0,
    CANNON=1,
    TORPEDO=2,
    ARTILLERY=3,
    MACHINEGUN=4
};

enum shipType {
    SUBMARINE=0,
    BATTLESHIP=1,
    CRUISER=2,
    CARRIER=3
};

enum hitType {
    MISS=0,
    HIT=1,
    NEAR=2,
    DESTROYED=3,
    HITAGAIN=4
};

enum gameState {
    STARTED=0,
    PLACE1,
    PLACE2,
    PLAYING,
    ENDED
};

enum direction {
    NORTH=0,
    EAST=1,
    SOUTH=2,
    WEST=3
};

enum actions {
    SHOOT=0,
    MOVING=1,
    ROTATE=2,
    EMPTY=3
};

#endif // TYPES_H
