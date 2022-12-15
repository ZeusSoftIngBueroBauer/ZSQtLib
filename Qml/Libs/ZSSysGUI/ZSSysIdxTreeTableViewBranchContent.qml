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
//import QtQuick.Controls 1.4 as C1   // TableView derived from BasicTableView
//import QtQuick.Layouts 1.15

Rectangle {
    readonly property string nameSpace: "ZS::System::GUI::Qml"
    readonly property string className: "IdxTreeTableViewBranchContent"
    readonly property string objectName: "Branch Name" // model.objectName

    property var myTrcAdminObj: _ZSSys_trcServer.getTraceAdminObj(
        nameSpace, className, objectName)
    Component.onCompleted: {
        myTrcAdminObj.traceMethodEnter("EnterLeave", "Component.onCompleted")
        //if(this.__listView) {
        //    this.__listView.add = transitionAdd
        //    this.__listView.displaced = transitionDisplaced
        //    this.__listView.spacing = 1
        //}
        myTrcAdminObj.traceMethodLeave("EnterLeave", "Component.onCompleted")
    }
    Component.onDestruction: {
        _ZSSys_trcServer.releaseTraceAdminObj(myTrcAdminObj);
    }

    id: root

    property string sortOrder: "Ascending"
    property var currentIndex: null
    property var model: null

    onCurrentIndexChanged: {
        myTrcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "treeView.onCurrentIndexChanged", model.modelIdx2Str(currentIndex));
        myTrcAdminObj.traceMethodLeave("EnterLeave", "treeView.onCurrentIndexChanged");
    }

    color: "lightgray"
}
