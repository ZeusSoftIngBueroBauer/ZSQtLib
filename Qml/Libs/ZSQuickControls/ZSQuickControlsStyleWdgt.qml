/*******************************************************************************

Copyright 2004 - 2023 by ZeusSoft, Ing. Buero Bauer, Germany
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
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3
import ZSQuickControls 1.0
import ZSSys 1.0

ColumnLayout {
    property string nameSpace: "ZS::QuickControls::Qml"
    readonly property string className: "QuickControlsStyleWdgt"
    readonly property string objectName: "windowsStyle"
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
            trcAdminObj = _ZSSys_trcServer.renameTraceAdminObj(trcAdminObj, objectName)
        }
    }

    readonly property var style: _ZSQuickControls_windowsStyle

    id: root

    ToolBar {
        id: toolBarHeadline
        Layout.fillWidth: true

        RowLayout {
            spacing: 10
            anchors.fill: parent

            ComboBox {
                id: cmbCurrentTheme
                model: style.themes()
                property string currentTheme: style.currentTheme
                onCurrentThemeChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "cmbTheme.onCurrentThemeChanged", currentTheme);
                    currentIndex = find(currentTheme)
                    trcAdminObj.traceMethodLeave("EnterLeave", "cmbTheme.onCurrentThemeChanged");
                }
                Component.onCompleted: {
                    trcAdminObj.traceMethodEnter("EnterLeave", "cmbTheme.Component.onCompleted");
                    currentIndex = find(currentTheme)
                    trcAdminObj.traceMethodLeave("EnterLeave", "cmbTheme.Component.onCompleted");
                }
                onCurrentTextChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "cmbTheme.onCurrrentTextChanged", currentText);
                    style.currentTheme = currentText
                    trcAdminObj.traceMethodLeave("EnterLeave", "cmbTheme.onCurrrentTextChanged");
                }
                onCurrentIndexChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "cmbTheme.onCurrentIndexChanged", currentIndex);
                    trcAdminObj.traceMethodLeave("EnterLeave", "cmbTheme.onCurrentIndexChanged");
                }
            }
            Image {
                id: imgCurrentTheme
                source: "qrc:/ZS/QuickControls/Theme" + style.currentTheme + ".png"
            }
            ToolButton {
                id: btnReset
                text: "Reset"
                onClicked: {
                    style.setToDefault(style.currentTheme)
                }
            }
            ToolButton {
                id: btnSave
                text: "Save"
                onClicked: {
                    style.save()
                }
            }
            ToolButton {
                id: btnRecall
                text: "Recall"
                onClicked: {
                    style.recall(style.currentTheme)
                }
            }
            TextField {
                id: edtAbsFilePath
                Layout.fillWidth: true
                text: style.saveRecallAbsoluteFilePath
                readOnly: true
            }
        }
    }

    SplitView {
        orientation: Qt.Horizontal
        Layout.fillWidth: true
        Layout.fillHeight: true

        // Left Pane                     | Right Pane
        // ------------------------------+--------------------------------------------
        // Controls to modify the style. | Controls implemented for the style.
        //                               | When modifying the style by the controls
        //                               | in the left pane the controls in the right
        //                               | pane indicate the style changes.

        QuickControlsStyleSettings {
            id: quickControlsStyleSettings
        }

        QuickControlsStyled {
            id: quickControlsStyled
        }
    }
}
