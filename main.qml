import QtQuick 2.15
import QtQuick.Controls 2.12
import QtMultimedia 5.12
import org.palette 1.0
import org.renderer 1.0
import QtQuick.Dialogs 1.2

ApplicationWindow {
    width: 640
    height: 480
    visible: true

    FileDialog {
        id: exportDialog
        title: "Choose export path"
        defaultSuffix: "png"
        folder: shortcuts.home
        nameFilters: ["Image files (*.jpg *.png)", "All Files (*)"]
        selectExisting: false
        onAccepted: renderer.exportTo(fileUrl.toString().split("file://")[1])
    }

    RenderingMenu { id: renderingMenu }
    PaletteMenu   { id: paletteMenu }

    menuBar: MenuBar {
        Menu {
            title: "&File"
            Action {
                text: "&Export"
                shortcut: "ctrl+e"
                onTriggered: exportDialog.open();
            }
            Action {
                text: "&Quit"
                shortcut: "ctrl+q"
                onTriggered: Qt.quit()
            }
        }

        Menu {
            title: "&View"
            Action {
                id: viewInfo
                text: "Info"
                checkable: true
                checked: true
            }

            Action {
                id: viewGrid
                text: "Grid"
                checkable: true
                checked: true
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

    Item {
        id: grid
        visible: viewGrid.checked
        anchors.fill: parent

        // horizontal
        Rectangle {
            x: 0
            y: parent.height / 2
            width: parent.width
            height: 1
            color: "red"
        }

        // vertical
        Rectangle {
            x: parent.width / 2
            y: 0
            width: 1
            height: parent.height
            color: "red"
        }
    }

    Info {
        id: info
        visible: viewInfo.checked
    }
}
