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

Image {
    property var toolTipText: ""
    property var severityMax: ""
    property var errorsCount: 0
    property var errorsCounts: {"Critical": 0, "Error": 0, "Warning": 0, "Info": 0, "Success": 0}

    property var errLog: null

    id: root

    Layout.maximumHeight: 24
    Layout.alignment: Qt.AlignRight
    source: severityMax === "" ? "qrc:/ZS/Result/ResultSeveritySuccess.png" : "qrc:/ZS/Result/ResultSeverity" + severityMax + ".png"

    function getErrorCount(severity) {
        return errLog.getEntryCount(severity);
    }

    function getToolTip() {
        var toolTip = ""
        if( errorsCount === 0 ) {
            toolTip = "There is no Info, no Warning, no Error and no Critical Error message pending";
        } else  {
            var errorsCountTmp = 0;
            if( errorsCounts["Critical"] > 0 ) {
                toolTip = "There are Critical Errors";
            } else if( errorsCounts["Error"] > 0 ) {
                toolTip = "There are Errors";
            } else if( errorsCounts["Warning"] > 0 ) {
                toolTip = "There are Warnings";
            } else if( errorsCounts["Info"] > 0 ) {
                toolTip = "There are Infos";
            } else if( errorsCounts["Success"] > 0 ) {
                toolTip = "There are Infos";
            }
        }
        toolTip += ".";
        return toolTip;
    }

    function updateErrorsStatus() {
        errorsCount = 0;
        severityMax = "";
        errLog.lock();
        for( var severity in errorsCounts ) {
            errorsCounts[severity] = getErrorCount(severity);
            if( errorsCounts[severity] > 0 && severityMax === "" ) {
                severityMax = severity;
            }
            errorsCount += errorsCounts[severity];
        }
        errLog.unlock();
        toolTipText = getToolTip();
    }

    Component.onCompleted: {
        updateErrorsStatus();
    }

    Connections {
        target: errLog
        function onCountChanged() {
            root.updateErrorsStatus();
        }
        function onEntryChanged() {
            root.updateErrorsStatus();
        }
    }

    Loader {
        id: loaderErrLogDlg
        active: false
        source: "qrc:/ZSSysGUI/ZSSysErrLogDlg.qml"
        onLoaded: {
            item.visible = true
            item.errLog = root.errLog
        }
    }

    Connections {
        target: loaderErrLogDlg.item
        function onVisibilityChanged() {
            if(loaderErrLogDlg.item && !loaderErrLogDlg.item.visible) {
                loaderErrLogDlg.active = false
            }
        }
    }

    ToolTip {
        id: toolTipImgErrors
        text: root.toolTipText
        visible: maImgErrors.containsMouse
    }

    MouseArea {
        id: maImgErrors
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton
        hoverEnabled: true
        onDoubleClicked: {
            loaderErrLogDlg.active = true
        }
    }
}
