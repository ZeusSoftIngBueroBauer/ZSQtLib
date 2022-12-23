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
import QtQuick.Controls 1.4 as C1   // TableView derived from BasicTableView
import ZSSysGUI 1.0

C1.TableView {
    readonly property string nameSpace: "ZS::System::GUI::Qml"
    readonly property string className: "IdxTreeTableViewBranchContent"
    readonly property string objectName: model.objectName
    property var myTrcAdminObj: _ZSSys_trcServer.getTraceAdminObj(
        nameSpace, className, objectName)

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
        myTrcAdminObj.traceMethodEnter("EnterLeave", "Component.onDestruction")
        myTrcAdminObj.traceMethodLeave("EnterLeave", "Component.onDestruction")
        _ZSSys_trcServer.releaseTraceAdminObj(myTrcAdminObj);
    }

    property alias idxTree: idxTreeBranchContentModel.idxTree
    property string keyInTreeOfRootEntry: ""

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
    selectionMode: C1.SelectionMode.ExtendedSelection

    model: ModelIdxTreeBranchContent {
        id: idxTreeBranchContentModel
        onSortOrderChanged: {
            root.myTrcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "idxTreeModel.onSortOrderChanged", sortOrder);
            root.myTrcAdminObj.traceMethodLeave("EnterLeave", "idxTreeModel.onSortOrderChanged");
        }
        onKeyInTreeOfRootEntryChanged: {
            root.myTrcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "idxTreeModel.onKeyInTreeOfRootEntryChanged", keyInTreeOfRootEntry);
            root.myTrcAdminObj.traceMethodLeave("EnterLeave", "idxTreeModel.onKeyInTreeOfRootEntryChanged");
        }
    }

    onIdxTreeChanged: {
        myTrcAdminObj.traceMethodEnter("EnterLeave", "onIdxTreeChanged");
        myTrcAdminObj.traceMethod("Debug", "onIdxTreeChanged", "IdxTree: " + idxTree ? idxTree.objectName : "null");
        myTrcAdminObj.traceMethodLeave("EnterLeave", "onIdxTreeChanged");
    }

    onModelChanged: {
        myTrcAdminObj.traceMethodEnter("EnterLeave", "onModelChanged");
        myTrcAdminObj.traceMethod("Debug", "onModelChanged", "Model: " + model.objectName);
        myTrcAdminObj.traceMethodLeave("EnterLeave", "onModelChanged");
    }

    onKeyInTreeOfRootEntryChanged: {
        myTrcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "onKeyInTreeOfRootEntryChanged", keyInTreeOfRootEntry);
        model.keyInTreeOfRootEntry = keyInTreeOfRootEntry
        myTrcAdminObj.traceMethodLeave("EnterLeave", "onKeyInTreeOfRootEntryChanged");
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
        myTrcAdminObj.traceMethodEnter("EnterLeave", "_resizeColumnsToContents");
        // The width of the headers is not taken into account.
        clmName.width = model.columnWidthByRole(clmName.role, fontPixelSize) + 2*columnSpacing
        clmInternalId.width = model.columnWidthByRole(clmInternalId.role, fontPixelSize) + 2*columnSpacing
        clmIdxInTree.width = model.columnWidthByRole(clmIdxInTree.role, fontPixelSize) + 2*columnSpacing
        clmIdxInParentBranch.width = model.columnWidthByRole(clmIdxInParentBranch.role, fontPixelSize) + 2*columnSpacing
        clmKeyInTree.width = model.columnWidthByRole(clmKeyInTree.role, fontPixelSize) + 2*columnSpacing
        clmKeyInParentBranch.width = model.columnWidthByRole(clmKeyInParentBranch.role, fontPixelSize) + 2*columnSpacing
        myTrcAdminObj.traceMethodLeave("EnterLeave", "_resizeColumnsToContents");
    }

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
