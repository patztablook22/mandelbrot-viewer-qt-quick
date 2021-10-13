import QtQuick 2.0
import QtQuick.Controls 2.12

Rectangle {
    x: 10
    y: 10
    width: 100
    height: 100
    color: "#99222222"
    clip: true

    Column {
        anchors.fill: parent

        Text {
            color: "white"
            text: "scale: " + Math.round(renderer.scale * 100) / 100
        }

        Text {
            color: "white"
            text: "precision: " + renderer.precision
        }

        Row {
            Text {
                color: "white"
                text: "threads: "
            }
            TextInput {
                color: "white"
                text: renderer.threads
                onTextChanged: renderer.threads = text
            }

        }

        Row {
            Text {
                color: "white"
                text: "exponent: "
            }
            TextInput {
                color: "white"
                text: renderer.exponent
                onTextChanged: renderer.exponent = text
            }
        }
        Slider {
            width: parent.width
            orientation: Qt.Horizontal
            from: 1
            to: 13
            value: renderer.exponent
            onValueChanged: renderer.exponent = value
        }

    }
}
