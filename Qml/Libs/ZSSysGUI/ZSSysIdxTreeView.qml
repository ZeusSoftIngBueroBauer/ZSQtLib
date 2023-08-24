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

import QtQml.Models 2.15
import QtQuick 2.15
import QtQuick.Controls 1.4 as C1   // TreeView derived from BasicTableView
import QtQuick.Layouts 1.15
import ZSSys 1.0
import ZSSysGUI 1.0

C1.TreeView {
    readonly property string nameSpace: "ZS::System::GUI::Qml"
    readonly property string className: "IdxTreeView"
    readonly property string objectName: model.objectName
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
        if(this.__listView) {
            this.__listView.add = transitionAdd
            this.__listView.displaced = transitionDisplaced
            this.__listView.spacing = 1
        }
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

    property alias idxTree: idxTreeModel.idxTree

    property alias columnNameVisible: clmName.visible
    property alias columnInternalIdVisible: clmInternalId.visible
    property alias columnIdxInTreeVisible: clmIdxInTree.visible
    property alias columnIdxInParentBranchVisible: clmIdxInParentBranch.visible
    property alias columnKeyInTreeVisible: clmKeyInTree.visible
    property alias columnKeyInParentBranchVisible: clmKeyInParentBranch.visible

    property var fontPixelSize: 0
    property var columnSpacing: 10

    id: root
    alternatingRowColors: true
    clip: true

    model: ModelIdxTree {
        id: idxTreeModel
        onSortOrderChanged: {
            root.trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "idxTreeModel.onSortOrderChanged", sortOrder);
            root.trcAdminObj.traceMethodLeave("EnterLeave", "idxTreeModel.onSortOrderChanged");
        }
    }

    onIdxTreeChanged: {
        trcAdminObj.traceMethodEnter("EnterLeave", "onIdxTreeChanged");
        trcAdminObj.traceMethod("Debug", "onIdxTreeChanged", "IdxTree: " + idxTree ? idxTree.objectName : "null");
        trcAdminObj.traceMethodLeave("EnterLeave", "onIdxTreeChanged");
    }

    onModelChanged: {
        trcAdminObj.traceMethodEnter("EnterLeave", "onModelChanged");
        trcAdminObj.traceMethod("Debug", "onModelChanged", "Model: " + model.objectName);
        trcAdminObj.traceMethodLeave("EnterLeave", "onModelChanged");
    }

    onCurrentIndexChanged: {
        trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "onCurrentIndexChanged", model.modelIdx2Str(currentIndex));
        trcAdminObj.traceMethodLeave("EnterLeave", "onCurrentIndexChanged");
    }

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

    // Need a different name as QML does not allow to override functions.
    function _resizeColumnsToContents() {
        trcAdminObj.traceMethodEnter("EnterLeave", "_resizeColumnsToContents");
        /* The column width of the first column depends not only by the maximum
            width of the column but also on the expanded depth because of indentations
            and decoration icons. Not easy to calculate .....
        clmName.width = model.columnWidthByRole(clmName.role, fontPixelSize) */
        resizeColumnsToContents(); // This resizes the Name column to a good size
        // The width of the headers is not taken into account.
        clmInternalId.width = model.columnWidthByRole(clmInternalId.role, fontPixelSize) + 2*columnSpacing
        clmIdxInTree.width = model.columnWidthByRole(clmIdxInTree.role, fontPixelSize) + 2*columnSpacing
        clmIdxInParentBranch.width = model.columnWidthByRole(clmIdxInParentBranch.role, fontPixelSize) + 2*columnSpacing
        clmKeyInTree.width = model.columnWidthByRole(clmKeyInTree.role, fontPixelSize) + 2*columnSpacing
        clmKeyInParentBranch.width = model.columnWidthByRole(clmKeyInParentBranch.role, fontPixelSize) + 2*columnSpacing
        trcAdminObj.traceMethodLeave("EnterLeave", "_resizeColumnsToContents");
    }

    function expandAll() {
        trcAdminObj.traceMethodEnter("EnterLeave", "expandAll");
        expandRecursive(model.index(0,0));
        trcAdminObj.traceMethodLeave("EnterLeave", "expandAll");
    }

    function expandRecursive(i_modelIdx) {
        trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "expandRecursive", model.modelIdx2Str(i_modelIdx));
        if(!isExpanded(i_modelIdx)) {
            expand(i_modelIdx)
        }
        for(var iRow = 0; iRow < model.rowCount(i_modelIdx); iRow++) {
            var modelIdx = model.index(iRow, 0, i_modelIdx);
            expandRecursive(modelIdx);
        }
        trcAdminObj.traceMethodLeave("EnterLeave", "expandRecursive");
    }

    function collapseAll() {
        trcAdminObj.traceMethodEnter("EnterLeave", "collapseAll");
        collapseRecursive(model.index(0,0));
        trcAdminObj.traceMethodLeave("EnterLeave", "collapseAll");
    }

    function collapseRecursive(i_modelIdx) {
        trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "collapseRecursive", model.modelIdx2Str(i_modelIdx));
        for(var iRow = 0; iRow < model.rowCount(i_modelIdx); iRow++) {
            var modelIdx = model.index(iRow, 0, i_modelIdx);
            collapseRecursive(modelIdx);
        }
        if(isExpanded(i_modelIdx)) {
            collapse(i_modelIdx)
        }
        trcAdminObj.traceMethodLeave("EnterLeave", "collapseRecursive");
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
