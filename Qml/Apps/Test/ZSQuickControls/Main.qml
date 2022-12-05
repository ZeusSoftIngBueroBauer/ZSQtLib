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
import QtGraphicalEffects 1.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.settings 1.0

ApplicationWindow {
    property string nameSpace: "ZS::Apps::Test::QuickControls::Qml"
    property string className: "Main::ApplicationWindow"
    property string objectName: "theInst"

    property var myTrcAdminObj: _ZSSys_trcServer.getTraceAdminObj(
        root.nameSpace, root.className, root.objectName);
    Component.onDestruction: {
        _ZSSys_trcServer.releaseTraceAdminObj(root.myTrcAdminObj);
    }

    id: root
    visible: true
    width: 800
    height: 600

    Settings {
        category: root.className
        property alias x: root.x
        property alias y: root.y
        property alias width: root.width
        property alias height: root.height
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            MenuSeparator { }
            Action {
                id: idActionFileQuit
                text: qsTr("&Quit")
                icon.name: "application-exit"
                onTriggered: {
                    Qt.quit();
                }
            }
        }
        Menu {
            title: qsTr("&Debug")
            Action {
                id: idActionDebugErrLog
                text: qsTr("Error Log")
                onTriggered: {
                    idLoaderErrLogDlg.active = true
                }
            }
        }
        Menu {
            title: qsTr("&Help")
            Action {
                text: qsTr("&About")
            }
        }
    }

    header: ToolBar {
        height: 32
        width: parent.width

        Rectangle {
            anchors.fill: parent
            color: "blue"
        }
    }

    background: Rectangle {
        color: "aqua"
        /*
        gradient: Gradient {
            GradientStop { position: 0; color: "#ffffff" }
            GradientStop { position: 1; color: "#000000" }
        }
        */
    }

    WidgetCentral {
        id: idWidgetCentral
        anchors.fill: parent
    }

    Loader {
        id: idLoaderErrLogDlg
        active: false
        source: "qrc:/ZSSysGUI/ZSSysErrLogDlg.qml"
        onLoaded: {
            item.visible = true
        }
    }
    Connections {
        target: idLoaderErrLogDlg.item
        function onVisibilityChanged() {
            if(idLoaderErrLogDlg.item && !idLoaderErrLogDlg.item.visible) {
                idLoaderErrLogDlg.active = false
            }
        }
    }

    footer: ToolBar {
        height: 32
        width: parent.width

        Rectangle {
            anchors.fill: parent
            color: "darkslateblue"

            RowLayout {
                anchors.fill: parent
                spacing: 10

                Image {
                    id: idImgErrors
                    property var toolTipText: ""
                    property var severityMax: ""
                    property var errorsCount: 0
                    property var errorsCounts: {"Critical": 0, "Error": 0, "Warning": 0, "Info": 0, "Success": 0}
                    function getErrorCount(severity) {
                        console.log("-> idImgErrors.getErrorCount(" + severity + ")");
                        var count = _ZSSys_errLog.getEntryCount(severity);
                        console.log("<- idImgErrors.getErrorCount(): " + count);
                        return count;
                    }
                    function getToolTip() {
                        console.log("-> idImgErrors.getToolTip()");
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
                        console.log("<- idImgErrors.getToolTip(): " + toolTip);
                        return toolTip;
                    }
                    function updateErrorsStatus() {
                        console.log("-> idImgErrors.updateErrorsStatus()");
                        errorsCount = 0;
                        severityMax = "";
                        for( var severity in errorsCounts ) {
                            errorsCounts[severity] = getErrorCount(severity);
                            if( errorsCounts[severity] > 0 && severityMax === "" ) {
                                severityMax = severity;
                            }
                            errorsCount += errorsCounts[severity];
                        }
                        toolTipText = getToolTip();
                        console.log("   idImgErrors.updateErrorsStatus: errorsCount: " + errorsCount);
                        console.log("   idImgErrors.updateErrorsStatus: severityMax: " + severityMax);
                        console.log("   idImgErrors.updateErrorsStatus: toolTipText: " + toolTipText);
                        console.log("<- idImgErrors.updateErrorsStatus()");
                    }
                    Component.onCompleted: {
                        console.log("-> idImgErrors.onCompleted()");
                        updateErrorsStatus();
                        console.log("<- idImgErrors.onCompleted()");
                    }
                    Connections {
                        target: _ZSSys_errLog
                        function onCountChanged() {
                            console.log("-> errLog.onCountChanged");
                            idImgErrors.updateErrorsStatus();
                            console.log("<- errLog.onCountChanged");
                        }
                    }
                    Layout.maximumHeight: 24
                    Layout.alignment: Qt.AlignRight
                    source: severityMax === "" ? "qrc:/ZS/Result/ResultSeveritySuccess.png" : "qrc:/ZS/Result/ResultSeverity" + severityMax + ".png"
                    ToolTip {
                        id: idTTImgErrors
                        text: idImgErrors.toolTipText
                        visible: idMAImgErrors.containsMouse
                    }
                    MouseArea {
                        id: idMAImgErrors
                        anchors.fill: parent
                        acceptedButtons: Qt.LeftButton
                        hoverEnabled: true
                        onDoubleClicked: {
                            idLoaderErrLogDlg.active = true
                        }
                    }
                }
            }
        }
    }
}
