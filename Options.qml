import QtQuick 2.0

Rectangle {
    x: 10
    y: 10
    width: 100
    height: 100
    color: "#99222222"
    clip: true

    Column {

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
