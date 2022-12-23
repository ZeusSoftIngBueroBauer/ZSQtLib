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
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import ZSSysGUI 1.0

ColumnLayout {
    readonly property string nameSpace: "ZS::System::GUI::Qml"
    readonly property string className: "IdxTreeTableViewBranchContentWdgt"
    readonly property string objectName: idxTree ? idxTree.objectName : "IdxTree"
    property var myTrcAdminObj: _ZSSys_trcServer.getTraceAdminObj(nameSpace, className, objectName)

    Component.onCompleted: {
        myTrcAdminObj.traceMethodEnter("EnterLeave", "Component.onCompleted");
        myTrcAdminObj.traceMethodLeave("EnterLeave", "Component.onCompleted");
    }
    Component.onDestruction: {
        myTrcAdminObj.traceMethodEnter("EnterLeave", "Component.onDestruction")
        myTrcAdminObj.traceMethodLeave("EnterLeave", "Component.onDestruction")
        _ZSSys_trcServer.releaseTraceAdminObj(myTrcAdminObj);
    }

    property alias idxTree: tableView.idxTree
    property alias tableView: tableView
    property alias keyInTreeOfRootEntry: tableView.keyInTreeOfRootEntry

    property alias columnInternalIdVisible: tableView.columnInternalIdVisible
    property alias columnIdxInTreeVisible: tableView.columnIdxInTreeVisible
    property alias columnIdxInParentBranchVisible: tableView.columnIdxInParentBranchVisible
    property alias columnKeyInTreeVisible: tableView.columnKeyInTreeVisible
    property alias columnKeyInParentBranchVisible: tableView.columnKeyInParentBranchVisible

    id: root
    spacing: 4

    onIdxTreeChanged: {
        myTrcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "onIdxTreeChanged", idxTree ? idxTree.objectName : "null");
        myTrcAdminObj.traceMethodLeave("EnterLeave", "onIdxTreeChanged");
    }

    onKeyInTreeOfRootEntryChanged: {
        myTrcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "onKeyInTreeOfRootEntryChanged", keyInTreeOfRootEntry);
        myTrcAdminObj.traceMethodLeave("EnterLeave", "onKeyInTreeOfRootEntryChanged");
    }

    ToolBar {
        id: toolBarHeadline
        Layout.fillWidth: true

        RowLayout {
            anchors.fill: parent
            spacing: 10

            Item { // Margin at left side of row layout
                Layout.alignment: Qt.AlignLeft
                Layout.minimumWidth: 4
                Layout.minimumHeight: 1
                Rectangle {
                    anchors.fill: parent
                    color: "red"
                }
            }
            ToolButton {
                id: btnTreeViewResizeColumnsToContents
                icon.source: "qrc:/ZS/TreeView/TreeViewResizeToContents.png"
                onClicked: {
                    tableView._resizeColumnsToContents();
                }
            }
            ToolButton {
                id: btnSortOrder
                icon.source: "qrc:/ZS/TreeView/TreeViewSortOrder" + tableView.model.sortOrder + ".png"
                onClicked: {
                    if(tableView.model.sortOrder === "Config") {
                        tableView.model.sortOrder = "Ascending";
                    }
                    else if(tableView.model.sortOrder === "Ascending") {
                        tableView.model.sortOrder = "Descending";
                    }
                    else {
                        tableView.model.sortOrder = "Config";
                    }
                }
            }
            Item { // Margin at right side of row layout
                Layout.alignment: Qt.AlignLeft
                Layout.minimumWidth: 4
                Layout.minimumHeight: 1
                Layout.fillWidth: true
                Rectangle {
                    anchors.fill: parent
                    color: "red"
                }
            }
        }
    }

    IdxTreeTableViewBranchContent {
        id: tableView
        Layout.fillWidth: true
        Layout.fillHeight: true
    }
}
