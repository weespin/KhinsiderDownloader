import QtQuick

Image {

    signal requestImageChange()

    SequentialAnimation {
        id: shrinkAnim
        NumberAnimation { target: arrow; property: "scale"; to: 0.96; duration: 100 }
    }

    SequentialAnimation {
        id: resetAnim
        NumberAnimation { target: arrow; property: "scale"; to: 1.0; duration: 100 }
    }
    SequentialAnimation {
        id: growAnim
        NumberAnimation { target: arrow; property: "scale"; to: 1.1; duration: 100 }
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onPressed: {
            shrinkAnim.running = true
        }
        onReleased: {
            growAnim.running = true
        }
        onEntered:
        {
            growAnim.running = true
        }
        onExited:
        {
            resetAnim.running = true
        }
        onClicked:
        {
            requestImageChange();
        }
    }

    id: arrow
    source: "qrc:/icons/arrowleft.svg"
    mirror: true
    fillMode: Image.PreserveAspectFit

}
