import QtQuick 2.5
import QtQuick.Window 2.2


Window {
    id: window
    visible: true
    width: 600
    height: 675
    title: qsTr("Match 3")
    maximumHeight: height
    maximumWidth: width
    minimumHeight: height
    minimumWidth: width
    Item {
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        anchors.topMargin: dataPanel.height
        GridView {
            id: girdView

            anchors.fill: parent
            model: match3Model
            interactive: false
            cellWidth: height / match3Model.colsNum()
            cellHeight: width / match3Model.rowsNum()
            verticalLayoutDirection: GridView.BottomToTop
            delegate: Delegate {}

            MouseArea {
                id: mouseArea
                enabled: match3Model.readyForInput;
                property int firstClick: -1
                property int secondClick: -1

                anchors.fill: parent;
                onClicked: {
                    if (firstClick === -1)
                    {
                        firstClick               = parseInt(mouse.x / girdView.cellWidth) + (parseInt((girdView.height - mouse.y) / girdView.cellHeight)) * match3Model.colsNum();
                        parent.currentIndex      = firstClick;
                        parent.currentItem.state = "clicked"
                        secondClick              = -1;
                    }
                    else
                    {
                        parent.currentItem.state = ""
                        secondClick              = parseInt(mouse.x / girdView.cellWidth) + (parseInt((girdView.height - mouse.y) / girdView.cellHeight)) * match3Model.colsNum();
                        //console.log(firstClick + " and " + secondClick);
                        match3Model.swapCell(firstClick, secondClick);
                        firstClick               = -1;
                    }
                }
            }
            move: Transition {
                    NumberAnimation { properties: "x"; duration: match3Model.readyForInput ? 500 : 0;}
                    NumberAnimation { properties: "y"; duration: 500;}
                }
            displaced: Transition {
                    NumberAnimation { properties: "x"; duration: match3Model.readyForInput ? 500 : 0;}
                     NumberAnimation { properties: "y"; duration: 500;}
                 }
        }
    }
    Rectangle {
        id: dataPanel
        height: 75
        color: "#ebeb06"
        border.width: 2
        border.color: "#000000"
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top
        Text {
            anchors.centerIn: parent
            width: 100
            height: 50

            text: "Score: " + match3Model.score
            font.pointSize: 30
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }

    }
    Rectangle {
        width:  parent.width / 2
        height: parent.height / 4
        anchors.centerIn: parent
        visible: !match3Model.readyForDisplay
        Text {
            anchors.centerIn: parent
            text: "Loading..."
            font.pointSize: parent.height / 8
        }
    }

    Component.onCompleted: girdView.positionViewAtEnd()
}
