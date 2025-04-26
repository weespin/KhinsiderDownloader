import QtQuick 2.15
import QtQuick.Controls
import "../shared"
Rectangle
{
       color: "#34495E"
       id:root
       height: 800
       width: 600
       WScrollView
       {
              id: scrollView

              width: parent.width
              height: parent.height * 0.9
              anchors.top: parent.top
              anchors.topMargin: 10
              anchors.bottom: s.top
              clip: true

              Column
              {

                     width: root.width * 0.92;
                     height: root.height;
                     leftPadding: 10
                     spacing:5
                     //anchors.fill: parent
                     Repeater
                     {
                            height:parent.height
                            model: appController.downloaderModel
                            width : parent.width
                            delegate: AlbumItem
                             {
                                    progress: model.progress
                                    state: model.state
                                    height: 45
                                    label: model.name
                                    width : parent.width
                                    donwloadedSongs: model.downloadedSongs
                                    totalSongs: model.totalSongs
                                    speedInBytes: model.speed;
                                    onCancelRequested: {
                                            appController.downloaderModel.cancelAlbum(model.index)
                                        }
                                    onRetryRequested :
                                    {
                                           appController.downloaderModel.retryAlbum(model.index);
                                    }
                             }
                     }

              }
       }
       Item
       {
              id: s
              width: parent.width
              height: parent.height * 0.1
              anchors.bottom: parent.bottom
              Rectangle {
                     width: parent.width
                     height: 1
              }
              Row
              {
                     width: parent.width
                     height: parent.height
              Text {
                     id: speedText
                     height: parent.height
                     width: parent.width/2
                     text: "";
                     color: "#ffffff"
                     font.pointSize: 16
                     horizontalAlignment: Text.AlignHCenter
                     verticalAlignment: Text.AlignVCenter
                     function formatSpeed(bytesPerSecond) {
                         if (bytesPerSecond >= 1024 * 1024) {
                             return (bytesPerSecond / (1024 * 1024)).toFixed(2) + " MB/s";
                         } else if (bytesPerSecond >= 1024) {
                             return (bytesPerSecond / 1024).toFixed(2) + " KB/s";
                         } else {
                             return bytesPerSecond + " B/s";
                         }
                     }
                     Connections {
                            target: appController.downloaderModel
                            function onTotalsChanged() {
                                speedText.text = speedText.formatSpeed(appController.downloaderModel.totalSpeed());
                            }
                        }
              }


              Text {
                      id: downloadedSizeText
                     height: parent.height
                     width: parent.width/2
                     font.pointSize: 16
                     color: "#ffffff"
                     horizontalAlignment: Text.AlignHCenter
                     verticalAlignment: Text.AlignVCenter
                     Connections {
                            target: appController.downloaderModel
                            function onTotalsChanged() {
                                downloadedSizeText.text = appController.downloaderModel.totalDownloadedSongs()
                                    + "/" + appController.downloaderModel.totalSongs();
                            }
                        }
              }
              }


       }
}

