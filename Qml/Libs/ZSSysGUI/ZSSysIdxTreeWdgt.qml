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

    property alias model: treeView.model
    property string viewMode: "NavPanelOnly" // | "NavPanelAndBranchContent"
    property string sortOrder: "Config" // | "Ascending"

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
                id: btnTreeViewResizeRowsAndColumnsToContents
                icon.source: "qrc:/ZS/TreeView/TreeViewResizeToContents.png"
                onClicked: {
                    //treeView.resizeColumnsToContents();
                }
            }
            ToolButton {
                id: btnTreeViewExpandAll
                icon.source: "qrc:/ZS/TreeView/TreeViewExpandAll.png"
                onClicked: {
                    //treeView.expandAll();
                    //treeView.resizeColumnsToContents();
                }
            }
            ToolButton {
                id: btnTreeViewCollapsedAll
                icon.source: "qrc:/ZS/TreeView/TreeViewCollapseAll.png"
                onClicked: {
                    //treeView.collapseAll();
                }
            }
            ToolButton {
                id: btnSortOrder
                icon.source: "qrc:/ZS/TreeView/TreeViewSortOrder" + root.sortOrder + ".png"
                onClicked: {
                    root.sortOrder = root.sortOrder === "Config" ? "Ascending" : "Config"
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
            if(this.__listView) {
                this.__listView.add = transitionAdd
                this.__listView.displaced = transitionDisplaced
                this.__listView.spacing = 1
            }
        }
        Component.onDestruction: {
            _ZSSys_trcServer.releaseTraceAdminObj(myTrcAdminObj);
        }

        id: treeView
        Layout.fillWidth: true
        Layout.fillHeight: true
        alternatingRowColors: true
        clip: true
        selectionMode: C1.SelectionMode.ExtendedSelection

        property var fontPixelSize: 0

        C1.TableViewColumn {
            id: clmResult
            title: "Name"
            role: "Name"
        }
    }
}
