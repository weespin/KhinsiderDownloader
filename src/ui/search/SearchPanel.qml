import QtQuick 2.12
import QtQuick.Controls.Basic
import "../shared"
import QtQuick.Layouts

Rectangle {
    width : 800
    id: mainWindow
    height : 500
    color: "#2c3e50"

    Connections
    {
        target: appController.searchVM
        function onSelectedIndexChanged()
        {
            mainWindow.state = "normal";
        }
    }
    state: "expanded"
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
    transitions: [
        Transition {
            from: "*"; to: "*"
            NumberAnimation {
                properties: "width, x"
                duration: 300
                easing.type: Easing.InOutQuad
            }
        }
    ]
    Row
    {
        id: mainRow
        width: parent.width
        height: parent.height


        Column {
            id: leftColumn
            topPadding: 10
            height: parent.height - 5
            spacing: 10

            Item{
                anchors.horizontalCenter: parent.horizontalCenter
                width:parent.width * 0.9

                height: 40

                //Search Box
                Row {
                    id: row1
                    spacing: parent.width * 0.05

                    height: 40
                    width: parent.width
                    Rectangle {
                        id: rectangle

                        width: parent.width * 0.75
                        color: "#6C98C4"

                        height: 40
                        radius: 15

                        RowLayout {
                            id: row

                            height: 40
                            width: parent.width

                            Item {
                                Layout.fillWidth: true
                                height: parent.height
                                Layout.alignment: Qt.AlignVCenter
                                width: parent.width * 0.9

                                TextField {
                                    id: textfield

                                    color: "#ffffff"
                                    font.pointSize: 13
                                    height: parent.height
                                    hoverEnabled: true
                                    placeholderText: "Search..."
                                    placeholderTextColor: "#b5ffffff"
                                    verticalAlignment: Text.AlignVCenter
                                    width: parent.width

                                    background: Rectangle {
                                        color: "#6C98C4" // match parent background
                                        radius: 10
                                    }
                                    onAccepted:
                                    {
                                        appController.searchVM.performSearch(textfield.text);
                                    }

                                    onHoveredChanged: {
                                        if (hovered) {
                                            onPlaceholderHover.start();
                                        } else {
                                            onPlaceholderUnHover.start();
                                        }
                                    }

                                    PropertyAnimation {
                                        id: onPlaceholderHover

                                        duration: 100
                                        property: "placeholderTextColor"
                                        target: textfield
                                        to: "#00ffffff" // ~70% opacity
                                    }
                                    PropertyAnimation {
                                        id: onPlaceholderUnHover

                                        duration: 100
                                        property: "placeholderTextColor"
                                        target: textfield
                                        to: "#b5ffffff" // original alpha
                                    }
                                }
                            }

                            Image {
                                id: icon
                                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter

                                fillMode: Image.PreserveAspectFit
                                height: parent.height
                                source: "qrc:/icons/search.svg"
                                Layout.rightMargin: 5

                                SequentialAnimation {
                                    id: shrinkAnim

                                    NumberAnimation {
                                        duration: 100
                                        property: "scale"
                                        target: icon
                                        to: 0.96
                                    }
                                }
                                SequentialAnimation {
                                    id: resetAnim

                                    NumberAnimation {
                                        duration: 100
                                        property: "scale"
                                        target: icon
                                        to: 1.0
                                    }
                                }
                                SequentialAnimation {
                                    id: growAnim

                                    NumberAnimation {
                                        duration: 100
                                        property: "scale"
                                        target: icon
                                        to: 1.1
                                    }
                                }
                                MouseArea {
                                    anchors.bottomMargin: 0
                                    anchors.fill: parent
                                    anchors.leftMargin: 0
                                    anchors.rightMargin: 0
                                    anchors.topMargin: 0
                                    hoverEnabled: true

                                    onClicked: {

                                        appController.searchVM.performSearch(textfield.text);
                                    }
                                    onEntered: {
                                        growAnim.running = true;
                                    }
                                    onExited: {
                                        resetAnim.running = true;
                                    }
                                    onPressed: {
                                        shrinkAnim.running = true;
                                    }
                                    onReleased: {
                                        growAnim.running = true;
                                    }
                                }
                            }
                        }
                    }
                    WButton {

                        height: 40
                        label: "Add All"
                        width: parent.width * 0.2
                        onClicked:
                        {
                            appController.searchVM.addAllAlbumsRequested();
                            searchList.hideAllResults();
                        }
                    }
                }
            }
            //Divider
            Rectangle {
                height: 1
                width: parent.width
            }
            //WButton {
            //    y: 0
            //    width: 140
            //    height: 40

            //    label: "StateTest"
            //    anchors.horizontalCenterOffset: 300
            //    onClicked:
            //    {
            //        console.log("test")
            //        mainWindow.state = (mainWindow.state === "normal") ? "expanded" : "normal"
            //    }
            //}
            //Search Info
            SearchResultsList {
                id: searchList;
                height: parent.height - rectangle.height - 30
                width: parent.width
            }
        }
        //SideColumn
        Column{
            id:rightColumn
            height: parent.height
            width: parent.width

            // Right panel
            Rectangle {
                id: rightPanel
                width: parent.width
                height: parent.height
                color: "#34495e"


                AlbumInfoSide
                {
                    width: parent.width
                    height: parent.height
                }
            }
        }
    }
}
