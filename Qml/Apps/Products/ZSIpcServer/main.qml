import QtQuick 2.15
import QtGraphicalEffects 1.15
import QtQuick.Controls 2.15
import Qt.labs.settings 1.0
import ZSSysGUI 1.0

ApplicationWindow {
    property var myTrcAdminObj: _trcServer.getTraceAdminObj(
            "ZS::Apps::Products::IpcServer", "main.qml", "theInst");
    Component.onDestruction: {
        _trcServer.releaseTraceAdminObj(root.myTrcAdminObj);
    }

    id: root
    visible: true
    width: 800
    height: 600

    Settings {
        property alias x: root.x
        property alias y: root.y
        property alias width: root.width
        property alias height: root.height
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            Action { text: qsTr("&New...") }
            Action { text: qsTr("&Open...") }
            Action { text: qsTr("&Save") }
            Action { text: qsTr("Save &As...") }
            MenuSeparator { }
            Action { text: qsTr("&Quit") }
        }
        Menu {
            title: qsTr("&Edit")
            Action { text: qsTr("Cu&t") }
            Action { text: qsTr("&Copy") }
            Action { text: qsTr("&Paste") }
        }
        Menu {
            title: qsTr("&Help")
            Action { text: qsTr("&About") }
        }
    }

    header: ToolBar {
        height: 32
        width: parent.width

        Rectangle {
            anchors.fill: parent
            color: "blue"
        }
    }

    background: Rectangle {
        color: "aqua"
        /*
        gradient: Gradient {
            GradientStop { position: 0; color: "#ffffff" }
            GradientStop { position: 1; color: "#000000" }
        }
        */
    }

    ErrLogWdgt {
        anchors.fill: parent
    }

    footer: Rectangle {
        height: 32
        width: parent.width
        color: "darkslateblue"
    }
}
