import QtQuick 2.15
import QtGraphicalEffects 1.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.settings 1.0
import ZSSysGUI 1.0

ApplicationWindow {
    property string nameSpace: "ZS::Apps::Products::IpcServer::Qml"
    property string className: "main.ApplicationWindow"
    property string objectName: "theInst"

    property var myTrcAdminObj: _trcServer.getTraceAdminObj(
            root.nameSpace, root.className, root.objectName);
    Component.onDestruction: {
        _trcServer.releaseTraceAdminObj(root.myTrcAdminObj);
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
            Action {
                text: qsTr("&New...")
                icon.source: "qrc:/ZS/Menu/MenuFileNew16x16.bmp"
            }
            Action {
                text: qsTr("&Open...")
                icon.source: "qrc:/ZS/Menu/MenuFileOpen16x16.bmp"
            }
            Action {
                text: qsTr("&Save")
                icon.source: "qrc:/ZS/Menu/MenuFileSave16x16.bmp"
            }
            Action {
                text: qsTr("Save &As...")
                icon.source: "qrc:/ZS/Menu/MenuFileSaveAs16x16.bmp"
            }
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
            title: qsTr("&Edit")
            Action {
                text: qsTr("Cu&t")
            }
            Action {
                text: qsTr("&Copy")
            }
            Action {
                text: qsTr("&Paste")
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

    ErrLogWdgt {
        id: idErrLogWdgt
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
                        var count = _errLog.getEntryCount(severity);
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
                        target: _errLog
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
