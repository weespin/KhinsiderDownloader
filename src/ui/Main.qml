import QtQuick 2.15
import QtQuick.Window 2.15
import "shared"
import "search"
import "settings"
import "download"
import "about"
import QtQuick.Dialogs
import QtQuick.Layouts
Window {
    id: window
    width: 1000
    height: 600
    onHeightChanged: {
        if(maincol.state === "abouttab")
        {
            //TODO: Meh solution
            slider.jump(leftaboutbutton);
        }
    }
    minimumHeight: 600
    minimumWidth: 1000
    visible: true
    color: "#2c3e50"
    title: qsTr("Khinsider Downloader - QT")

    Column {
        id: maincol
        state: "downloadtab"
        states: [
            State {
                name: "downloadtab" //centerPanel

                PropertyChanges { target: downloadPanel; visible:true }
                PropertyChanges { target: settingsPanel; visible:false }
                PropertyChanges { target: searchPanel; visible: false}
                PropertyChanges { target: aboutPanel; visible:false }
            },
            State {
                name: "searchtab"

                PropertyChanges { target: searchPanel; visible:true }
                PropertyChanges { target: settingsPanel; visible:false }
                PropertyChanges { target: downloadPanel; visible: false}
                PropertyChanges { target: aboutPanel; visible:false }
            },
            State {
                name: "settingstab"

                PropertyChanges { target: settingsPanel; visible:true }
                PropertyChanges { target: downloadPanel; visible:false }
                PropertyChanges { target: searchPanel; visible: false}
                PropertyChanges { target: aboutPanel; visible:false }
            },
            State {
                name: "abouttab"

                PropertyChanges { target: aboutPanel; visible:true }
                PropertyChanges { target: downloadPanel; visible:false }
                PropertyChanges { target: searchPanel; visible: false}
                PropertyChanges { target: settingsPanel; visible: false}
            }
        ]
        anchors.fill: parent
        spacing: 0
        // Header
        //Rectangle {
        //    width: parent.width
        //    height: parent.height * 0.05
        //    color: "#34495e"
        //    Text {
        //        anchors.fill: parent
        //        anchors.leftMargin: 10
        //        text: "KhinsiderDownloader 🎵"
        //        color: "white"
        //        font.pixelSize: 20
        //        horizontalAlignment: Text.AlignLeft
        //        verticalAlignment: Text.AlignVCenter
        //    }
        //}
        Row {
            width: parent.width
            height: parent.height * 1
            spacing: 0
            Rectangle {
                id: leftPanel
                width: 100
                Layout.minimumWidth: 100
                Layout.preferredWidth: 100
                Layout.fillHeight: true
                height: parent.height
                color: "#34495e"
                Rectangle{
                    Behavior on y {
                        NumberAnimation {
                            duration: 150
                            easing.type: Easing.InOutQuad
                        }
                    }
                    x: leftdownloadbutton.x
                    y: leftdownloadbutton.y - (leftdownloadbutton.height * 0.2)
                    width: leftdownloadbutton.width
                    height: leftdownloadbutton.height + (leftdownloadbutton.height * 0.4)
                    id:slider
                    color: "#2c3e50"
                    function jump(id)
                    {
                        var offset = id.height * 0.2;
                        slider.y = id.y - offset;
                        slider.x = id.x;
                        slider.width = id.width;
                        slider.height = id.height + (offset * 2);
                    }

                }


                ColumnLayout {
                    id: column
                    width: parent.width
                    height: parent.height
                    spacing: 20
                    Item {
                        Layout.preferredHeight: 10
                    }
                    SideButton {
                        id: leftdownloadbutton
                        Layout.preferredHeight: 40
                        Layout.alignment: Qt.AlignTop
                        iconSource: "../../icons/dl.svg"
                        iconFallback: "qrc:/icons/dl.svg"
                        label: "Download"
                        onClicked: {
                            maincol.state = "downloadtab"
                            slider.jump(leftdownloadbutton);
                        }
                    }

                    SideButton {
                        id: leftsearchbutton
                        Layout.preferredHeight: 40
                        Layout.alignment: Qt.AlignTop
                        iconSource: "../../icons/search.svg"
                        iconFallback: "qrc:/icons/search.svg"
                        label: "Search"
                        onClicked: {
                            maincol.state = "searchtab"
                            slider.jump(leftsearchbutton);
                        }
                    }

                    SideButton {
                        id: leftsettingsbutton
                        Layout.preferredHeight: 40
                        Layout.alignment: Qt.AlignTop
                        iconSource: "../../icons/settings.svg"
                        iconFallback: "qrc:/icons/settings.svg"
                        label: "Settings"
                        onClicked: {
                            maincol.state = "settingstab"
                            slider.jump(leftsettingsbutton);
                        }
                    }

                    Item {
                        Layout.fillHeight: true
                    }

                    SideButton {
                        id: leftaboutbutton
                        Layout.preferredHeight: 50
                        Layout.alignment: Qt.AlignBottom
                        iconSource: "../../icons/about.svg"
                        iconFallback: "qrc:/icons/about.svg"
                        label: "About"
                        onClicked: {
                            maincol.state = "abouttab"
                            slider.jump(leftaboutbutton);
                        }
                    }
                }
            }
            // Center panel
            Rectangle {
                id: centerPanel
                width: parent.width - leftPanel.width
                Layout.fillWidth: true
                Layout.fillHeight: true
                height: parent.height
                color: "transparent"
                SearchPanel
                {
                    id: searchPanel;
                    width: parent.width
                    height: parent.height
                }
                SettingsPanel
                {
                    id: settingsPanel;
                    width: parent.width
                    height: parent.height
                }
                DownloadPanel
                {
                    id: downloadPanel;
                    width: parent.width
                    height: parent.height
                }
                AboutPanel
                {
                    id: aboutPanel;
                    width: parent.width
                    height: parent.height
                }
            }
        }
    }
}
