import QtQuick
import QtQuick.Layouts
import QtQuick.Effects
import QtQuick.Controls.Basic
import "../shared"

Rectangle {
    color: "#34495E"
    height: 500
    width: 200

    ColumnLayout {
        anchors.fill:parent
        spacing: 5

        // Top - Album image caret
        AlbumImageCaret {
            Layout.preferredHeight: parent.height * 0.3
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            //width: 100
            height: parent.height * 0.4
        }

        // Center - Scrollable album info
        WScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            id: scrollView
            wheelEnabled: false

            Column {
                width: scrollView.width * 0.9
                height: scrollView.height
                leftPadding: 10
                spacing: 5

                Repeater {
                    model: appController.albumInfoVM.filteredAlbumFields
                    Text {
                        color: "#ffffff"
                        text: modelData.label + ": " + modelData.value
                        elide: Text.ElideRight
                        font.pointSize: 13
                        font.bold: true
                        width: parent.width
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                }

            }
        }

        // Bottom - Download buttons
        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: column2.implicitHeight

            Column {
                id: column2
                spacing: 5
                width: parent.width

                Repeater
                {
                    model: appController.albumInfoVM.formats
                    WButton {
                        width: parent.width * 0.6
                        anchors.horizontalCenter: parent.horizontalCenter
                        label: "Add " + modelData
                        opacity:0
                        Behavior on opacity {
                            NumberAnimation {
                                duration: 100
                                easing.type: Easing.InOutQuad
                            }
                        }
                        Component.onCompleted: {
                            opacity = 1
                        }
                        onClicked:
                        {
                            appController.albumInfoVM.requestDownload(modelData);
                        }
                    }
                }



                Item {
                    height: 20
                    width: parent.width
                }
            }
        }
    }
}
