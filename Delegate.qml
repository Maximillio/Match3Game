import QtQuick 2.0

Rectangle {
    id: rectangle

    color: colorValue
    width: parent.width/ match3Model.colsNum()
    height: parent.height/ match3Model.rowsNum() / 2
    border.color: "#000000"
    border.width: 1
    visible: true
    opacity: isChecked ? 0 : 1
    Text {
        anchors.centerIn: parent
        //text: model.typeValue
        font.pixelSize: parent.height / 2
    }
    states: [
        State {
            name: "clicked"
            PropertyChanges {
                target: girdView.currentItem;
                border.color: "#222222";
                border.width: 3;
            }
        }
    ]
    Behavior on opacity { NumberAnimation {duration: 300;} }
}
