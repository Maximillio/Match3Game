import QtQuick 2.0

Rectangle {
    id: rectangle

    property int typeValue: 1// type

    width: parent.width/match3Model.fieldRowCount()
    height: parent.height/match3Model.fieldColumnCount()
    color: {
        switch (typeValue) {
        case 2: return "#eb5858";
        case 3: return "#1E90FF";
        case 4: return "#6de54c";
        case 5: return "#e0ea2c";
        case 6: return "#b802fd";
        case 7: return "#FFFAF0";
        default: return "#ff7cff";
        }
    }
    border.color: "#000000"
    border.width: 1
    visible: true
    Text {
        anchors.centerIn: parent
        text: typeValue
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
    NumberAnimation on opacity { duration: 1000 }

}
