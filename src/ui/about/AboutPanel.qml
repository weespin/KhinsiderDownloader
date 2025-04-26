import QtQuick
import QtQuick.Dialogs
import "../shared"
import QtQml
import QtQuick.Controls.Basic
import QtQml.XmlListModel
import QtQuick.Layouts
Rectangle {
    color: "#2c3e50"
    height: 700
    width: 400

    ColumnLayout {
        id:maincolumn
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 10
        width: parent.width * 0.9
        height: parent.height
        Text {
            color: "white"
            height: implicitHeight
            width: implicitWidth
            text: qsTr("KhinsiderDownloader QT")
            font.pointSize: 25
        }
        Text {
            color: "white"
            height: implicitHeight
            width: implicitWidth
            text: qsTr("Thanks to:")
            font.pointSize: 15
        }
        Rectangle
        {
            color: "#6C98C4"
            width: parent.width
            Layout.preferredHeight: parent.height * 0.6
            Layout.fillWidth: true
            radius: 10
            WScrollView {
                id: scrollView
                anchors.fill: parent

                XmlListModel {
                    id: contributorsModel
                    source: "qrc:/CONTRIBUTORS.xml"
                    query: "/contributors/contributor"

                    XmlListModelRole { name: "username"; elementName: "username" }
                    XmlListModelRole { name: "contributionType"; elementName: "contributionType" }
                }

                Column {
                    id: column
                    width: parent.width
                    height: parent.height
                    spacing: 5

                    Repeater {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10
                        anchors.topMargin: 10
                        anchors.bottomMargin: 10
                        width: parent.width
                        model: contributorsModel

                        Rectangle {
                            width: scrollView.width
                            height: contributorRow.height + 10
                            color: "transparent"

                            RowLayout {
                                id: contributorRow
                                width: scrollView.width

                                anchors.verticalCenter: parent.verticalCenter

                                Text {
                                    Layout.leftMargin: 10
                                    text: username
                                    color: "white"
                                    font.pointSize: 13
                                    elide: Text.ElideRight
                                    verticalAlignment: Text.AlignVCenter
                                }

                                Item {

                                    Layout.fillHeight: true
                                    Layout.fillWidth: true
                                }

                                Text {
                                    Layout.rightMargin: 10
                                    text: contributionType
                                    color: "white"
                                    font.pointSize: 13
                                    elide: Text.ElideRight
                                    verticalAlignment: Text.AlignVCenter
                                }
                            }
                        }
                    }
                }

            }
        }
        Item {
            Layout.fillHeight: true
        }
        RowLayout
        {
            Layout.fillWidth: true
            Layout.preferredHeight: implicitHeight * 2
            Text
            {
                Layout.fillWidth: true
                font.pointSize: 16
                color: "white"
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                text: "Version: " + appController.aboutController.appVersion
            }

            Text
            {
                Layout.fillWidth: true
                Layout.preferredHeight: implicitHeight
                text: "<a href=\"https://weesp.in\">Weespin</a> 2025"
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                textFormat: Text.RichText
                color: "white"
                onLinkActivated: Qt.openUrlExternally(link)
                font.pointSize: 16
            }
        }

    }
}
