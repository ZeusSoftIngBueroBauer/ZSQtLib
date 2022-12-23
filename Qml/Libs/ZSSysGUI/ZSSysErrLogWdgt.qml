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
    readonly property string objectName: errLog ? errLog.objectName : "ZSErrLog"
    property var myTrcAdminObj: _ZSSys_trcServer.getTraceAdminObj(nameSpace, className, objectName)

    Component.onCompleted: {
        myTrcAdminObj.traceMethodEnter("EnterLeave", "Component.onCompleted")
        myTrcAdminObj.traceMethodLeave("EnterLeave", "Component.onCompleted")
    }
    Component.onDestruction: {
        myTrcAdminObj.traceMethodEnter("EnterLeave", "Component.onDestruction")
        myTrcAdminObj.traceMethodLeave("EnterLeave", "Component.onDestruction")
        _ZSSys_trcServer.releaseTraceAdminObj(myTrcAdminObj);
    }

    property alias errLog: tableView.errLog

    id: root
    spacing: 4

    onErrLogChanged: {
        myTrcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "onErrLogChanged", errLog ? errLog.objectName : "null");
        myTrcAdminObj.traceMethodLeave("EnterLeave", "onErrLogChanged");
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
                id: btnClearTable
                text: "Clear"
                onClicked: {
                    tableView.model.clear()
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
                    tableView.model.removeEntries(idxList);
                    tableView.selection.clear();
                }
            }
            ToolButton {
                id: btnResizeRowsAndColumnsToContents
                icon.source: "qrc:/ZS/TreeView/TreeViewResizeToContents.png"
                onClicked: {
                    tableView._resizeColumnsToContents();
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
                text: errLog.absFilePath
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

    ErrLogTableView {
        id: tableView
        Layout.fillWidth: true
        Layout.fillHeight: true
    }
}
