import QtQuick 2.0
import QtQuick.Controls 2.12
import Qt.labs.folderlistmodel 2.12
import org.palette 0.0

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
        background: Rectangle {
                color: "#99222222"
                radius: 10
        }

        FolderListModel {
                id: palettes
                folder: "qrc:/palettes/"
        }

        ScrollView {
                anchors.fill: parent
                ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                ListView {
                        boundsBehavior: Flickable.StopAtBounds
                        anchors.fill: parent
                        model: palettes
                        clip: true
                        delegate: Rectangle {
                                height: 20
                                width: parent.width
                                color: "transparent"
                                Text {
                                        color: "white"
                                        text: fileName
                                }

                                Palette {
                                    id: pal
                                    source: filePath
                                }

                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        renderer.palette = pal
                                    }
                                }
                        }
                }
        }

}
