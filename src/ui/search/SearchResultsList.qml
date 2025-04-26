import QtQuick 2.12
import QtQuick.Controls 2.12
import "../shared"

Item {
    id: root
    function hideAllResults()
    {
        root.isSearching = true;
        root.selectedIndex = -1;
    }

    property int selectedIndex: appController.searchVM.selectedIndex
    property bool isSearching: false

    height: 600
    width: 600

    WScrollView {
        id: scrollView

        anchors.fill: parent
        clip: true

        Column {
            id: repeater

            spacing: 5
            width: root.width

            Repeater {
                model: appController.searchVM.searchResult

                Rectangle {
                    id: rectangle

                    property bool isHovered: false
                    property bool isSelected: index === root.selectedIndex

                    color: isSelected || isHovered ? "#759fc7" : "#6c98c4"
                    height: 45
                    radius: 10
                    width: parent.width * 0.9
                    x: isSelected ? 0 : (parent.width - width) / 2
                    scale: root.isSearching ? 0 : isSelected ? 0.95 : mouseArea.pressed ? 0.90 : 1.0
                    opacity: root.isSearching ? 0 : 1
                    Behavior on scale {
                        NumberAnimation {
                            duration: 150
                        }
                    }
                    Behavior on x {
                        NumberAnimation {
                            duration: 150
                            easing.type: Easing.InOutQuad
                        }
                    }
                    Behavior on opacity {
                        NumberAnimation {
                            duration: 100
                            easing.type: Easing.InOutQuad
                        }
                    }

                    MouseArea {
                        id: mouseArea

                        anchors.fill: parent
                        hoverEnabled: true

                        onClicked: {
                            if (root.selectedIndex >= 0 && root.selectedIndex !== index) {
                                var prevItem = repeater.children[root.selectedIndex];
                                if (prevItem && prevItem.isHovered) {
                                    prevItem.isHovered = false;
                                }
                            }
                            rectangle.anchors.horizontalCenter = undefined;
                            root.selectedIndex = index;
                            appController.searchVM.setSelectedIndex(index);
                        }
                        onEntered: {
                            if (!isSelected) {
                                isHovered = true;
                            }
                        }
                        onExited: {
                            if (!isSelected) {
                                isHovered = false;
                            }
                        }
                    }
                    Text {
                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        anchors.right: parent.right
                        anchors.rightMargin: 10
                        color: "white"
                        elide: Text.ElideRight
                        font.bold: true
                        font.pointSize: 13
                        height: parent.height
                        horizontalAlignment: Text.AlignLeft
                        text: model.name
                        verticalAlignment: Text.AlignVCenter
                        width: parent.width
                    }
                }
            }
        }
    }
    Connections {
           target: appController.searchVM
           function onPerformSearch() {
               root.hideAllResults();
           }
       }

       Connections {
           target: appController.searchVM
           function onSearchResultChanged() {
               root.isSearching = false;
           }
       }
}
