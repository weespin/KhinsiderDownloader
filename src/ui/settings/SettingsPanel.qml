import QtQuick
import QtQuick.Dialogs
import "../shared"
import QtQml
import QtQuick.Layouts
import QtQuick
Rectangle {
    color: "#2c3e50"
    height: 700
    width: 400

    Connections{
        target: appController.settingsVM
        onUpdateFound: {
            messageDialog.visible = true;
        }
    }

    UpdateCheckerDialog {
        id: messageDialog
        onAccepted: {
            Qt.openUrlExternally("https://github.com/weespin/KhinsiderDownloader/releases")
        }
        Component.onCompleted: visible = false
    }
    FolderDialog {
        id: folderDialog

        currentFolder: appController.settingsVM.settings.downloadPath
        selectedFolder: appController.settingsVM.settings.downloadPath

        onAccepted: {
            appController.settingsVM.settings.setDownloadPath(selectedFolder);
        }
    }
    Column {
        id:maincolumn
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 10
        topPadding: 10
        width: parent.width * 0.9

        Row {
            height: 40
            spacing: 10
            width: parent.width

            Rectangle {
                color: "#6c98c4"
                height: parent.height
                radius: 10
                width: parent.width * 0.7

                Text {
                    anchors.fill: parent
                    anchors.leftMargin: 8
                    color: "#ffffff"
                    font.pointSize: 12
                    text: "Path: " + appController.settingsVM.settings.downloadPath
                    elide: Text.ElideRight
                    verticalAlignment: Text.AlignVCenter
                    MouseArea
                    {
                        anchors.fill: parent;
                        onClicked:
                        {
                            Qt.openUrlExternally("file://" + appController.settingsVM.settings.downloadPath);
                        }
                    }
                }
            }
            WButton {
                fontSize: 12

                height: parent.height
                label: "Select Path"
                width: parent.width * 0.25

                onClicked: {
                    folderDialog.open();
                }
            }
        }
        Row {
            height: 40
            spacing: 10
            width: parent.width

            Rectangle {
                color: "#6c98c4"
                height: parent.height
                radius: 10
                width: parent.parent.width * 0.7
                RowLayout
                {
                    id: logrow
                    anchors.fill: parent
                    anchors.leftMargin: 8
                    anchors.rightMargin: 8
                    Text {

                        color: "#ffffff"
                        font.pointSize: 12
                        text: "Enable Logging"
                        verticalAlignment: Text.AlignVCenter
                    }
                    Item {
                        Layout.fillWidth: true
                    }
                    Text {
                        color: "#99ffffff"
                        font.pointSize: 12
                        text: "Open Log Path"
                        verticalAlignment: Text.AlignVCenter
                        MouseArea
                        {
                            width: parent.width
                            height: logrow.height
                            y: logrow.y - parent.y
                            onClicked:
                            {
                                Qt.openUrlExternally("file:///" +  appController.logController.logDir);
                            }
                        }
                    }
                }


            }
            WEnumButton {
                height: parent.height
                width: parent.parent.width * 0.25
                fontSize: 13
                onValueChanged:
                {
                    appController.settingsVM.settings.setSuppressLogs(selectedIndex == 0);
                }
                selectedIndex: appController.settingsVM.settings.suppressLogs ? 0 : 1;
                Component.onCompleted: {
                    model.clear();
                    model.append({
                                     text: "True"
                                 });
                    model.append({
                                     text: "False"
                                 });
                    selectedIndex = 1
                    selectedIndex = 0
                    selectedIndex = appController.settingsVM.settings.suppressLogs ? 0 : 1;
                }

                //True false
            }
        }
        Row {
            height: 40
            spacing: 10
            width: parent.width

            Rectangle {
                color: "#6c98c4"
                height: parent.height
                radius: 10
                width: parent.width * 0.7

                Text {
                    anchors.fill: parent
                    anchors.leftMargin: 8
                    color: "#ffffff"
                    font.pointSize: 12
                    text: "Threads"
                    verticalAlignment: Text.AlignVCenter
                }
            }
            WNumberBox {
                height: parent.height
                currentNumber: appController.settingsVM.settings.downloadThreads
                nextNumber: appController.settingsVM.settings.downloadThreads
                onValueChanged:
                {
                    appController.settingsVM.settings.setDownloadThreads(currentNumber);
                }
                maxNumber: 64
                minNumber: 1
                width: parent.width * 0.25
            }

        }
        Row {
            height: 40
            spacing: 10
            width: parent.width

            Rectangle {
                color: "#6c98c4"
                height: parent.height
                radius: 10
                width: parent.width * 0.7

                Text {
                    anchors.fill: parent
                    anchors.leftMargin: 8
                    color: "#ffffff"
                    font.pointSize: 12
                    text: "Downloads per thread"
                    verticalAlignment: Text.AlignVCenter
                }
            }
            WNumberBox {
                height: parent.height
                currentNumber: appController.settingsVM.settings.maxConcurrentDownloadsPerThread
                nextNumber: appController.settingsVM.settings.maxConcurrentDownloadsPerThread
                onValueChanged:
                {
                    appController.settingsVM.settings.setMaxConcurrentDownloadsPerThread(currentNumber);
                }
                maxNumber: 256
                minNumber: 0
                width: parent.width * 0.25
            }

        }
        Row {
            height: 40
            spacing: 10
            width: parent.width

            Rectangle {
                color: "#6c98c4"
                height: parent.height
                radius: 10
                width: parent.width * 0.7

                Text {
                    anchors.fill: parent
                    anchors.leftMargin: 8
                    color: "#ffffff"
                    font.pointSize: 12
                    text: "Audio Quality"
                    verticalAlignment: Text.AlignVCenter
                }
            }
            WEnumButton {
                height: parent.height
                width: parent.width * 0.25
                fontSize: 13
                onValueChanged:
                {
                    appController.settingsVM.settings.setPreferredAudioQualityInt(selectedIndex);
                }
                selectedIndex: appController.settingsVM.settings.preferredAudioQuality;
                Component.onCompleted: {
                    model.clear();
                    model.append({
                                     text: "MP3"
                                 });
                    model.append({
                                     text: "Best"
                                 });
                    selectedIndex = 1
                    selectedIndex = 0
                    selectedIndex = appController.settingsVM.settings.preferredAudioQuality;
                }


                //MP3/Best
            }
        }
        Row {
            height: 40
            spacing: 10
            width: parent.width

            Rectangle {
                color: "#6c98c4"
                height: parent.height
                radius: 10
                width: parent.width * 0.7

                Text {
                    anchors.fill: parent
                    anchors.leftMargin: 8
                    color: "#ffffff"
                    font.pointSize: 12
                    text: "Download Art Covers"
                    verticalAlignment: Text.AlignVCenter
                }
            }
            WEnumButton
            {
                height: parent.height
                width: parent.width * 0.25
                fontSize: 13
                onValueChanged:
                {
                    appController.settingsVM.settings.setDownloadArt(selectedIndex == 0);
                }
                selectedIndex: appController.settingsVM.settings.downloadArt ? 0 : 1;
                Component.onCompleted: {
                    model.clear();
                    model.append({
                                     text: "True"
                                 });
                    model.append({
                                     text: "False"
                                 });
                    selectedIndex = 1
                    selectedIndex = 0
                    selectedIndex = appController.settingsVM.settings.downloadArt ? 0 : 1;
                }

            }


        }
        Row {
            height: 40
            spacing: 10
            width: parent.width

            Rectangle {
                color: "#6c98c4"
                height: 40
                radius: 10
                width: maincolumn.width * 0.7

                Text {
                    anchors.fill: parent
                    anchors.leftMargin: 8

                    color: "#ffffff"
                    font.pointSize: 12
                    text: "Skip Downloaded"
                    verticalAlignment: Text.AlignVCenter
                }
            }

            WEnumButton
            {
                height: parent.height
                width: parent.width * 0.25
                fontSize: 13
                onValueChanged:
                {
                    appController.settingsVM.settings.setSkipDownloaded(selectedIndex == 0);
                }
                selectedIndex: appController.settingsVM.settings.skipDownloaded ? 0 : 1;
                Component.onCompleted: {
                    model.clear();
                    model.append({
                                     text: "True"
                                 });
                    model.append({
                                     text: "False"
                                 });
                    selectedIndex = 1
                    selectedIndex = 0
                    selectedIndex = appController.settingsVM.settings.skipDownloaded ? 0 : 1;
                }

                //True false
            }


        }

    }
}
