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
//import QtQuick.Layouts 1.15

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

    id: root

    alternatingRowColors: true
    clip: true
    selectionMode: C1.SelectionMode.ExtendedSelection
    //color: "lightgray"

    property string sortOrder: "Ascending"
    //property var currentIndex: null
    //property var model: null

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

    onModelChanged: {
        myTrcAdminObj.traceMethodEnter("EnterLeave", "onModelChanged");
        myTrcAdminObj.traceMethod("Debug", "onModelChanged", "Model: " + objectName);
        myTrcAdminObj.traceMethodLeave("EnterLeave", "onModelChanged");
    }

    /* onCurrentIndexChanged: {
        myTrcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "treeView.onCurrentIndexChanged", model.modelIdx2Str(currentIndex));
        myTrcAdminObj.traceMethodLeave("EnterLeave", "treeView.onCurrentIndexChanged");
    } */

    C1.TableViewColumn {
        id: clmName
        title: "Name"
        role: "TreeEntryName"
    }
}
