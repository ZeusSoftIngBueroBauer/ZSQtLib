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
import ZSSys 1.0
import ZSSysGUI 1.0
import ZSQuickControls 1.0

Pane {
    property string nameSpace: "ZS::Apps::Test::QuickControls::Qml"
    property string className: "WidgetCentral"
    property string objectName: "theInst"
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

    id: root
    width: 320
    height: 480

    ColumnLayout {
        spacing: 4
        anchors.fill: parent

        ToolBar {
            id: toolBarHeadline
            Layout.fillWidth: true

            RowLayout {
                anchors.fill: parent
                spacing: 10

                TextField {
                    id: edtBranchPath
                    Layout.fillWidth: true
                    text: treeViewWdgt.treeView.model.getTreeEntryAbsoluteNodePath(treeViewWdgt.treeView.currentIndex)
                    readOnly: true
                }
            }
        }

        SplitView {
            orientation: Qt.Horizontal
            Layout.fillWidth: true
            Layout.fillHeight: true

            IdxTreeViewWdgt {
                id: treeViewWdgt
                idxTree: _ZSQuickControls_idxTreeStyles
                Layout.minimumWidth: 50
                columnInternalIdVisible: false
                columnIdxInTreeVisible: false
                columnIdxInParentBranchVisible: false
                columnKeyInTreeVisible: false
                columnKeyInParentBranchVisible: false
                //onCurrentIndexChanged: {
                //    root.trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "treeViewWdgt.onCurrentIndexChanged", treeView.model.modelIdx2Str(currentIndex));
                //    root.trcAdminObj.traceMethodLeave("EnterLeave", "treeViewWdgt.onCurrentIndexChanged");
                //}
            }

            QuickControlsStyleWdgt {
                id: quickControlsStyleWdgt
            }
        }
    }
}
