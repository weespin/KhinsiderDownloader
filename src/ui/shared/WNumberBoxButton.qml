import QtQuick 2.15

Text
{
    SequentialAnimation {
        id: onUnpressed
        NumberAnimation { target: control; property: "y"; to: -4; duration: 100 }
    }
    SequentialAnimation {
        id: onPressed
        NumberAnimation { target: control; property: "y"; to: isPlus ? -10 : 3 ; duration: 100 }
        onStopped:
        {
            onUnpressed.start()
        }
    }

    signal invoked() // SIGNAL!
    SequentialAnimation {
        id: onUnhover
        NumberAnimation { target: control; property: "scale"; to: 1; duration: 100 }
    }

    SequentialAnimation {
        id: onHover
        NumberAnimation { target: control; property: "scale"; to: 1.3; duration: 100 }
    }
    property bool isPlus: false
    id: control

    color:"white"
    text: isPlus ? "+" : "-"
    horizontalAlignment: Text.AlignHCenter
    verticalAlignment: Text.AlignVCenter
    font.pointSize: 25
    MouseArea
    {
        anchors.fill: parent
        hoverEnabled: true
        onEntered: {

            onHover.start()
        }

        onExited: {
            onUnhover.start()
        }
        onClicked:
        {
            invoked();
            onPressed.start()
        }
    }
}
