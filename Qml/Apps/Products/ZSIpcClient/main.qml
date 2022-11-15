import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import Qt.labs.settings 1.0
//import ZSSysGUI 1.0

ApplicationWindow {
    id: root
    visible: true

    x: 20
    y: 20
    width: 800
    height: 600

    Component.onCompleted: {
        ensureValidWindowPosition()
    }
    Component.onDestruction: {
        saveScreenLayout()
    }

    Settings {
        id: settings

        property alias x: root.x
        property alias y: root.y
        property alias width: root.width
        property alias height: root.height

        property var desktopAvailableWidth
        property var desktopAvailableHeight
    }

    function saveScreenLayout() {
        console.debug("-> root.saveScreenLayout(desktopAvailableWidth: " + Screen.desktopAvailableWidth + ", desktopAvailableHeight: " +Screen.desktopAvailableHeight + ")")
        settings.desktopAvailableWidth = Screen.desktopAvailableWidth
        settings.desktopAvailableHeight = Screen.desktopAvailableHeight
        console.debug("<- root.saveScreenLayout()")
    }

    function ensureValidWindowPosition() {
        console.debug("-> root.ensureValidWindowPosition()")
        console.debug("   root.ensureValidWindowPosition: settings.x: " + settings.x)
        console.debug("   root.ensureValidWindowPosition: settings.y: " + settings.y)
        console.debug("   root.ensureValidWindowPosition: settings.desktopAvailableWidth: " + settings.desktopAvailableWidth)
        console.debug("   root.ensureValidWindowPosition: settings.desktopAvailableHeight: " + settings.desktopAvailableHeight)
        console.debug("   root.ensureValidWindowPosition: Screen.desktopAvailableWidth: " + Screen.desktopAvailableWidth)
        console.debug("   root.ensureValidWindowPosition: Screen.desktopAvailableHeight: " + Screen.desktopAvailableHeight)
        var isSavedScreenLayout = (settings.desktopAvailableWidth === Screen.desktopAvailableWidth)
                               && (settings.desktopAvailableHeight === Screen.desktopAvailableHeight)
        root.x = (isSavedScreenLayout) ? settings.x : Screen.width / 2 - root.width / 2
        root.y = (isSavedScreenLayout) ? settings.y : Screen.height / 2 - root.height / 2
        console.debug("   root.ensureValidWindowPosition: root.x: " + root.x)
        console.debug("   root.ensureValidWindowPosition: root.y: " + root.y)
        console.debug("<- root.ensureValidWindowPosition()")
    }

    Rectangle {
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            bottom: parent.bottom
        }
        color: "lightblue"
        Component.onCompleted: {
            console.debug("-> Rectangle.onCompleted(x: " + x + ", y: " +y + ", width: " + width + ", height: " + height + ")")
            console.debug("<- Rectangle.onCompleted")
        }
    }

    /*
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

    header: Rectangle {
        height: 20
        width: parent.width
        color: "green"
    }

    Rectangle {
        anchors {
            left: parent.left
            right: parent.right
            top: header.bottom
            bottom: footer.top
        }
        color: "lightblue"
        Component.onCompleted: {
            console.debug("-> Rectangle.onCompleted")
            console.debug("<- Rectangle.onCompleted")
        }
    }

    footer: Rectangle {
        height: 20
        width: parent.width
        color: "red"
    }
    */
}
