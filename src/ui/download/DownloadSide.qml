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
                            model: app.downloaderController.downloaderVM
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
                                   onCancelRequested:
                                   {
                                          app.downloaderController.downloaderVM.cancelAlbum(model.index)
                                   }
                                   onRetryRequested :
                                   {
                                          app.downloaderController.downloaderVM.retryAlbum(model.index);
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
                            opacity: 0
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
                            Behavior on opacity {
                                 NumberAnimation {
                                     duration: 100
                                     easing.type: Easing.InOutQuad
                                 }
                             }
                            Connections {
                                   target: app.downloaderController.downloaderVM
                                   function onTotalsChanged()
                                   {
                                          var shouldShowAnyData = app.downloaderController.downloaderVM.totalSongs() !== 0 ? 1 : 0;
                                          speedText.opacity = shouldShowAnyData ? 1 : 0;
                                          if(shouldShowAnyData)
                                          {
                                                 speedText.text = speedText.formatSpeed(app.downloaderController.downloaderVM.totalSpeed());
                                          }
                                   }
                            }
                     }


                     Text {
                            id: downloadedSizeText
                            height: parent.height
                            width: parent.width/2
                            font.pointSize: 16
                            color: "#ffffff"
                            opacity: 0
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            Behavior on opacity {
                                 NumberAnimation {
                                     duration: 100
                                     easing.type: Easing.InOutQuad
                                 }
                             }
                            Connections {
                                   target: app.downloaderController.downloaderVM
                                   function onTotalsChanged()
                                   {
                                          var shouldShowAnyData = app.downloaderController.downloaderVM.totalSongs() !== 0 ? 1 : 0;
                                          downloadedSizeText.opacity = shouldShowAnyData ? 1 : 0;
                                          if(shouldShowAnyData)
                                          {
                                                 downloadedSizeText.text = app.downloaderController.downloaderVM.totalDownloadedSongs()
                                                               + "/" + app.downloaderController.downloaderVM.totalSongs();
                                          }
                                   }
                            }
                     }
              }


       }
}

