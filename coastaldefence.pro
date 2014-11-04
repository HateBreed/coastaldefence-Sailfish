# The name of your app.
# NOTICE: name defined in TARGET has a corresponding QML filename.
#         If name defined in TARGET is changed, following needs to be
#         done to match new name:
#         - corresponding QML filename must be changed
#         - desktop icon filename must be changed
#         - desktop filename must be changed
#         - icon definition filename in desktop file must be changed
TARGET = coastaldefence

CONFIG += sailfishapp

SOURCES += \
    src/CoastalDefence.cpp \
    src/GameEngine.cpp \
    src/Ship.cpp \
    src/Weapon.cpp \
    src/Action.cpp

OTHER_FILES += qml/coastaldefence.qml \
    qml/cover/CoverPage.qml \
    qml/pages/FirstPage.qml \
    qml/pages/SecondPage.qml \
    rpm/coastaldefence.spec \
    rpm/coastaldefence.yaml \
    coastaldefence.desktop \
    qml/pages/StartPage.qml \
    qml/pages/PlayerTurn.qml \
    qml/pages/Winner.qml \
    qml/pages/PlacingBlock.qml \
    qml/pages/ShootingBlock.qml \
    qml/pages/Ship.qml \
    qml/pages/ShipInfo.qml \
    qml/pages/InfoItem.qml

HEADERS += \
    src/GameEngine.h \
    src/Ship.h \
    src/Types.h \
    src/Weapon.h \
    src/Action.h

