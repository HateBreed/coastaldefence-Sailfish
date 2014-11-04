import QtQuick 2.0

Rectangle {
    width: 50
    height: 50
    color: "blue"

    MouseArea {
        anchors.fill: parent
        onClicked: parent.color = "red"
    }
}
