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

import QtQuick 2.15                 // TableView derived from Flickable
import QtQuick.Controls 1.4 as C1   // TableView derived from BasicTableView
import ZSSysGUI 1.0

C1.TableView {
    readonly property string nameSpace: "ZS::System::GUI::Qml"
    readonly property string className: "ErrLogTableView"
    readonly property string objectName: model.objectName
    property var myTrcAdminObj: _ZSSys_trcServer.getTraceAdminObj(nameSpace, className, objectName)

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

    property alias errLog: errLogModel.errLog

    property var fontPixelSize: 0
    property var columnSpacing: 10

    id: root
    alternatingRowColors: true
    clip: true
    selectionMode: C1.SelectionMode.ExtendedSelection

    model: ModelErrLog {
        id: errLogModel
    }

    onErrLogChanged: {
        myTrcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "onErrLogChanged", errLog ? errLog.objectName : "null");
        myTrcAdminObj.traceMethodLeave("EnterLeave", "onErrLogChanged");
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
        clmSeverityImage.width = 24 // model.columnWidthByRole(clmSeverityImage.role, fontPixelSize) + 2*columnSpacing
        clmResult.width = model.columnWidthByRole(clmResult.role, fontPixelSize) + 2*columnSpacing
        clmDate.width = model.columnWidthByRole(clmDate.role, fontPixelSize) + 2*columnSpacing
        clmTime.width = model.columnWidthByRole(clmTime.role, fontPixelSize) + 2*columnSpacing
        clmOccurences.width = model.columnWidthByRole(clmOccurences.role, fontPixelSize) + 2*columnSpacing
        clmSource.width = model.columnWidthByRole(clmSource.role, fontPixelSize) + 2*columnSpacing
        clmAddInfo.width = model.columnWidthByRole(clmAddInfo.role, fontPixelSize) + 2*columnSpacing
        myTrcAdminObj.traceMethodLeave("EnterLeave", "_resizeColumnsToContents");
    }

    C1.TableViewColumn {
        id: clmSeverityImage
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
        width: Math.min(600, root.model.columnWidthByRole(clmResult.role, root.fontPixelSize))
    }
    C1.TableViewColumn {
        id: clmDate
        title: "Date"
        role: "Date"
        width: Math.min(600, root.model.columnWidthByRole(clmDate.role, root.fontPixelSize))
    }
    C1.TableViewColumn {
        id: clmTime
        title: "Time"
        role: "Time"
        width: Math.min(600, root.model.columnWidthByRole(clmTime.role, root.fontPixelSize))
    }
    C1.TableViewColumn {
        id: clmOccurences
        title: "Occurences"
        role: "Occurences"
        width: Math.min(600, root.model.columnWidthByRole(clmOccurences.role, root.fontPixelSize))
    }
    C1.TableViewColumn {
        id: clmSource
        title: "Source"
        role: "Source"
        width: Math.min(600, root.model.columnWidthByRole(clmSource.role, root.fontPixelSize))
    }
    C1.TableViewColumn {
        id: clmAddInfo
        title: "AdditionalInfo"
        role: "AddInfo"
        width: root.width - x
    }
}
