import QtQuick 2.0

Item {
        anchors.fill: parent

        // horizontal grid line
        Rectangle {
                x: 0
                y: parent.height / 2
                width: parent.width
                height: 1
                color: "red"
        }

        // vertical grid line
        Rectangle {
                x: parent.width / 2
                y: 0
                width: 1
                height: parent.height
                color: "red"
        }
}
