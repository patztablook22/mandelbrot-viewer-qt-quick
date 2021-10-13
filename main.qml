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

        MouseArea {
            anchors.fill: parent
            onWheel: {
                renderer.scale += wheel.angleDelta.y / 1000;
            }
            onClicked: {
                let newX = renderer.calcCenter.x + (mouseX / parent.width  - 0.5) * renderer.calcSize.width
                let newY = renderer.calcCenter.y - (mouseY / parent.height - 0.5) * renderer.calcSize.height // y goes from bottom to up
                renderer.calcCenter = Qt.point(newX, newY)

                if (mouse.button == Qt.LeftButton)
                    renderer.scale *= 1.3
                else
                    renderer.scale /= 1.3
            }
        }
    }

    Options {}
}
