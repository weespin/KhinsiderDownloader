import QtQuick 2.15
//TODO PLEASE REMAKE ME! MY PARENTS ARE FORCING TO CHANGE MY INDEX BECAUSE I DON'T KNOW HOW TO RESET MY MODEL PROPERLY!
Rectangle {
    signal valueChanged();
    property int selectedIndex: 0
    property alias model: internalModel
    ListModel {
        id: internalModel
        ListElement { text: "First";}
        ListElement { text: "Second";}
    }
    property alias fontSize: buttonlabel.font.pointSize
    property alias label: buttonlabel.text
    id: buttonRect
    width: 40
    radius: 107
    height: 40
    color: "#6c98c4"
    scale: 1.0
    Behavior on color {
        ColorAnimation {
            duration: 150
        }
    }

    SequentialAnimation {
        id: shrinkAnim
        NumberAnimation { target: buttonRect; property: "scale"; to: 0.96; duration: 100 }
    }

    SequentialAnimation {
        id: growAnim
        NumberAnimation { target: buttonRect; property: "scale"; to: 1.0; duration: 100 }
    }
    SequentialAnimation {
        id: fadeIn
        NumberAnimation { target: buttonlabel; property: "opacity"; to: 1; duration: 100 }
    }
    SequentialAnimation {
        id: fadeOut
        NumberAnimation { target: buttonlabel; property: "opacity"; to: 0; duration: 100 }
        onStopped:
        {
            //wrap imageTarget
            if(buttonRect.selectedIndex > internalModel.count)
            {
                buttonRect.selectedIndex = 0;
            }
            buttonRect.selectedIndex = (buttonRect.selectedIndex + 1) % internalModel.count;
            valueChanged();
            fadeIn.start();
        }
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onEntered: buttonRect.color = "#5a87b3"
        onExited: buttonRect.color = "#6c98c4"
        onPressed: {
            shrinkAnim.running = true
        }
        onReleased: {
            growAnim.running = true
        }
        onClicked:
        {
            fadeOut.start()
        }
    }

    Text {
        id: buttonlabel
        color: "white"
        text: internalModel.get(selectedIndex).text
        elide: Text.ElideRight
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pointSize: 10
    }
}
