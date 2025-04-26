import QtQuick 2.15
import Qt5Compat.GraphicalEffects
import QtQuick.Layouts

Item {
       state: "errored"

       signal cancelRequested();
       signal retryRequested();
       property var donwloadedSongs;
       property var totalSongs;
       property var speedInBytes;
       function formatSpeed(bytesPerSecond) {
           if (bytesPerSecond >= 1024 * 1024) {
               return (bytesPerSecond / (1024 * 1024)).toFixed(2) + " MB/s";
           } else if (bytesPerSecond >= 1024) {
               return (bytesPerSecond / 1024).toFixed(2) + " KB/s";
           } else {
               return bytesPerSecond + " B/s";
           }
       }
       onDonwloadedSongsChanged:
       {
              fileslabel.text = "Files: " + donwloadedSongs + "/" + totalSongs;
       }

       onSpeedInBytesChanged:
       {
              downloadstatus.text =  "Speed: " + formatSpeed(speedInBytes);
       }

       states: [
              State {
                     name: "downloading"
                     PropertyChanges { target: percentagebar; color:"#4CAF50" }
                     PropertyChanges { target: mainrect; color:"#6C98C4" }
                     PropertyChanges { target: perclabel; visible: true}
                     PropertyChanges { target: downloadstatus; visible: true }
                     PropertyChanges { target: fileslabel; visible: true }
                     PropertyChanges { target: restartbutton; visible: false }

              },
              State {
                     name: "unparsed"
                     PropertyChanges { target: percentagebar; color:"transparent" }
                     PropertyChanges { target: mainrect; color:"#FFA000" }
                     PropertyChanges { target: perclabel; visible: false}
                     PropertyChanges { target: downloadstatus; visible: false }
                     PropertyChanges { target: fileslabel; visible: false }
                     PropertyChanges { target: restartbutton; visible: false }

              },
              State {
                     name: "parsed"
                     PropertyChanges { target: percentagebar; color:"transparent" }
                     PropertyChanges { target: mainrect; color:"#6C98C4" }
                     PropertyChanges { target: perclabel; visible: false}
                     PropertyChanges { target: downloadstatus; visible: true }
                     PropertyChanges { target: fileslabel; visible: true }
                      PropertyChanges { target: restartbutton; visible: false }

              },
              State {
                     name: "errored"
                     PropertyChanges { target: percentagebar; color:"transparent" }
                     PropertyChanges { target: mainrect; color:"#D32F2F" }
                     PropertyChanges { target: perclabel; visible: false}
                     PropertyChanges { target: downloadstatus; visible: false }
                     PropertyChanges { target: fileslabel; visible: true }
                     PropertyChanges { target: restartbutton; visible: true }

              }
       ]
       property alias progress: percentagebar.percentage;
       property alias label: albumname.text
       height: 50
       width: 400
       Rectangle
       {
              MouseArea{
                     width: parent.width
                     height: parent.height
                     hoverEnabled: true
                     onEntered:
                     {
                            downloaderRow.visible = false
                            darkoverlay.visible = true;
                     }
                     onExited:
                     {
                            downloaderRow.visible = true;
                            darkoverlay.visible = false;
                     }

              }

              layer.enabled: true
              layer.effect: OpacityMask{
                     maskSource: Item{
                            width: mainrect.width
                            height: mainrect.height
                            Rectangle{

                                   width: parent.width

                                   height: parent.height
                                   radius: 20
                            }
                     }
              }

              id:mainrect
              color: "#6C98C4" // background
              width: parent.width
              height: parent.height
              radius: 20

              Rectangle
              {
                     property int percentage;
                     id: percentagebar
                     color: "#4CAF50"
                     width: (percentagebar.percentage / 100) * mainrect.width
                     height: parent.height
                     radius: 20

                     onPercentageChanged:
                     {
                            percentagebar.width = (percentagebar.percentage / 100) * mainrect.width
                     }

                     Behavior on width {
                         NumberAnimation {
                             duration: 100
                             easing.type: Easing.InOutQuad
                         }
                     }
              }
              Row
              {
                     id:downloaderRow
                     width: parent.width
                     height: parent.height
                     Text
                     {

                            width: parent.width * 0.75

                            height: parent.height
                            id: albumname;
                            text:model.name;
                            elide: Text.ElideRight
                            horizontalAlignment: Text.AlignLeft
                            color:"white"
                            verticalAlignment: Text.AlignVCenter
                            leftPadding: 7
                            font.bold: false
                            font.pointSize: 14
                     }
                     Text
                     {
                            width: parent.width * 0.25
                            height: parent.height
                            id: perclabel;
                            text: percentagebar.percentage + "%"
                            color:"white"
                            elide: Text.ElideRight
                            horizontalAlignment: Text.AlignRight
                            verticalAlignment: Text.AlignVCenter
                            rightPadding: 10
                            font.bold: false
                            font.pointSize: 14
                     }
              }

              Rectangle {

                     visible: false
                     width: parent.width
                     height: parent.height
                     color: "#bc000000"
                     id: darkoverlay

                     Item {
                            anchors.horizontalCenter: parent.horizontalCenter
                            width: parent.width * 0.95
                            height: parent.height

                            RowLayout {
                                spacing: 10
                                width: parent.width
                                height: parent.height

                                Text {
                                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                                    id: fileslabel
                                    Layout.fillHeight: true
                                    //Layout.preferredWidth: implicitWidth;
                                    Layout.preferredWidth: Math.min(implicitWidth, parent.width * 0.4)
                                    Layout.maximumWidth: parent.width * 0.4
                                    text: "Files: 50/136"
                                    elide: Text.ElideRight
                                    color: "white"
                                    horizontalAlignment: Text.AlignLeft
                                    verticalAlignment: Text.AlignVCenter
                                    font.pointSize: 14
                                }

                                Text {
                                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                                    id: downloadstatus
                                    Layout.fillHeight: true
                                    //Layout.preferredWidth: implicitWidth;
                                    Layout.preferredWidth: Math.min(implicitWidth, parent.width * 0.4)
                                    Layout.maximumWidth: parent.width * 0.4
                                    text: "10MB/20MB @50KB/s"
                                    elide: Text.ElideRight
                                    color: "white"
                                    horizontalAlignment: Text.AlignLeft
                                    verticalAlignment: Text.AlignVCenter
                                    font.pointSize: 14
                                }

                              //Item
                              //{
                              //   Layout.fillWidth: true
                              //   Layout.fillHeight: true
                              //}
                                Image {
                                    id: restartbutton
                                    Layout.preferredWidth: 60 * 0.5
                                    Layout.fillHeight: true
                                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                                    // Remove the anchors.right property
                                    visible: true
                                    property string iconFallback: "qrc:/icons/retry.svg"
                                    source: "../../../icons/retry.svg"
                                    fillMode: Image.PreserveAspectFit

                                    ColorOverlay {
                                        anchors.fill: parent
                                        source: parent
                                        color: "white"
                                    }
                                    MouseArea
                                    {
                                          hoverEnabled: false;
                                          width: parent.width
                                          height: parent.height
                                          onClicked:
                                          {
                                              retryRequested();
                                          }
                                    }

                                    onStatusChanged: {
                                        if (status === Image.Error) {
                                            source = iconFallback;
                                        }
                                    }
                                }
                                Image {
                                    id: deletebutton
                                    Layout.preferredWidth: 30
                                    Layout.preferredHeight: parent.height * 0.8
                                    Layout.fillHeight: true
                                    visible: true
                                    property string iconFallback: "qrc:/icons/delete.svg"
                                    source: "../../../icons/delete.svg"
                                    Layout.rightMargin: 5
                                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter

                                    fillMode: Image.PreserveAspectFit

                                    ColorOverlay {
                                        anchors.fill: parent
                                        source: parent
                                        color: "white"
                                    }
                                    MouseArea
                                    {
                                          hoverEnabled: false;
                                          width: parent.width
                                          height: parent.height
                                          onClicked:
                                          {
                                                 cancelRequested();
                                          }
                                    }

                                    onStatusChanged: {
                                        if (status === Image.Error) {
                                            source = iconFallback;
                                        }
                                    }
                                }
                            }
                     }
              }
       }
}
