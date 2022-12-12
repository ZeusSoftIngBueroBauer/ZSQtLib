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

import QtQml 2.15
import QtQuick 2.15
import QtQuick.Controls 1.4 as C1   // TreeView derived from BasicTableView
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import ZSSysGUI 1.0

ColumnLayout {
    readonly property string nameSpace: "ZS::System::GUI::Qml"
    readonly property string className: "IdxTreeWdgt"
    readonly property string objectName: model.objectName

    property var myTrcAdminObj: _ZSSys_trcServer.getTraceAdminObj(
        nameSpace, className, objectName)
    Component.onDestruction: {
        _ZSSys_trcServer.releaseTraceAdminObj(myTrcAdminObj);
    }

    property alias model: treeView.model
    property string viewMode: "NavPanelOnly" // | "NavPanelAndBranchContent"
    //property string sortOrder: treeView.model.sortOrder // "Config" | "Ascending" | "Descending"

    /*
    onSortOrderChanged: {
        myTrcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "onSortOrderChanged", root.sortOrder)
        myTrcAdminObj.traceMethodLeave("EnterLeave", "onSortOrderChanged")
    }
    */

    id: root
    spacing: 4

    ToolBar {
        property string className: root.className + "::" + "ToolBar"
        property var myTrcAdminObj: _ZSSys_trcServer.getTraceAdminObj(
            root.nameSpace, className, root.objectName)
        Component.onDestruction: {
            _ZSSys_trcServer.releaseTraceAdminObj(myTrcAdminObj);
        }

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
                id: btnViewMode
                icon.source: "qrc:/ZS/TreeView/TreeViewViewMode" + root.viewMode + ".png"
                onClicked: {
                    root.viewMode = root.viewMode === "NavPanelOnly" ? "NavPanelAndBranchContent" : "NavPanelOnly"
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
                    myTrcAdminObj.traceObjMethodEnterWithInArgs("EnterLeave", "btnSortOrder", "onClicked", treeView.model.sortOrder)
                    if(treeView.model.sortOrder === "Config") {
                        treeView.model.sortOrder = "Ascending";
                    }
                    else if(treeView.model.sortOrder === "Ascending") {
                        treeView.model.sortOrder = "Descending";
                    }
                    else if(treeView.model.sortOrder === "Descending") {
                        treeView.model.sortOrder = "Config";
                    }
                    myTrcAdminObj.traceObjMethodLeaveWithReturn("EnterLeave", "btnSortOrder", "onClicked", treeView.model.sortOrder)
                }
            }
            TextField {
                id: edtBranchPath
                Layout.fillWidth: true
                text: "Branch Path"
                readOnly: true
                visible: root.viewMode === "NavPanelAndBranchContent"
            }
            Item { // Margin at right side of row layout (if Branch Path is visible)
                Layout.alignment: Qt.AlignLeft
                Layout.minimumWidth: 4
                Layout.minimumHeight: 1
                visible: edtBranchPath.visible
                Rectangle {
                    anchors.fill: parent
                    color: "red"
                }
            }
            Item { // Stretch at right side of row layout (if Branch Path is not visible)
                Layout.alignment: Qt.AlignRight
                Layout.fillWidth: true
                Layout.minimumHeight: 1
                visible: !edtBranchPath.visible
                Rectangle {
                    anchors.fill: parent
                    color: "red"
                }
            }
        }
    }

    C1.TreeView {
        property string className: root.className + "::" + "TreeView"
        property var myTrcAdminObj: _ZSSys_trcServer.getTraceAdminObj(
            root.nameSpace, className, root.objectName)

        Transition {
            id: transitionAdd
            PropertyAction { property: "transformOrigin"; value: Item.TopLeft }
            NumberAnimation { property: "opacity"; from: 0; to: 1.0; duration: 300 }
            NumberAnimation { property: "scale"; from: 0; to: 1.0; duration: 300 }
        }

        Transition {
            id: transitionDisplaced
            // Ensure row is scaled to 1 and got opacity of 1 if immediately displaced after added.
            PropertyAction { properties: "opacity, scale"; value: 1 }
            NumberAnimation { properties: "x, y"; duration: 300 }
        }

        Component.onCompleted: {
            myTrcAdminObj.traceMethodEnter("EnterLeave", "Component.onCompleted")
            if(this.__listView) {
                this.__listView.add = transitionAdd
                this.__listView.displaced = transitionDisplaced
                this.__listView.spacing = 1
            }
            myTrcAdminObj.traceMethodLeave("EnterLeave", "Component.onCompleted")
        }
        Component.onDestruction: {
            _ZSSys_trcServer.releaseTraceAdminObj(myTrcAdminObj);
        }

        onModelChanged: {
            myTrcAdminObj.traceMethodEnter("EnterLeave", "onModelChanged");
            myTrcAdminObj.traceMethod("Debug", "onModelChanged", "Model: " + model.objectName);
            myTrcAdminObj.traceMethodLeave("EnterLeave", "onModelChanged");
        }

        id: treeView
        Layout.fillWidth: true
        Layout.fillHeight: true
        alternatingRowColors: true
        clip: true
        //selectionMode: C1.SelectionMode.ExtendedSelection

        property var fontPixelSize: 0
        property var columnSpacing: 10

        // Need a different name as QML does not allow to override functions.
        function _resizeColumnsToContents() {
            myTrcAdminObj.traceMethodEnter("EnterLeave", "_resizeColumnsToContents");
            /* The column width of the first column depends not only by the maximum
                width of the column but also on the expanded depth because of indentations
                and decoration icons. Not easy to calculate .....
            clmName.width = treeView.model.columnWidthByRole(clmName.role, treeView.fontPixelSize) */
            resizeColumnsToContents(); // This resizes the Name column to a good size
            // The width of the headers is not taken into account.
            clmInternalId.width = treeView.model.columnWidthByRole(clmInternalId.role, treeView.fontPixelSize) + 2*treeView.columnSpacing
            clmIdxInTree.width = treeView.model.columnWidthByRole(clmIdxInTree.role, treeView.fontPixelSize) + 2*treeView.columnSpacing
            clmIdxInParentBranch.width = treeView.model.columnWidthByRole(clmIdxInParentBranch.role, treeView.fontPixelSize) + 2*treeView.columnSpacing
            clmKeyInTree.width = treeView.model.columnWidthByRole(clmKeyInTree.role, treeView.fontPixelSize) + 2*treeView.columnSpacing
            clmKeyInParentBranch.width = treeView.model.columnWidthByRole(clmKeyInParentBranch.role, treeView.fontPixelSize) + 2*treeView.columnSpacing
            myTrcAdminObj.traceMethodLeave("EnterLeave", "_resizeColumnsToContents");
        }

        function expandAll() {
            myTrcAdminObj.traceMethodEnter("EnterLeave", "expandAll");
            expandRecursive(model.index(0,0));
            myTrcAdminObj.traceMethodLeave("EnterLeave", "expandAll");
        }

        function expandRecursive(i_modelIdx) {
            myTrcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "expandRecursive", model.modelIdx2Str(i_modelIdx));
            if(!isExpanded(i_modelIdx)) {
                expand(i_modelIdx)
            }
            for(var iRow = 0; iRow < model.rowCount(i_modelIdx); iRow++) {
                var modelIdx = model.index(iRow, 0, i_modelIdx);
                expandRecursive(modelIdx);
            }
            myTrcAdminObj.traceMethodLeave("EnterLeave", "expandRecursive");
        }

        function collapseAll() {
            myTrcAdminObj.traceMethodEnter("EnterLeave", "collapseAll");
            collapseRecursive(model.index(0,0));
            myTrcAdminObj.traceMethodLeave("EnterLeave", "collapseAll");
        }

        function collapseRecursive(i_modelIdx) {
            myTrcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "collapseRecursive", model.modelIdx2Str(i_modelIdx));
            for(var iRow = 0; iRow < model.rowCount(i_modelIdx); iRow++) {
                var modelIdx = model.index(iRow, 0, i_modelIdx);
                collapseRecursive(modelIdx);
            }
            if(isExpanded(i_modelIdx)) {
                collapse(i_modelIdx)
            }
            myTrcAdminObj.traceMethodLeave("EnterLeave", "collapseRecursive");
        }

        /* When using a delegate "resizeColumnsToContents" does not work anymore.
        C1.TableViewColumn {
            id: clmType
            title: "Name"
            role: "TreeEntryNameDecorated"
            delegate: Item {
                RowLayout {
                    Image {
                        id: imgType
                        Layout.maximumWidth: 16
                        Layout.maximumHeight: 16
                        Layout.minimumWidth: 16
                        source: styleData.value
                        fillMode: Image.PreserveAspectFit
                    }
                    Label {
                        text: "Hello World"
                        Layout.fillWidth: true
                        Layout.minimumWidth: 64
                    }
                }
            }
        }
        */
        C1.TableViewColumn {
            id: clmName
            title: "Name"
            role: "TreeEntryName"
        }
        C1.TableViewColumn {
            id: clmInternalId
            title: "InternalId"
            role: "InternalId"
        }
        C1.TableViewColumn {
            id: clmIdxInTree
            title: "IdxInTree"
            role: "IdxInTree"
        }
        C1.TableViewColumn {
            id: clmIdxInParentBranch
            title: "IdxInParentBranch"
            role: "IdxInParentBranch"
        }
        C1.TableViewColumn {
            id: clmKeyInTree
            title: "KeyInTree"
            role: "KeyInTree"
        }
        C1.TableViewColumn {
            id: clmKeyInParentBranch
            title: "KeyInParentBranch"
            role: "KeyInParentBranch"
        }
    }
}
