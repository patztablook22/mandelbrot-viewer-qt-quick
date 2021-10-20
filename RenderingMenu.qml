import QtQuick 2.0
import QtQuick.Controls 2.12

Popup {
    width: 300
    height: 300
    anchors.centerIn: parent
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
    focus: true
    clip: true
    modal: true
    padding: 20
    enter: Transition {
            NumberAnimation {
                    property: "opacity"
                    from: 0
                    to: 1
            }
    }
    exit: Transition {
            NumberAnimation {
                    property: "opacity"
                    from: 1
                    to: 0
            }
    }

    Column {
            anchors.fill: parent
            Text {
                    color: "white"
                    font.bold: true
                    text: "Rendering Menu"
            }
            Rectangle {
                    width: parent.width
                    height: 1
                    color: "white"
            }

            Row {
                    Text {
                            color: "white"
                            text: "threads: "
                    }
                    TextInput {
                            color: "gray"
                            text: renderer.threads
                            onTextChanged: renderer.threads = text
                    }
            }

            Row {
                    Text {
                            color: "white"
                            text: "resolution: "
                    }
                    TextInput {
                            color: "gray"
                            text: renderer.outSize.width
                            onTextChanged: {
                                    if (Number(text) < 1)
                                            return;
                                    renderer.outSize.width = text
                            }
                    }
                    Text {
                            color: "white"
                            text: "x"
                    }
                    TextInput {
                            color: "gray"
                            text: renderer.outSize.height
                            onTextChanged: {
                                    if (Number(text) < 1)
                                            return;
                                    renderer.outSize.height = text
                            }
                    }
            }
            Text {
                    color: "orange"
                    text: "(resolution will reset on window resize)\n"
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
