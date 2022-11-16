import QtQuick 2.15
import QtGraphicalEffects 1.15
// QtQuick.Controls version above 2.0 cannot be imported in petalinux
//import QtQuick.Controls 1.6
// QtQuick.Controls version above 2.0 cannot be imported in petalinux
import QtQuick.Controls 2.0
// This is the latest 1.x version available for ubuntu 20.04
//import QtQuick.Controls 1.6
//import Qt.labs.settings 1.0


Rectangle {
    width: 400
    height: 500
    color: "lightGray"

    Column {
        anchors.centerIn: parent
        spacing: 10
        Text {
            text: "PressMe: "

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    console.debug("global lookup: ", _aErrLogEntry);
                    var errLogEntry = _aErrLogEntry;
                    console.debug("Err Log entry: ", errLogEntry);
                    errLogEntry.m_iRowIdx = 99;
                    //errLogEntry.m_dateTime
                    errLogEntry.m_fSysTime_us = 98983;
                    //errLogEntry.m_errResultInfo
                    errLogEntry.m_strProposal = "Geht scho";
                    errLogEntry.m_iOccurrences = 7;
                    console.debug("Qml Var: ", errLogEntry);
                }
            }
        }
    }
}

/*
ApplicationWindow {
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

    background: Rectangle {
        gradient: Gradient {
            GradientStop { position: 0; color: "#ffffff" }
            GradientStop { position: 1; color: "#000000" }
        }
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
    }

    footer: TabBar {
    }
}
*/
