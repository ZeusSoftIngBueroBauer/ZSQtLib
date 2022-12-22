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
    readonly property string className: "IdxTreeViewWdgt"
    readonly property string objectName: treeViewModel.objectName
    property var myTrcAdminObj: _ZSSys_trcServer.getTraceAdminObj(
        nameSpace, className, objectName)

    Component.onCompleted: {
        myTrcAdminObj.traceMethodEnter("EnterLeave", "Component.onCompleted");
        treeViewModel.sortOrder = "Ascending";
        treeViewModel.excludeLeaves = false;
        myTrcAdminObj.traceMethodLeave("EnterLeave", "Component.onCompleted");
    }
    Component.onDestruction: {
        myTrcAdminObj.traceMethodEnter("EnterLeave", "Component.onDestruction")
        myTrcAdminObj.traceMethodLeave("EnterLeave", "Component.onDestruction")
        _ZSSys_trcServer.releaseTraceAdminObj(myTrcAdminObj);
    }

    property alias treeViewModel: treeView.model
    property alias currentIndex: treeView.currentIndex

    property alias columnInternalIdVisible: treeView.columnInternalIdVisible
    property alias columnIdxInTreeVisible: treeView.columnIdxInTreeVisible
    property alias columnIdxInParentBranchVisible: treeView.columnIdxInParentBranchVisible
    property alias columnKeyInTreeVisible: treeView.columnKeyInTreeVisible
    property alias columnKeyInParentBranchVisible: treeView.columnKeyInParentBranchVisible

    id: root
    spacing: 4

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
                    treeView._resizeColumnsToContents();
                }
            }
            ToolButton {
                id: btnTreeViewExpandAll
                icon.source: "qrc:/ZS/TreeView/TreeViewExpandAll.png"
                onClicked: {
                    treeView.expandAll();
                    treeView.resizeColumnsToContents();
                }
            }
            ToolButton {
                id: btnTreeViewCollapsedAll
                icon.source: "qrc:/ZS/TreeView/TreeViewCollapseAll.png"
                onClicked: {
                    treeView.collapseAll();
                    treeView.resizeColumnsToContents();
                }
            }
            ToolButton {
                id: btnSortOrder
                icon.source: "qrc:/ZS/TreeView/TreeViewSortOrder" + treeView.model.sortOrder + ".png"
                onClicked: {
                    treeView.model.sortOrder = treeView.model.sortOrder === "Descending" ? "Ascending" : "Descending"
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

    IdxTreeView {
        id: treeView
        Layout.fillWidth: true
        Layout.fillHeight: true
        onCurrentIndexChanged: {
            root.myTrcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "treeView.onCurrentIndexChanged", model.modelIdx2Str(currentIndex));
            root.myTrcAdminObj.traceMethodLeave("EnterLeave", "treeView.onCurrentIndexChanged");
        }
    }
}
