import QtQuick 2.15

Rectangle {
    property alias text: textInput.text
    signal returnPressed(string text)
    border.color: "green"
    color: "white"
    radius: 4; smooth: true
    height: 30
    clip: true

    TextInput {
        id: textInput
        anchors {
            fill: parent
            margins: 2
        }
        color: focus ? "black" : "gray"
        font.pixelSize: parent.height - 4
        Keys.onReturnPressed: {
            parent.returnPressed(text)
        }
    }
}
