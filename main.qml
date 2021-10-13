import QtQuick 2.15
import QtQuick.Controls 2.12
import QtMultimedia 5.12
import org.renderer 1.0

ApplicationWindow {
    width: 640
    height: 480
    visible: true

    menuBar: MenuBar {
        Menu {
            title: "&File"
            Action {
                text: "&Export"
                shortcut: "ctrl+e"
            }
            Action {
                text: "&Quit"
                shortcut: "ctrl+q"
                onTriggered: Qt.quit()
            }
        }
    }

    Renderer {
        outSize: Qt.size(output.width, output.height)
        id: renderer
    }

    VideoOutput {
        id: output
        anchors.fill: parent
        source: renderer
    }

    Options {}
}
