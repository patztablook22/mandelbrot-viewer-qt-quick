import QtQuick 2.0

Rectangle {
    x: 10
    y: 10
    width: 100
    height: 100
    color: "#99222222"
    clip: true

    Column {

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

    }
}
