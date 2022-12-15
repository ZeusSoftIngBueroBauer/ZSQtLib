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

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import ZSSysGUI 1.0

Item {
    property string nameSpace: "ZS::Apps::Test::QuickControls::Qml"
    property string className: "WidgetCentral::Item"
    property string objectName: "theInst"

    property var myTrcAdminObj: _ZSSys_trcServer.getTraceAdminObj(
        root.nameSpace, root.className, root.objectName);
    Component.onDestruction: {
        _ZSSys_trcServer.releaseTraceAdminObj(root.myTrcAdminObj);
    }

    id: root
    width: 320
    height: 480

    GridLayout {
        id: gridLayout
        anchors.fill: parent

        columnSpacing: 5
        rowSpacing: 5
        rows: 4
        columns: 3

        IdxTreeWdgt {
            id: idxTreeWdgtControls
            model: _ZSQuickControls_stylesModel
            Layout.column: 0
            Layout.row: 0
            Layout.rowSpan: 4
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        Label {
            text: qsTr("Button:")
            Layout.column: 1
            Layout.row: 0
        }

        Button {
            id: buttonControl
            text: qsTr("Click Me")
            Layout.column: 2
            Layout.row: 0
        }

        Label {
            text: qsTr("CheckBox:")
            Layout.column: 1
            Layout.row: 1
        }

        CheckBox {
            id: checkBoxControl
            text: qsTr("")
            checked: true
            Layout.column: 2
            Layout.row: 1
        }

        Label {
            text: qsTr("Switch:")
            Layout.column: 1
            Layout.row: 2
        }

        Switch {
            id: switchControl
            Layout.column: 2
            Layout.row: 2
        }

        Label {
            text: qsTr("ToolButton:")
            Layout.column: 1
            Layout.row: 3
        }

        ToolButton {
            id: toolButtonControl
            text: qsTr("Click Me")
            Layout.column: 2
            Layout.row: 3
        }
    }
}
