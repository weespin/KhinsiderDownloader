import QtQuick 2.15

Rectangle {
    id: buttonRect

    signal clicked();
    property alias label: buttonlabel.text

    property alias fontSize : buttonlabel.font.pointSize
    color: "#6c98c4"
    height: 40
    //width: parent.width * 0.5
    radius: 107
    scale: 1.0

    Behavior on color {
        ColorAnimation {
            duration: 150
        }
    }

    SequentialAnimation {
        id: shrinkAnim

        NumberAnimation {
            duration: 100
            property: "scale"
            target: buttonRect
            to: 0.96
        }
    }
    SequentialAnimation {
        id: growAnim

        NumberAnimation {
            duration: 100
            property: "scale"
            target: buttonRect
            to: 1.0
        }
    }
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true

        onEntered: buttonRect.color = "#5a87b3"
        onExited: buttonRect.color = "#6c98c4"
        onPressed: {
            shrinkAnim.running = true;
        }
        onReleased: {
            growAnim.running = true;
        }
        onClicked:
        {
            buttonRect.clicked();
        }
    }
    Text {
        id: buttonlabel

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.verticalCenter: parent.verticalCenter
        color: "white"
        elide: Text.ElideRight
        font.pointSize: 16
        horizontalAlignment: Text.AlignHCenter
        text: "Add FLAC"
        verticalAlignment: Text.AlignVCenter
    }
}
