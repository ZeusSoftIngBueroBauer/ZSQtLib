import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import Qt.labs.settings 1.0
import ZSSysGUI 1.0

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
        settings.desktopAvailableWidth = Screen.desktopAvailableWidth
        settings.desktopAvailableHeight = Screen.desktopAvailableHeight
    }

    function ensureValidWindowPosition() {
        var isSavedScreenLayout = (settings.desktopAvailableWidth === Screen.desktopAvailableWidth)
                               && (settings.desktopAvailableHeight === Screen.desktopAvailableHeight)
        root.x = (isSavedScreenLayout) ? settings.x : Screen.width / 2 - root.width / 2
        root.y = (isSavedScreenLayout) ? settings.y : Screen.height / 2 - root.height / 2
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

        ListModel {
            id: nameModel
            ListElement { name: "0: Alice"; team: "Crypto" }
            ListElement { name: "1: Bob"; team: "Crypto" }
            ListElement { name: "2: Jane"; team: "Crypto" }
            ListElement { name: "3: Victor"; team: "Crypto" }
            ListElement { name: "4: Wendy"; team: "Crypto" }
            ListElement { name: "5: Alice"; team: "Crypto" }
            ListElement { name: "6: Bob"; team: "Crypto" }
            ListElement { name: "7: Jane"; team: "Crypto" }
            ListElement { name: "8: Victor"; team: "Crypto" }
            ListElement { name: "9: Wendy"; team: "Crypto" }
            ListElement { name: "10: Alice"; team: "Crypto" }
            ListElement { name: "11: Bob"; team: "Crypto" }
            ListElement { name: "12: Jane"; team: "QA" }
            ListElement { name: "13: Victor"; team: "QA" }
            ListElement { name: "14: Wendy"; team: "QA" }
            ListElement { name: "15: Alice"; team: "QA" }
            ListElement { name: "16: Bob"; team: "QA" }
            ListElement { name: "17: Jane"; team: "QA" }
            ListElement { name: "18: Victor"; team: "QA" }
            ListElement { name: "19: Wendy"; team: "QA" }
            ListElement { name: "20: Alice"; team: "QA" }
            ListElement { name: "21: Bob"; team: "QA" }
            ListElement { name: "22: Jane"; team: "QA" }
            ListElement { name: "23: Victor"; team: "Graphics" }
            ListElement { name: "24: Wendy"; team: "Graphics" }
            ListElement { name: "25: Alice"; team: "Graphics" }
            ListElement { name: "26: Bob"; team: "Graphics" }
            ListElement { name: "27: Jane"; team: "Graphics" }
            ListElement { name: "28: Victor"; team: "Graphics" }
            ListElement { name: "29: Wendy"; team: "Graphics" }
        }

        Component {
            id: nameDelegate

            Text {
                readonly property ListView __lv: ListView.view
                //property bool expanded: ListView.view.isSectionExpanded(model.team)

                text: model.name
                font.pointSize: 24
                width: __lv.width
                height: implicitHeight //expanded ? implicitHeight : 0
                clip: true

                Behavior on height {
                    NumberAnimation { duration: 200 }
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        nameModel.insert(index, {name: "Item #" + nameModel.count, team: "Graphics"})
                        //__lv.currentIndex = model.index
                    }
                }
            }
        }

        /*
        ListView {
            id: listView
            property var collapsed: ({})
            //property var something: true

            anchors {left: parent.left; right: parent.right; top: parent.top; bottom: label.top}

            model: nameModel
            delegate: nameDelegate
            focus: true
            clip: true
            section {
                property: "team"
                criteria: ViewSection.FullString
                delegate: Rectangle {
                    anchors { left: parent.left; right: parent.right }
                    color: "#b0dfb0"
                    implicitHeight: txt.implicitHeight + 4
                    signal clicked()
                    Text {
                        id: txt
                        anchors.centerIn: parent
                        font.pixelSize: 16
                        font.bold: true
                        text: section
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: listView.toggleSection(section)
                    }
                }
            }
            header: Rectangle {
                anchors { left: parent.left; right: parent.right }
                height: 10
                color: "pink"
            }
            footer: Rectangle {
                anchors { left: parent.left; right: parent.right }
                height: 10
                color: "blue"
            }
            highlight: Rectangle {
                anchors { left: parent.left; right: parent.right }
                color: "lightgray"
            }
            populate: Transition {
                NumberAnimation {properties: "x, y"; duration: 300}
            }
            add: Transition {
                PropertyAction {property: "transformOrigin"; value: Item.TopLeft}
                NumberAnimation {property: "opacity"; from: 0; to: 1.0; duration: 200}
                NumberAnimation {property: "scale"; from: 0; to: 1.0; duration: 200}
            }
            displaced: Transition {
                PropertyAction {properties: "opacity, scale"; value: 1}
                NumberAnimation {properties: "x, y"; duration: 200}
            }
            function isSectionExpanded(section) {
                return !(section in collapsed)
            }
            function showSection(section) {
                delete collapsed[section]
                collapsedChanged();
                //somethingChanged();
            }
            function hideSection(section) {
                collapsed[section] = true
                collapsedChanged();
                //somethingChanged();
            }
            function toggleSection(section) {
                if( isSectionExpanded(section) ) {
                    hideSection(section)
                } else {
                    showSection(section);
                }
            }
        }
        */

        PathView {
            anchors {left: parent.left; right: parent.right; top: parent.top; bottom: label.top}
            model: nameModel
            delegate: nameDelegate
            focus: true
            path: Path {
                startX: 150; startY: 200
                PathCubic {
                    x: 50; y: 100
                    control1X: 100; control1Y: 200
                    control2X: 50; control2Y: 125
                }
                PathCubic {
                    x: 150; y: 50
                    control1X: 50; control1Y: 75
                    control2X: 100; control2Y: 50
                }
                PathCubic {
                    x: 250; y: 100
                    control1X: 200; control1Y: 50
                    control2X: 250; control2Y: 75
                }
                PathCubic {
                    x: 150; y: 200
                    control1X: 250; control1Y: 125
                    control2X: 200; control2Y: 200
                }
            }
            Keys.onLeftPressed: decrementCurrentIndex()
            Keys.onRightPressed: incrementCurrentIndex()
        }

        Text {
            id: label
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            text: "<b>" + listView.currentItem.text + "</b> is current"
            font.pixelSize: 32
        }

        /*
        Rectangle {
            width: 150; height: 360
            x: 100; y: 100
            color: "black"

            Rectangle {
                id: redLight
                x: 25; y: 15;
                width: 100; height: 100
                radius: 50
            }
            Rectangle {
                id: yellowLight
                x: 25; y: 130;
                width: 100; height: 100
                radius: 50
            }
            Rectangle {
                id: greenLight
                x: 25; y: 245;
                width: 100; height: 100
                radius: 50
            }
        }
        states: [
            State {
                name: "stopState"
                PropertyChanges { target: redLight; color: "red" }
                PropertyChanges { target: yellowLight; color: "lightGray" }
                PropertyChanges { target: greenLight; color: "lightGray" }
            },
            State {
                name: "waitState"
                PropertyChanges { target: redLight; color: "red" }
                PropertyChanges { target: yellowLight; color: "yellow" }
                PropertyChanges { target: greenLight; color: "lightGray" }
            },
            State {
                name: "driveState"
                PropertyChanges { target: redLight; color: "lightGray" }
                PropertyChanges { target: yellowLight; color: "lightGray" }
                PropertyChanges { target: greenLight; color: "green" }
            },
            State {
                name: "slowState"
                PropertyChanges { target: redLight; color: "lightGray" }
                PropertyChanges { target: yellowLight; color: "yellow" }
                PropertyChanges { target: greenLight; color: "lightGray" }
            }
        ]
        state: "stopState"

        Timer {
            interval: 1000
            repeat: true
            running: true
            onTriggered: {
                var states = ["stopState", "waitState", "driveState", "slowState"]
                var nextIndex = (states.indexOf(parent.state) + 1) % states.length
                parent.state = states[nextIndex]
            }
        }

        transitions: [
            Transition {
                from: "stopState"; to: "waitState"
                PropertyAnimation {
                    targets: [redLight, yellowLight]
                    properties: "color"; duration: 100
                }
            },
            Transition {
                from: "waitState"; to: "driveState"
                PropertyAnimation {
                    targets: [redLight, yellowLight, greenLight]
                    properties: "color"; duration: 100
                }
            },
            Transition {
                from: "driveState"; to: "slowState"
                PropertyAnimation {
                    targets: [greenLight, yellowLight]
                    properties: "color"; duration: 100
                }
            },
            Transition {
                from: "slowState"; to: "stopState"
                PropertyAnimation {
                    targets: [yellowLight, redLight]
                    properties: "color"; duration: 100
                }
            }
        ]
        */

        /*
        Flipable {
            id: flipable
            anchors.centerIn: parent
            property bool flipped: false

            front: Rectangle {
                color: "green"
                width: 100
                height: 100
                anchors.centerIn: parent
            }

            back: Rectangle {
                color: "red"
                width: 100
                height: 100
                anchors.centerIn: parent
            }

            transform: Rotation {
                axis.x: 1; axis.y: 0; axis.z: 0
                angle: flipable.flipped? 270 : 0

                Behavior on angle {
                    NumberAnimation {
                        duration: 500
                    }
                }
            }

            Text {
                text: flipable.side == Flipable.Front ? "Front" : "Back"
                anchors.centerIn: parent
            }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: flipable.flipped = !flipable.flipped
        }
        */

        /*
        Rectangle {
            id: rectGreen
            x: path.startX - width/2; y: path.startY - height/2
            width: 10; height: 10
            color: "green"
            //Behavior on x { SpringAnimation { spring: 1; damping: 0.5 } }
            //Behavior on y { SpringAnimation { spring: 1; damping: 0.5 } }
        }

        PathAnimation {
            id: pathAnim
            target: rectGreen
            duration: 3000
            loops: 10
            orientation: PathAnimation.TopFirst
            anchorPoint: Qt.point(rectGreen.width/2, rectGreen.height/2)
            path: Path {
                id: path
                startX: 100; startY: 300
                PathCubic {
                    id: part1
                    x: 400
                    y: 300
                    control1X: 200; control1Y: 500
                    control2X: 300; control2Y: 100
                }
                PathCubic {
                    id: part2
                    x: 100
                    y: 300
                    control1X: 300; control1Y: 500
                    control2X: 200; control2Y: 100
                }
            }
        }

        Rectangle {
            id: rectPathStart
            color: "blue"
            width: 4; height: 4
            x: path.startX - width/2; y: path.startY - height/2
        }

        Text {
            text: "p.Start(" + path.startX + ", " + path.startY + ")"
            color: "blue"
            x: path.startX - width/2; y: path.startY - height - 2
        }

        Rectangle {
            color: "white"
            width: 4; height: 4
            x: part1.x - width/2; y: part1.y - height/2
        }

        Text {
            text: "p1.pos(" + part1.x + ", " + part1.y + ")"
            color: "white"
            x: part1.x - width/2; y: part1.y - height - 2
        }

        Rectangle {
            color: "green"
            width: 4; height: 4
            x: part1.control1X - width/2; y: part1.control1Y - height/2
        }

        Text {
            text: "p1.C1(" + part1.control1X + ", " + part1.control1Y + ")"
            color: "green"
            x: part1.control1X - width/2; y: part1.control1Y - height - 2
        }

        Rectangle {
            color: "red"
            width: 4; height: 4
            x: part1.control2X - width/2; y: part1.control2Y - height/2
        }

        Text {
            text: "p1.C2(" + part1.control2X + ", " + part1.control2Y + ")"
            color: "red"
            x: part1.control2X - width/2; y: part1.control2Y - height - 2
        }

        PathView {
            anchors.fill: parent
            path: Path {
                startX: path.startX
                startY: path.startY
                pathElements: part1
            }
            model: 100
            delegate: Rectangle {
                width: 2; height: 2
                color: "white"
            }
        }

        Rectangle {
            color: "white"
            width: 4; height: 4
            x: part2.x - width/2; y: part2.y - height/2
        }

        Text {
            text: "p2.pos(" + part2.x + ", " + part2.y + ")"
            color: "white"
            x: part2.x - width/2; y: part2.y - height - 2
        }

        Rectangle {
            color: "green"
            width: 4; height: 4
            x: part2.control1X - width/2; y: part2.control1Y - height/2
        }

        Text {
            text: "p2.C1(" + part2.control1X + ", " + part2.control1Y + ")"
            color: "green"
            x: part2.control1X - width/2; y: part2.control1Y - height - 2
        }

        Rectangle {
            color: "red"
            width: 4; height: 4
            x: part2.control2X - width/2; y: part2.control2Y - height/2
        }

        Text {
            text: "p2.C2(" + part2.control2X + ", " + part2.control2Y + ")"
            color: "red"
            x: part2.control2X - width/2; y: part2.control2Y - height - 2
        }

        PathView {
            anchors.fill: parent
            path: Path {
                startX: part1.x
                startY: part1.y
                pathElements: part2
            }
            model: 100
            delegate: Rectangle {
                width: 2; height: 2
                color: "white"
            }
        }
        */

        /*
        ColorAnimation {
            id: clrAnim
            target: rectGreen
            property: "color"
            from: Qt.rgba(0, 0.5, 0, 1)
            to: Qt.rgba(1, 1, 1, 1)
            duration: 1000
        }

        RotationAnimation {
            id: rotAnim
            target: rectGreen
            property: "rotation"
            from: 45
            to: 315
            direction: RotationAnimation.Shortest
            duration: 1000
        }

        NumberAnimation {
            id: nrAnim
            target: rectGreen
            properties: "x,y"
            to: 150; duration: 1000
            //running: true
        }
        */

        /*
        Component {
            id: errLogWdgt

            ErrLogWdgt {
                width: 200
                height: 20
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    top: parent.top
                    topMargin: 16
                }
            }
        }

        Loader {
            id: loader
            sourceComponent: errLogWdgt
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
            }
        }

        Text {
            id: txt1
            text: "Binding: " + (loader.item ? loader.item.text : "")
            anchors {
                left: parent.left
                right: parent.right
                top: loader.bottom
            }
        }

        Text {
            id: txt2
            anchors {
                left: parent.left
                right: parent.right
                top: txt1.bottom
            }
        }

        Binding {
            target: loader.item
            property: "text"
            value: "Hello World"
        }

        Connections {
            target: loader.item
            onReturnPressed: txt2.text = "Return Pressed: " + text
        }
        */

        /*
        Image {
            id: imgBtnClear
            source: "qrc:/ZS/Button/ButtonDelete24x24.bmp"
            anchors {
                left: errLogWdgt.right
                leftMargin: 4
                verticalCenter: errLogWdgt.verticalCenter
            }
            opacity: errLogWdgt.text === "" ? 0.25 : 1.0
            MouseArea {
                anchors.fill: parent
                onClicked: errLogWdgt.text = ""
            }
        }

        Text {
            id: txtSummary
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: errLogWdgt.bottom
                topMargin: 12
            }
            text: "<b>Summary:</b> " + errLogWdgt.text
        }

        Row {
            id: rowTopButtons
            anchors {
                top: txtSummary.bottom
                horizontalCenter: parent.horizontalCenter
                topMargin: 16
                bottomMargin: 16
            }
            spacing: 20

            PushButton {
                id: pushButton1
                width: 300
                text: "Open"
            }

            PushButton {
                id: pushButton2
                width: 300
                text: "Close"
            }
        }

        Rectangle {
            id: rectContent
            color: "beige"
            anchors {
                top: rowTopButtons.bottom
                bottom: rowBottomButtons.top
                left: parent.left
                right: parent.right
            }

            PushButton {
                id: pushButtonContent
                anchors {
                    left: parent.left
                    verticalCenter: parent.verticalCenter
                }
                height: 100
                text: "Test"
            }

            Rectangle {
                anchors {
                    left: pushButtonContent.right
                    verticalCenter: pushButtonContent.verticalCenter
                }
                height: pushButtonContent.height
                width: 50
                color: "red"
            }
        }

        Row {
            id: rowBottomButtons
            anchors {
                bottom: parent.bottom
                horizontalCenter: parent.horizontalCenter
                topMargin: 16
                bottomMargin: 16
            }
            spacing: 20

            PushButton {
                id: pushButton3
                text: "Ok"
            }

            PushButton {
                id: pushButton4
                text: "Cancel"
            }
        }
        */

        /*
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onClicked: {
                console.debug("-> MouseArea.onClicked")
                pathAnim.running = true
                console.debug("<- MouseArea.onClicked")
            }
            Component.onCompleted: {
                console.debug("-> MouseArea.onCompleted")
                console.debug("   MouseArea.onCompleted.x: " + x)
                console.debug("   MouseArea.onCompleted.y: " + y)
                console.debug("   MouseArea.onCompleted.width: " + width)
                console.debug("   MouseArea.onCompleted.height: " + height)
                console.debug("<- MouseArea.onCompleted")
            }
        }
        */
    }

    footer: Rectangle {
        height: 20
        width: parent.width
        color: "red"
    }
}
