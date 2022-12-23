/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer, Germany
                         Gewerbepark 28
                         D-83670 Bad Heilbrunn
                         Tel: 0049 8046 9488
                         www.zeussoft.de
                         E-Mail: mailbox@zeussoft.de

--------------------------------------------------------------------------------

Content: This file is part of the ZSQtLib.

This file may be used with no license restrictions for your needs. But it is not
allowed to resell any modules of the ZSQtLib veiling the original developer of
the modules. Therefore the copyright link to ZeusSoft, Ing. Buero Bauer must not
be removed from the header of the source code modules.

ZeusSoft, Ing. Buero Bauer provides the source code as is without any guarantee
that the code is written without faults.

ZeusSoft, Ing. Buero Bauer does not assume any liability for any damages which
may result in using the software modules.

*******************************************************************************/

import QtQuick 2.15
import QtGraphicalEffects 1.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.settings 1.0
import ZSSysGUI 1.0

ApplicationWindow {
    property string nameSpace: "ZS::Apps::Test::QuickControls::Qml"
    property string className: "Main::ApplicationWindow"
    property string objectName: "theInst"

    property var myTrcAdminObj: _ZSSys_trcServer.getTraceAdminObj(
        root.nameSpace, root.className, root.objectName);
    Component.onDestruction: {
        _ZSSys_trcServer.releaseTraceAdminObj(root.myTrcAdminObj);
    }

    id: root
    visible: true
    width: 800
    height: 600

    Settings {
        category: root.className
        property alias x: root.x
        property alias y: root.y
        property alias width: root.width
        property alias height: root.height
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            MenuSeparator { }
            Action {
                id: actionFileQuit
                text: qsTr("&Quit")
                icon.name: "application-exit"
                onTriggered: {
                    Qt.quit();
                }
            }
        }
        Menu {
            title: qsTr("&Debug")
            Action {
                id: actionDebugErrLog
                text: qsTr("Error Log")
                onTriggered: {
                    loaderErrLogDlg.active = true
                }
            }
        }
        Menu {
            title: qsTr("&Help")
            Action {
                text: qsTr("&About")
            }
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

    WidgetCentral {
        id: widgetCentral
        anchors.fill: parent
    }

    Loader {
        id: loaderErrLogDlg
        active: false
        source: "qrc:/ZSSysGUI/ZSSysErrLogDlg.qml"
        onLoaded: {
            item.visible = true
            item.errLog = _ZSSys_errLog
        }
    }

    Connections {
        target: loaderErrLogDlg.item
        function onVisibilityChanged() {
            if(loaderErrLogDlg.item && !loaderErrLogDlg.item.visible) {
                loaderErrLogDlg.active = false
            }
        }
    }

    footer: ToolBar {
        height: 32
        width: parent.width

        Rectangle {
            anchors.fill: parent
            color: "darkslateblue"

            RowLayout {
                anchors.fill: parent
                spacing: 10

                ErrLogIcon {
                    id: errLogIcon
                    errLog: _ZSSys_errLog
                }
            }
        }
    }
}
