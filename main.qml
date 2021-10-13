import QtQuick 2.15
import QtQuick.Controls 2.12
import QtMultimedia 5.12
import org.palette 1.0
import org.renderer 1.0

ApplicationWindow {
    width: 640
    height: 480
    visible: true

    RenderingMenu { id: renderingMenu }
    PaletteMenu   { id: paletteMenu }

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

        Menu {
            title: "&Options"
            Action {
                text: "&Palette"
                shortcut: "ctrl+p"
                onTriggered: paletteMenu.open();
            }
            Action {
                text: "&Rendering"
                shortcut: "ctrl+r"
                onTriggered: renderingMenu.open();
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
        onWidthChanged:  renderer.outSize = Qt.size(width, height)
        onHeightChanged: renderer.outSize = Qt.size(width, height)

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton | Qt.MidButton
            onWheel: {
                renderer.scale += wheel.angleDelta.y / 1000;
            }
            onClicked: {
                let newX = renderer.calcCenter.x + (mouseX / parent.width  - 0.5) * renderer.calcSize.width
                let newY = renderer.calcCenter.y - (mouseY / parent.height - 0.5) * renderer.calcSize.height // y goes from bottom to up
                renderer.calcCenter = Qt.point(newX, newY)

                switch (mouse.button) {
                case Qt.LeftButton:
                    renderer.scale *= 1.3
                    break;
                case Qt.RightButton:
                    renderer.scale /= 1.3
                    break;
                case Qt.MidButton:
                    break;
                }
            }
        }
    }

    Info {}
}
