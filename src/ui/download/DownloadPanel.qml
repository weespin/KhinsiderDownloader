import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import "../shared"
Item {
    id:root
    //todo: show speed per album, MB/MB stop button (delete)
    width: 800
    height: 600
    Rectangle {
        id:mainWindow
        state: "normal"
        states: [
            State {
                name: "normal"
                PropertyChanges { target: leftColumn; width: mainRow.width * 0.7 }
                PropertyChanges { target: rightColumn; width: mainRow.width * 0.3; x: leftColumn.width }
            },
            State {
                name: "expanded"
                PropertyChanges { target: leftColumn; width: mainRow.width }
                PropertyChanges { target: rightColumn; width: mainRow.width * 0.3; x: mainWindow.width }
            }
        ]
        width: parent.width
        height: parent.height

        color: "#2C3E50"
        Row
        {
            id:mainRow
            width: parent.width
            height: parent.height

            Column {
                id:leftColumn
                width: parent.width
                height: parent.height

                spacing: 10
                // Bulk
                Column {
                    height: parent.height * 0.8
                    width: parent.width
                    Item {
                        width: parent.width
                        height: parent.height
                        Rectangle {
                            id: bulkRectangle
                            anchors {
                                top: parent.top
                                left: parent.left
                                right: parent.right
                                topMargin: 10
                                leftMargin: 10
                                rightMargin: 10
                                bottomMargin: 10
                            }
                            height: parent.height - (anchors.topMargin + anchors.bottomMargin)
                            color: "#34495E"
                            radius: 10
                            WScrollView
                            {
                                anchors.fill: parent
                                TextArea
                                {
                                    hoverEnabled: true
                                    id: textfield
                                    height: parent.height
                                    color: "#ffffff"
                                    font.pointSize: 13
                                    placeholderTextColor: "#b5ffffff"
                                    placeholderText: "URLs here"
                                    width: parent.width
                                    background:null

                                    PropertyAnimation {
                                        id: onPlaceholderHover
                                        target: textfield
                                        property: "placeholderTextColor"
                                        to: "#00ffffff" // ~70% opacity
                                        duration: 100
                                    }
                                    PropertyAnimation {
                                        id: onPlaceholderUnHover
                                        target: textfield
                                        property: "placeholderTextColor"
                                        to: "#b5ffffff" // original alpha
                                        duration: 100
                                    }
                                    onHoveredChanged: {
                                        if (hovered) {
                                            onPlaceholderHover.start()
                                        } else {
                                            onPlaceholderUnHover.start()
                                        }
                                    }

                                }
                            }
                        }
                    }

                }
                // Status
                ColumnLayout {
                    height: parent.height * 0.2 - leftColumn.spacing
                    spacing: 10
                    width: parent.width
                    WButton {

                        id: wButton
                        Layout.alignment: Qt.AlignCenter
                        Layout.preferredHeight: 45

                        Layout.preferredWidth: parent.width * 0.8
                        label: "Import"
                        onClicked:
                        {
                            app.downloaderController.downloaderVM.addToDownloadList(textfield.text);
                            textfield.text = "";
                            //mainWindow.state = (mainWindow.state === "normal") ? "expanded" : "normal"
                        }
                    }
                    WButton {
                        id: stopButton
                        Layout.preferredHeight: 45
                        Layout.preferredWidth: parent.width * 0.8
                        Layout.alignment: Qt.AlignCenter
                        onClicked:
                        {
                            app.downloaderController.downloaderVM.cancelAllDownloads();
                        }
                        label: "Cancel All"
                    }
                    Item
                    {
                        //Spacer
                        Layout.fillWidth: true
                        Layout.preferredHeight: 10
                    }
                }
            }
            Column{
                id :rightColumn
                height: parent.height
                width: parent.width
                DownloadSide
                {
                    width: parent.width
                    height: parent.height

                }

            }
        }
    }
}
