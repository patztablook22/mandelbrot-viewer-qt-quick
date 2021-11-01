import QtQuick 2.0
import QtQuick.Controls 2.12

Rectangle {
    width: 128
    height: 42
    anchors.right: parent.right
    anchors.bottom: parent.bottom
    color: "#99222222"
    clip: true

    Column {
        anchors.fill: parent
        anchors.margins: 5

        Text {
            color: "white"
            text: {
                if (renderer.precision === 0)
                   "loading..."
                else
                   "precision: " + renderer.precision
            }
        }

        Text {
            color: "white"
            text: "scale: " + Math.round(renderer.scale * 100) / 100
        }
    }
}
