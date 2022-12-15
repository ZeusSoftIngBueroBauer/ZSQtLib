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
import QtQuick 2.15                 // TableView derived from Flickable
import QtQuick.Controls 1.4 as C1   // TableView derived from BasicTableView
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import ZSSysGUI 1.0

ColumnLayout {
    readonly property string nameSpace: "ZS::System::GUI::Qml"
    readonly property string className: "ErrLogWdgt"
    readonly property string objectName: _ZSSysGUI_errLogModel.objectName
    property var myTrcAdminObj: _ZSSys_trcServer.getTraceAdminObj(
        root.nameSpace, className, root.objectName)
    Component.onDestruction: {
        _ZSSys_trcServer.releaseTraceAdminObj(myTrcAdminObj);
    }

    property alias model: tableView.model

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
                id: btnClearTable
                text: "Clear"
                onClicked: {
                    _ZSSysGUI_errLogModel.clear()
                }
            }
            ToolButton {
                id: btnDeleteRows
                text: "Delete Row(s)"
                onClicked: {
                    var idxList = []
                    tableView.selection.forEach(
                        function(rowIndex) {
                            idxList.push(rowIndex)
                        }
                    )
                    _ZSSysGUI_errLogModel.removeEntries(idxList);
                    tableView.selection.clear();
                }
            }
            ToolButton {
                id: btnResizeRowsAndColumnsToContents
                icon.source: "qrc:/ZS/TreeView/TreeViewResizeToContents.png"
                onClicked: {
                    tableView.resizeColumnsToContents();
                }
            }
            Item { // Space before File Name
                Layout.minimumWidth: 10
                Layout.minimumHeight: 1
                Rectangle {
                    anchors.fill: parent
                    color: "red"
                }
            }
            Text {
                id: lblFileName
                text: "File Name:"
            }
            TextField {
                id: edtFileName
                Layout.fillWidth: true
                text: _ZSSys_errLog.absFilePath
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

    C1.TableView {
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

        id: tableView
        Layout.fillWidth: true
        Layout.fillHeight: true
        alternatingRowColors: true
        clip: true
        selectionMode: C1.SelectionMode.ExtendedSelection

        property var fontPixelSize: 0

        C1.TableViewColumn {
            title: ""
            role: "SeverityImageUrl"
            width: 24
            delegate: Item {
                Image {
                    id: imgSeverity
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.leftMargin: 10
                    anchors.rightMargin: 10
                    width: height
                    height: parent.height
                    source: styleData.value
                    fillMode: Image.PreserveAspectFit
                }
            }
        }
        C1.TableViewColumn {
            id: clmResult
            title: "Result"
            role: "Result"
            width: Math.min(600, tableView.model.columnWidthByRole(clmResult.role, tableView.fontPixelSize))
        }
        C1.TableViewColumn {
            id: clmDate
            title: "Date"
            role: "Date"
            width: Math.min(600, tableView.model.columnWidthByRole(clmDate.role, tableView.fontPixelSize))
        }
        C1.TableViewColumn {
            id: clmTime
            title: "Time"
            role: "Time"
            width: Math.min(600, tableView.model.columnWidthByRole(clmTime.role, tableView.fontPixelSize))
        }
        C1.TableViewColumn {
            id: clmOccurences
            title: "Occurences"
            role: "Occurences"
            width: Math.min(600, tableView.model.columnWidthByRole(clmOccurences.role, tableView.fontPixelSize))
        }
        C1.TableViewColumn {
            id: clmSource
            title: "Source"
            role: "Source"
            width: Math.min(600, tableView.model.columnWidthByRole(clmSource.role, tableView.fontPixelSize))
        }
        C1.TableViewColumn {
            id: clmAddInfo
            title: "AdditionalInfo"
            role: "AddInfo"
            width: tableView.width - x
        }
    }
}
