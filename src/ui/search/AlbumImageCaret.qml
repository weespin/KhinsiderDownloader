import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects
import QtQuick.Layouts

ColumnLayout{
    id:col
    width:300
    height:300
    property int imageIndex: 0
    property int imageTarget : 0
    property var imageSources: appController.albumInfoVM.albumImages
    Connections {
        target: appController.albumInfoVM
        function onCurrentAlbumChanged()
        {
            col.imageIndex = 0
            col.imageTarget = 0
            fadeOut.start()
            if(appController.albumInfoVM.currentAlbum.isInfoParsed)
            {
                blur.radius = 0;
            }
            else
            {
                blur.radius = 30;
            }
            blur.source = profileImage;
        }
    }
    Item {
       Layout.fillHeight: true
       Layout.fillWidth: true
        Image {
            SequentialAnimation {
                id: fadeIn
                NumberAnimation { target: blur; property: "opacity"; to: 1; duration: 100 }
                NumberAnimation { target: profileImage; property: "opacity"; to: 1; duration: 100 }
            }
            SequentialAnimation {
                id: fadeOut
                NumberAnimation { target: blur; property: "opacity"; to: 0; duration: 100 }
                NumberAnimation { target: profileImage; property: "opacity"; to: 0; duration: 100 }
                onStopped:
                {
                    //wrap imageTarget
                    if(imageTarget < 0)
                    {
                        imageTarget = col.imageSources.length - 1;
                    }

                    col.imageIndex = (imageTarget) % col.imageSources.length;
                    fadeIn.start();
                    imageTarget = col.imageIndex;
                }
            }
            cache: true
            id: profileImage
            anchors.fill: parent
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            anchors.topMargin: 10
            anchors.bottomMargin: 10
            source: col.imageSources[col.imageIndex]
            //source: "icons/albumplaceholder.jpg"
            fillMode: Image.PreserveAspectFit
            layer.enabled: true
            layer.effect: OpacityMask{
                maskSource: Item{
                    width: profileImage.width
                    height: profileImage.height
                    Rectangle{
                        anchors.centerIn: parent
                        width: Math.min(profileImage.width, profileImage.height)
                        height: width
                        radius: 10
                    }
                }
            }

        }
        FastBlur {
            id: blur
            cached:false
            anchors.fill: profileImage
            source: profileImage
            radius: 30
            Behavior on radius {
                NumberAnimation {
                    duration: 100
                    easing.type: Easing.InOutQuad
                }
            }
            layer.enabled: true
            layer.effect: OpacityMask{
                maskSource: Item{
                    width: profileImage.width
                    height: profileImage.height
                    Rectangle{
                        anchors.centerIn: parent
                        width: Math.min(profileImage.width, profileImage.height)
                        height: width
                        radius: 10
                    }
                }
            }
        }
        BusyIndicator {
            id: busyIndicator
            anchors.fill: profileImage
            running: {
                if (!appController.albumInfoVM.currentAlbum)
                    return false;
                return !appController.albumInfoVM.currentAlbum.isInfoParsed || profileImage.progress !== 1;
            }
            layer.enabled: true
            layer.effect:ColorOverlay{
                antialiasing: true
                color: "#ce60f7ff"
            }
        }
    }

    Rectangle {
        color: "#6C98C4"
        width: parent.width * 0.8
        radius: 10
        Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
        height: 45
        Row
        {
            id: row
            height: parent.height
            width:parent.width
            AlbumImageCaretButton {
                width: parent.width*0.2
                height: parent.height
                mirror: false
                onRequestImageChange:
                {
                    if(col.imageSources.length > 1)
                    {
                        col.imageTarget--;
                        fadeOut.start();
                    }
                }
            }
            Text {
                id:indextextlabel
                property string _text: "0/0";
                text: _text

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.weight: Font.DemiBold
                font.bold: false
                textFormat: Text.AutoText
                font.pointSize: 16
                width: parent.width*0.6
                height: parent.height
                color: "#ffffff"
                SequentialAnimation {
                    id: fadeTextOut
                    NumberAnimation { target: indextextlabel; property: "opacity"; to: 0; duration: 50 }
                    onStopped: {
                        // Update the text after the fade-out
                        indextextlabel._text = (col.imageIndex + 1) + "/" + col.imageSources.length
                        fadeTextIn.start()  // Start the fade-in animation
                    }
                }

                SequentialAnimation {
                    id: fadeTextIn
                    NumberAnimation { target: indextextlabel; property: "opacity"; to: 1; duration: 50 }
                }
                function checkAndFade()
                {

                    if ((col.imageIndex + 1) + "/" + col.imageSources.length !== indextextlabel._text)
                    {
                        fadeTextOut.start();
                    }
                    }
                Connections
                {
                    target: col
                    function onImageSourcesChanged()
                    {
                        indextextlabel.checkAndFade();
                    }
                    function onImageIndexChanged()
                    {
                          indextextlabel.checkAndFade();
                    }

                }
            }
            AlbumImageCaretButton
            {
                width: parent.width*0.2
                height: parent.height
                onRequestImageChange:
                {
                    if(col.imageSources.length > 1)
                    {
                        col.imageTarget++;
                        fadeOut.start();
                    }
                }
            }
        }

    }
}

