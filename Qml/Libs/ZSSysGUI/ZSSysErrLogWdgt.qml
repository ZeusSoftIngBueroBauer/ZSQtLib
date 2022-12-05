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
    property string nameSpace: "ZS::System::GUI::Qml"
    property string className: "ErrLogWdgt"
    property string objectName: _ZSSysGUI_errLogModel.objectName

    id: root
    spacing: 4

    ToolBar {
        property string className: root.className + "::" + "ToolBar"
        property var myTrcAdminObj: _ZSSys_trcServer.getTraceAdminObj(
            root.nameSpace, className, root.objectName)
        Component.onDestruction: {
            _ZSSys_trcServer.releaseTraceAdminObj(myTrcAdminObj);
        }

        id: idToolBarHeadline
        Layout.preferredWidth: parent.width

        RowLayout {
            anchors.fill: parent
            spacing: 10

            ToolButton {
                id: idBtnClearTable
                text: "Clear"
                Layout.minimumWidth: 100
                Layout.maximumHeight: 24
                onClicked: {
                    _ZSSysGUI_errLogModel.clear()
                }
            }
            ToolButton {
                id: idBtnDeleteRows
                text: "Delete Row(s)"
                Layout.minimumWidth: 100
                Layout.maximumHeight: 24
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
                id: idBtnResizeRowsAndColumnsToContents
                icon.source: "qrc:/ZS/TreeView/TreeViewResizeToContents.png"
                Layout.minimumWidth: 16
                Layout.minimumHeight: 16
                Layout.maximumHeight: 24
                onClicked: {
                    tableView.resizeColumnsToContents();
                }
            }
            Item {
                width: 20
            }
            Text {
                id: idLblFileName
                text: "File Name:"
                Layout.maximumHeight: 24
            }
            TextField {
                id: idEdtFileName
                Layout.fillWidth: true
                Layout.maximumHeight: 24
                text: _ZSSys_errLog.absFilePath
                readOnly: true
            }
        }
    }

    C1.TableView {
        property string className: root.className + "::" + "TableView"
        property var myTrcAdminObj: _ZSSys_trcServer.getTraceAdminObj(
            root.nameSpace, className, root.objectName)

        Transition {
            id: idTransitionAdd
            PropertyAction { property: "transformOrigin"; value: Item.TopLeft }
            NumberAnimation { property: "opacity"; from: 0; to: 1.0; duration: 300 }
            NumberAnimation { property: "scale"; from: 0; to: 1.0; duration: 300 }
        }

        Transition {
            id: idTransitionDisplaced
            // Ensure row is scaled to 1 and got opacity of 1 if immediately displaced after added.
            PropertyAction { properties: "opacity, scale"; value: 1 }
            NumberAnimation { properties: "x, y"; duration: 300 }
        }

        Component.onCompleted: {
            if(this.__listView) {
                this.__listView.add = idTransitionAdd
                this.__listView.displaced = idTransitionDisplaced
                this.__listView.spacing = 1
            }
        }
        Component.onDestruction: {
            _ZSSys_trcServer.releaseTraceAdminObj(myTrcAdminObj);
        }

        id: tableView
        model: _ZSSysGUI_errLogModel
        Layout.fillHeight: true
        Layout.preferredWidth: parent.width
        alternatingRowColors: true
        clip: true
        selectionMode: C1.SelectionMode.ExtendedSelection

        property var fontPixelSize: 0
        property var clmMargin: 10

        C1.TableViewColumn {
            title: ""
            role: "SeverityImageUrl"
            width: 24
            delegate: Item {
                Image {
                    id: idSeverityImg
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.leftMargin: tableView.clmMargin
                    anchors.rightMargin: tableView.clmMargin
                    width: height
                    height: parent.height
                    source: styleData.value
                    fillMode: Image.PreserveAspectFit
                }
            }
        }
        C1.TableViewColumn {
            id: idClmResult
            title: "Result"
            role: "Result"
            width: Math.min(600, tableView.model.columnWidthByRole(idClmResult.role, tableView.fontPixelSize))
        }
        C1.TableViewColumn {
            id: idClmDate
            title: "Date"
            role: "Date"
            width: Math.min(600, tableView.model.columnWidthByRole(idClmDate.role, tableView.fontPixelSize))
        }
        C1.TableViewColumn {
            id: idClmTime
            title: "Time"
            role: "Time"
            width: Math.min(600, tableView.model.columnWidthByRole(idClmTime.role, tableView.fontPixelSize))
        }
        C1.TableViewColumn {
            id: idClmOccurences
            title: "Occurences"
            role: "Occurences"
            width: Math.min(600, tableView.model.columnWidthByRole(idClmOccurences.role, tableView.fontPixelSize))
        }
        C1.TableViewColumn {
            id: idClmSource
            title: "Source"
            role: "Source"
            width: Math.min(600, tableView.model.columnWidthByRole(idClmSource.role, tableView.fontPixelSize))
        }
        C1.TableViewColumn {
            id: idClmAddInfo
            title: "AdditionalInfo"
            role: "AddInfo"
            width: tableView.width - x
        }
    }
}
