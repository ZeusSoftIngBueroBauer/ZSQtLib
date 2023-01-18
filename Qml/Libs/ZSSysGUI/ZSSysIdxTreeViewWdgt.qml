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

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import ZSSys 1.0
import ZSSysGUI 1.0

ColumnLayout {
    readonly property string nameSpace: "ZS::System::GUI::Qml"
    readonly property string className: "IdxTreeViewWdgt"
    readonly property string objectName: idxTree ? idxTree.objectName : "IdxTree"
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
        treeView.model.excludeLeaves = false;
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
            if( trcAdminObj.status === Component.Ready ) {
                trcAdminObj = _ZSSys_trcServer.renameTraceAdminObj(trcAdminObj, objectName)
            }
        }
    }

    property alias idxTree: treeView.idxTree
    property alias treeView: treeView
    property alias currentIndex: treeView.currentIndex

    property alias columnInternalIdVisible: treeView.columnInternalIdVisible
    property alias columnIdxInTreeVisible: treeView.columnIdxInTreeVisible
    property alias columnIdxInParentBranchVisible: treeView.columnIdxInParentBranchVisible
    property alias columnKeyInTreeVisible: treeView.columnKeyInTreeVisible
    property alias columnKeyInParentBranchVisible: treeView.columnKeyInParentBranchVisible

    id: root
    spacing: 4

    onIdxTreeChanged: {
        trcAdminObj.traceMethodEnter("EnterLeave", "onIdxTreeChanged");
        trcAdminObj.traceMethod("Debug", "onIdxTreeChanged", "IdxTree: " + idxTree ? idxTree.objectName : "null");
        trcAdminObj.traceMethodLeave("EnterLeave", "onIdxTreeChanged");
    }

    ToolBar {
        id: toolBarHeadline
        Layout.fillWidth: true

        RowLayout {
            anchors.fill: parent
            spacing: 10

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
                    if(treeView.model.sortOrder === "Config") {
                        treeView.model.sortOrder = "Ascending";
                    } else if(treeView.model.sortOrder === "Ascending") {
                        treeView.model.sortOrder = "Descending";
                    } else {
                        treeView.model.sortOrder = "Config";
                    }
                }
            }
            Item { // Stretch at right side of row layout
                Layout.alignment: Qt.AlignLeft
                Layout.fillWidth: true
            }
        }
    }

    IdxTreeView {
        id: treeView
        Layout.fillWidth: true
        Layout.fillHeight: true
        onCurrentIndexChanged: {
            root.trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "treeView.onCurrentIndexChanged", model.modelIdx2Str(currentIndex));
            root.trcAdminObj.traceMethodLeave("EnterLeave", "treeView.onCurrentIndexChanged");
        }
    }
}
