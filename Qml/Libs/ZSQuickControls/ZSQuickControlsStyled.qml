/*******************************************************************************

Copyright 2004 - 2023 by ZeusSoft, Ing. Buero Bauer, Germany
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

import QtQml 2.15
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import ZSSys 1.0

Pane {
    property string nameSpace: "ZS::QuickControls::Qml"
    readonly property string className: "QuickControlsStyled"
    readonly property string objectName: "windowsStyle"
    property var trcAdminObj: TrcAdminObj {
        nameSpace: root.nameSpace
        className: root.className
        objectName: root.objectName
    }

    Component.onCompleted: {
        if( typeof(_ZSSys_trcServer) !== "undefined" ) {
            trcAdminObj = _ZSSys_trcServer.getTraceAdminObj(nameSpace, className, objectName)
        }
        trcAdminObj.traceMethodEnter("EnterLeave", "Component.onCompleted")
        trcAdminObj.traceMethodLeave("EnterLeave", "Component.onCompleted")
    }
    Component.onDestruction: {
        trcAdminObj.traceMethodEnter("EnterLeave", "Component.onDestruction")
        trcAdminObj.traceMethodLeave("EnterLeave", "Component.onDestruction")
        if( typeof(_ZSSys_trcServer) !== "undefined" ) {
            _ZSSys_trcServer.releaseTraceAdminObj(trcAdminObj);
        }
    }
    onObjectNameChanged: {
        if( typeof(_ZSSys_trcServer) !== "undefined" ) {
            trcAdminObj = _ZSSys_trcServer.renameTraceAdminObj(trcAdminObj, objectName)
        }
    }

    readonly property var style: _ZSQuickControls_windowsStyle

    id: root

    GridLayout {
        id: mainLayout
        columnSpacing: 20
        rowSpacing: 10
        property int rowLayoutSpacing: 5

        RowLayout {
            spacing: mainLayout.rowLayoutSpacing
            Label {
                text: qsTr("Button:")
            }
            Button {
                id: buttonControl
                text: qsTr("Click Me")
            }
        }
        RowLayout {
            spacing: mainLayout.rowLayoutSpacing
            Label {
                text: qsTr("CheckBox:")
            }
            CheckBox {
                id: checkBoxControl
                text: qsTr("")
                checked: true
            }
        }
        RowLayout {
            spacing: mainLayout.rowLayoutSpacing
            Label {
                text: qsTr("ComboBox:")
            }
            ComboBox {
                id: comboBoxControl
                model: ["item1", "item2", "item3"]
                Component.onCompleted: {
                    currentIndex = find("item2")
                }
            }
        }
        RowLayout {
            spacing: mainLayout.rowLayoutSpacing
            Label {
                text: qsTr("Label:")
            }
            Label {
                id: labelControl
                text: "hello world"
            }
        }
        RowLayout {
            spacing: mainLayout.rowLayoutSpacing
            Label {
                text: qsTr("Pane:")
            }
            Pane {
                id: paneControl
                width: 100
                height: 24
            }
        }
        RowLayout {
            spacing: mainLayout.rowLayoutSpacing
            Label {
                text: qsTr("Switch:")
            }
            Switch {
                id: switchControl
            }
        }
        RowLayout {
            spacing: mainLayout.rowLayoutSpacing
            Label {
                text: qsTr("ToolButton:")
            }
            ToolButton {
                id: toolButtonControl
                text: qsTr("Click Me")
            }
        }
    }
}
