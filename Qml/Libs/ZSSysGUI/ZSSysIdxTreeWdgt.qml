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
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import ZSSysGUI 1.0

ColumnLayout {
    readonly property string nameSpace: "ZS::System::GUI::Qml"
    readonly property string className: "IdxTreeWdgt"
    readonly property string objectName: idxTree ? idxTree.objectName : "IdxTree"
    property var myTrcAdminObj: _ZSSys_trcServer.getTraceAdminObj(
        nameSpace, className, objectName)

    Component.onCompleted: {
        myTrcAdminObj.traceMethodEnter("EnterLeave", "Component.onCompleted");
        treeViewWdgt.treeView.model.excludeLeaves = false;
        myTrcAdminObj.traceMethodLeave("EnterLeave", "Component.onCompleted");
    }
    Component.onDestruction: {
        myTrcAdminObj.traceMethodEnter("EnterLeave", "Component.onDestruction")
        myTrcAdminObj.traceMethodLeave("EnterLeave", "Component.onDestruction")
        _ZSSys_trcServer.releaseTraceAdminObj(myTrcAdminObj);
    }

    property var idxTree: null
    property string viewMode: "NavPanelOnly" // | "NavPanelAndBranchContent"

    id: root
    spacing: 4

    onIdxTreeChanged: {
        myTrcAdminObj.traceMethodEnter("EnterLeave", "onIdxTreeChanged");
        myTrcAdminObj.traceMethod("Debug", "onIdxTreeChanged", "IdxTree: " + idxTree ? idxTree.objectName : "null");
        myTrcAdminObj.traceMethodLeave("EnterLeave", "onIdxTreeChanged");
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
                id: btnViewMode
                icon.source: "qrc:/ZS/TreeView/TreeViewViewMode" + root.viewMode + ".png"
                onClicked: {
                    treeViewWdgt.treeView.model.excludeLeaves = !treeViewWdgt.treeView.model.excludeLeaves
                    root.viewMode = treeViewWdgt.treeView.model.excludeLeaves ? "NavPanelAndBranchContent" : "NavPanelOnly"
                }
            }
            TextField {
                id: edtBranchPath
                Layout.fillWidth: true
                Layout.maximumHeight: btnViewMode.height
                text: treeViewWdgt.treeView.model.getTreeEntryAbsoluteNodePath(treeViewWdgt.treeView.currentIndex)
                readOnly: true
            }
            Item { // Margin at right side of row layout
                Layout.alignment: Qt.AlignLeft
                Layout.minimumWidth: 4
                Layout.minimumHeight: 1
                Rectangle {
                    anchors.fill: parent
                    color: "red"
                }
            }
        }
    }

    SplitView {
        orientation: Qt.Horizontal
        Layout.fillWidth: true
        Layout.fillHeight: true

        IdxTreeViewWdgt {
            id: treeViewWdgt
            idxTree: root.idxTree
            Layout.minimumWidth: 50
            Layout.fillWidth: root.viewMode ===  "NavPanelOnly"
            columnInternalIdVisible: root.viewMode ===  "NavPanelOnly"
            columnIdxInTreeVisible: root.viewMode ===  "NavPanelOnly"
            columnIdxInParentBranchVisible: root.viewMode ===  "NavPanelOnly"
            columnKeyInTreeVisible: root.viewMode ===  "NavPanelOnly"
            columnKeyInParentBranchVisible: root.viewMode ===  "NavPanelOnly"
            onCurrentIndexChanged: {
                root.myTrcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "treeViewWdgt.onCurrentIndexChanged", treeView.model.modelIdx2Str(currentIndex));
                root.myTrcAdminObj.traceMethodLeave("EnterLeave", "treeViewWdgt.onCurrentIndexChanged");
            }
        }
        IdxTreeTableViewBranchContentWdgt {
            id: tableViewBranchContentWdgt
            idxTree: root.idxTree
            Layout.fillWidth: true
            visible: root.viewMode === "NavPanelAndBranchContent"
            keyInTreeOfRootEntry: treeViewWdgt.currentIndex ? treeViewWdgt.treeView.model.getTreeEntryKeyInTree(treeViewWdgt.currentIndex) : ""
        }
    }
}
