import QtQuick 2.15
import QtQuick.Controls.Basic

ScrollView {
    id: scrollView
    clip: true
       ScrollBar.vertical.policy: ScrollBar.AlwaysOn
    //contentItem: null

    //ScrollBar.vertical :
    //   ScrollBar {
    //   id: scrollBar
    //   active: false
    //   visible: false
    //   clip: true
    //   height: scrollView.availableHeight
    //   hoverEnabled: false
    //   parent: scrollView.parent
    //   policy: ScrollBar.AlwaysOff
    //   width: 10
    //   x: scrollView.mirrored ? 0 : scrollView.width - width
    //   y: scrollView.topPadding
    //   opacity: scrollView.contentHeight > scrollView.height ? 1 : 0

    //   contentItem: Rectangle {
    //       color: "#759fc7"
    //       implicitWidth: 6
    //       radius: width / 2
    //   }

    //   Behavior on opacity {
    //       NumberAnimation {
    //           duration: 100
    //           easing.type: Easing.InOutQuad
    //       }
    //   }
    //}
}
