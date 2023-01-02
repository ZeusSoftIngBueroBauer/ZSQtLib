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
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3
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

        Pane {
            ColumnLayout {
                spacing: 4
                anchors.fill: parent

                // BackgroundStartColor
                //----------------------

                RowLayout {
                    spacing: gridLayout.rowLayoutSpacing
                    Label {
                        id: lblBackgroundStartColor
                        text: qsTr("BackgroundStartColor:")
                        Layout.fillWidth: true
                        color: rectBackgroundStartColorOrig.border.color
                        onColorChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "lblBackgroundStartColor.onColorChanged", color);
                            trcAdminObj.traceMethodLeave("EnterLeave", "lblBackgroundStartColor.onColorChanged");
                        }
                    }
                    Rectangle {
                        id: rectBackgroundStartColorOrig
                        width: 100
                        height: 24
                        border.color: style.isLightTheme ? "black" : "white"
                        border.width: 1
                        visible: !backgroundStartColorDialog.visible
                        onVisibleChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectBackgroundStartColorOrig.onVisibleChanged", visible);
                            trcAdminObj.traceMethodLeave("EnterLeave", "rectBackgroundStartColorOrig.onVisibleChanged");
                        }
                        color: style.backgroundStartColor
                        onColorChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectBackgroundStartColorOrig.onColorChanged", color);
                            chkBackgroundStartColorDefault.checked = style.isBackgroundStartColorSetToDefault()
                            trcAdminObj.traceMethodLeave("EnterLeave", "rectBackgroundStartColorOrig.onColorChanged");
                        }
                        MouseArea {
                            id: maBackgroundStartColorOrig
                            anchors.fill: parent
                            acceptedButtons: Qt.LeftButton
                            onClicked: { backgroundStartColorDialog.open(); }
                        }
                    }
                    Rectangle {
                        id: rectBackgroundStartColorCurr
                        width: 100
                        height: 24
                        border.color: style.isLightTheme ? "black" : "white"
                        border.width: 1
                        visible: backgroundStartColorDialog.visible
                        onVisibleChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectBackgroundStartColorCurr.onVisibleChanged", visible);
                            if( visible ) { color = style.backgroundStartColor }
                            trcAdminObj.traceMethodLeave("EnterLeave", "rectBackgroundStartColorCurr.onVisibleChanged");
                        }
                        onColorChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectBackgroundStartColorCurr.onColorChanged", color);
                            trcAdminObj.traceMethodLeave("EnterLeave", "rectBackgroundStartColorCurr.onColorChanged");
                        }
                    }
                    CheckBox {
                        id: chkBackgroundStartColorDefault
                        text: "Default"
                        enabled: !checked
                        checked: style.isBackgroundStartColorSetToDefault()
                        onCheckedChanged: {
                            if( checked ) {
                                style.setBackgroundStartColorToDefault()
                            }
                        }
                    }
                    ColorDialog {
                        id: backgroundStartColorDialog
                        showAlphaChannel: true
                        onVisibleChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "backgroundStartColorDialog.onVisibleChanged", visible);
                            if( visible ) { color = style.backgroundStartColor }
                            trcAdminObj.traceMethodLeave("EnterLeave", "backgroundStartColorDialog.onVisibleChanged");
                        }
                        onAccepted: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "backgroundStartColorDialog.onAccepted", color);
                            style.setBackgroundStartColor(color)
                            trcAdminObj.traceMethodLeave("EnterLeave", "backgroundStartColorDialog.onAccepted");
                        }
                        onCurrentColorChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "backgroundStartColorDialog.onCurrentColorChanged", currentColor);
                            rectBackgroundStartColorCurr.color = currentColor
                            trcAdminObj.traceMethodLeave("EnterLeave", "backgroundStartColorDialog.onCurrentColorChanged");
                        }
                        onColorChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "backgroundStartColorDialog.onColorChanged", color);
                            trcAdminObj.traceMethodLeave("EnterLeave", "backgroundStartColorDialog.onColorChanged");
                        }
                    }
                }

                // BackgroundStopColor
                //--------------------

                RowLayout {
                    spacing: gridLayout.rowLayoutSpacing
                    Label {
                        id: lblBackgroundStopColor
                        text: qsTr("BackgroundStopColor:")
                        Layout.fillWidth: true
                        color: rectBackgroundStopColorOrig.border.color
                        onColorChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "lblBackgroundStopColor.onColorChanged", color);
                            trcAdminObj.traceMethodLeave("EnterLeave", "lblBackgroundStopColor.onColorChanged");
                        }
                    }
                    Rectangle {
                        id: rectBackgroundStopColorOrig
                        width: 100
                        height: 24
                        border.color: style.isLightTheme ? "black" : "white"
                        border.width: 1
                        visible: !backgroundStopColorDialog.visible
                        onVisibleChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectBackgroundStopColorOrig.onVisibleChanged", visible);
                            trcAdminObj.traceMethodLeave("EnterLeave", "rectBackgroundStopColorOrig.onVisibleChanged");
                        }
                        color: style.backgroundStopColor
                        onColorChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectBackgroundStopColorOrig.onColorChanged", color);
                            chkBackgroundStopColorDefault.checked = style.isBackgroundStopColorSetToDefault()
                            trcAdminObj.traceMethodLeave("EnterLeave", "rectBackgroundStopColorOrig.onColorChanged");
                        }
                        MouseArea {
                            id: maBackgroundStopColorOrig
                            anchors.fill: parent
                            acceptedButtons: Qt.LeftButton
                            onClicked: { backgroundStopColorDialog.open(); }
                        }
                    }
                    Rectangle {
                        id: rectBackgroundStopColorCurr
                        width: 100
                        height: 24
                        border.color: style.isLightTheme ? "black" : "white"
                        border.width: 1
                        visible: backgroundStopColorDialog.visible
                        onVisibleChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectBackgroundStopColorCurr.onVisibleChanged", visible);
                            if( visible ) { color = style.backgroundStopColor }
                            trcAdminObj.traceMethodLeave("EnterLeave", "rectBackgroundStopColorCurr.onVisibleChanged");
                        }
                        onColorChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectBackgroundStopColorCurr.onColorChanged", color);
                            trcAdminObj.traceMethodLeave("EnterLeave", "rectBackgroundStopColorCurr.onColorChanged");
                        }
                    }
                    CheckBox {
                        id: chkBackgroundStopColorDefault
                        text: "Default"
                        enabled: !checked
                        checked: style.isBackgroundStopColorSetToDefault()
                        onCheckedChanged: {
                            if( checked ) {
                                style.setBackgroundStopColorToDefault()
                            }
                        }
                    }
                    ColorDialog {
                        id: backgroundStopColorDialog
                        showAlphaChannel: true
                        onVisibleChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "backgroundStopColorDialog.onVisibleChanged", visible);
                            if( visible ) { color = style.backgroundStopColor }
                            trcAdminObj.traceMethodLeave("EnterLeave", "backgroundStopColorDialog.onVisibleChanged");
                        }
                        onAccepted: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "backgroundStopColorDialog.onAccepted", color);
                            style.backgroundStopColor = color
                            trcAdminObj.traceMethodLeave("EnterLeave", "backgroundStopColorDialog.onAccepted");
                        }
                        onCurrentColorChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "backgroundStopColorDialog.onCurrentColorChanged", currentColor);
                            rectBackgroundStopColorCurr.color = currentColor
                            trcAdminObj.traceMethodLeave("EnterLeave", "backgroundStopColorDialog.onCurrentColorChanged");
                        }
                        onColorChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "backgroundStopColorDialog.onColorChanged", color);
                            trcAdminObj.traceMethodLeave("EnterLeave", "backgroundStopColorDialog.onColorChanged");
                        }
                    }
                }

                // ButtonColor
                //------------

                RowLayout {
                    spacing: gridLayout.rowLayoutSpacing
                    Label {
                        id: lblButtonColor
                        text: qsTr("ButtonColor:")
                        Layout.fillWidth: true
                        color: rectButtonColorOrig.border.color
                        onColorChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "lblButtonColor.onColorChanged", color);
                            trcAdminObj.traceMethodLeave("EnterLeave", "lblButtonColor.onColorChanged");
                        }
                    }
                    Rectangle {
                        id: rectButtonColorOrig
                        width: 100
                        height: 24
                        border.color: style.isLightTheme ? "black" : "white"
                        border.width: 1
                        visible: !buttonColorDialog.visible
                        onVisibleChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectButtonColorOrig.onVisibleChanged", visible);
                            trcAdminObj.traceMethodLeave("EnterLeave", "rectButtonColorOrig.onVisibleChanged");
                        }
                        color: style.buttonBaseColor
                        onColorChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectButtonColorOrig.onColorChanged", color);
                            chkButtonColorDefault.checked = style.isButtonColorSetToDefault()
                            trcAdminObj.traceMethodLeave("EnterLeave", "rectButtonColorOrig.onColorChanged");
                        }
                        MouseArea {
                            id: maButtonColorOrig
                            anchors.fill: parent
                            acceptedButtons: Qt.LeftButton
                            onClicked: { buttonColorDialog.open(); }
                        }
                    }
                    Rectangle {
                        id: rectButtonColorCurr
                        width: 100
                        height: 24
                        border.color: style.isLightTheme ? "black" : "white"
                        border.width: 1
                        visible: buttonColorDialog.visible
                        onVisibleChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectButtonColorCurr.onVisibleChanged", visible);
                            if( visible ) { color = style.buttonColor }
                            trcAdminObj.traceMethodLeave("EnterLeave", "rectButtonColorCurr.onVisibleChanged");
                        }
                        onColorChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectButtonColorCurr.onColorChanged", color);
                            trcAdminObj.traceMethodLeave("EnterLeave", "rectButtonColorCurr.onColorChanged");
                        }
                    }
                    CheckBox {
                        id: chkButtonColorDefault
                        text: "Default"
                        enabled: !checked
                        checked: style.isButtonColorSetToDefault()
                        onCheckedChanged: {
                            if( checked ) {
                                style.setButtonColorToDefault()
                            }
                        }
                    }
                    ColorDialog {
                        id: buttonColorDialog
                        showAlphaChannel: true
                        onVisibleChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "buttonColorDialog.onVisibleChanged", visible);
                            if( visible ) { color = style.buttonColor }
                            trcAdminObj.traceMethodLeave("EnterLeave", "buttonColorDialog.onVisibleChanged");
                        }
                        onAccepted: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "buttonColorDialog.onAccepted", color);
                            style.buttonColor = color
                            trcAdminObj.traceMethodLeave("EnterLeave", "buttonColorDialog.onAccepted");
                        }
                        onCurrentColorChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "buttonColorDialog.onCurrentColorChanged", currentColor);
                            rectButtonColorCurr.color = currentColor
                            trcAdminObj.traceMethodLeave("EnterLeave", "buttonColorDialog.onCurrentColorChanged");
                        }
                        onColorChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "buttonColorDialog.onColorChanged", color);
                            trcAdminObj.traceMethodLeave("EnterLeave", "buttonColorDialog.onColorChanged");
                        }
                    }
                }

                // WindowTextColor
                //----------------

                RowLayout {
                    spacing: gridLayout.rowLayoutSpacing
                    Label {
                        id: lblWindowTextColor
                        text: qsTr("WindowTextColor:")
                        Layout.fillWidth: true
                        color: rectWindowTextColorOrig.border.color
                        onColorChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "lblWindowTextColor.onColorChanged", color);
                            trcAdminObj.traceMethodLeave("EnterLeave", "lblWindowTextColor.onColorChanged");
                        }
                    }
                    Rectangle {
                        id: rectWindowTextColorOrig
                        width: 100
                        height: 24
                        border.color: style.isLightTheme ? "black" : "white"
                        border.width: 1
                        visible: !windowTextColorDialog.visible
                        onVisibleChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectWindowTextColorOrig.onVisibleChanged", visible);
                            trcAdminObj.traceMethodLeave("EnterLeave", "rectWindowTextColorOrig.onVisibleChanged");
                        }
                        color: style.windowTextColor
                        onColorChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectWindowTextColorOrig.onColorChanged", color);
                            chkWindowTextColorDefault.checked = style.isWindowTextColorSetToDefault()
                            trcAdminObj.traceMethodLeave("EnterLeave", "rectWindowTextColorOrig.onColorChanged");
                        }
                        MouseArea {
                            id: maWindowTextColorOrig
                            anchors.fill: parent
                            acceptedButtons: Qt.LeftButton
                            onClicked: { windowTextColorDialog.open(); }
                        }
                    }
                    Rectangle {
                        id: rectWindowTextColorCurr
                        width: 100
                        height: 24
                        border.color: style.isLightTheme ? "black" : "white"
                        border.width: 1
                        visible: windowTextColorDialog.visible
                        onVisibleChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectWindowTextColorCurr.onVisibleChanged", visible);
                            if( visible ) { color = style.windowTextColor }
                            trcAdminObj.traceMethodLeave("EnterLeave", "rectWindowTextColorCurr.onVisibleChanged");
                        }
                        onColorChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectWindowTextColorCurr.onColorChanged", color);
                            trcAdminObj.traceMethodLeave("EnterLeave", "rectWindowTextColorCurr.onColorChanged");
                        }
                    }
                    CheckBox {
                        id: chkWindowTextColorDefault
                        text: "Default"
                        enabled: !checked
                        checked: style.isWindowTextColorSetToDefault()
                        onCheckedChanged: {
                            if( checked ) {
                                style.setWindowTextColorToDefault()
                            }
                        }
                    }
                    ColorDialog {
                        id: windowTextColorDialog
                        showAlphaChannel: true
                        onVisibleChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "windowTextColorDialog.onVisibleChanged", visible);
                            if( visible ) { color = style.windowTextColor }
                            trcAdminObj.traceMethodLeave("EnterLeave", "windowTextColorDialog.onVisibleChanged");
                        }
                        onAccepted: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "windowTextColorDialog.onAccepted", color);
                            style.windowTextColor = color
                            trcAdminObj.traceMethodLeave("EnterLeave", "windowTextColorDialog.onAccepted");
                        }
                        onCurrentColorChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "windowTextColorDialog.onCurrentColorChanged", currentColor);
                            rectWindowTextColorCurr.color = currentColor
                            trcAdminObj.traceMethodLeave("EnterLeave", "windowTextColorDialog.onCurrentColorChanged");
                        }
                        onColorChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "windowTextColorDialog.onColorChanged", color);
                            trcAdminObj.traceMethodLeave("EnterLeave", "windowTextColorDialog.onColorChanged");
                        }
                    }
                }

                // LinkColor
                //----------

                RowLayout {
                    spacing: gridLayout.rowLayoutSpacing
                    Label {
                        id: lblLinkColor
                        text: qsTr("LinkColor:")
                        Layout.fillWidth: true
                        color: rectLinkColorOrig.border.color
                        onColorChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "lblLinkColor.onColorChanged", color);
                            trcAdminObj.traceMethodLeave("EnterLeave", "lblLinkColor.onColorChanged");
                        }
                    }
                    Rectangle {
                        id: rectLinkColorOrig
                        width: 100
                        height: 24
                        border.color: style.isLightTheme ? "black" : "white"
                        border.width: 1
                        visible: !linkColorDialog.visible
                        onVisibleChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectLinkColorOrig.onVisibleChanged", visible);
                            trcAdminObj.traceMethodLeave("EnterLeave", "rectLinkColorOrig.onVisibleChanged");
                        }
                        color: style.linkColor
                        onColorChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectLinkColorOrig.onColorChanged", color);
                            chkLinkColorDefault.checked = style.isLinkColorSetToDefault()
                            trcAdminObj.traceMethodLeave("EnterLeave", "rectLinkColorOrig.onColorChanged");
                        }
                        MouseArea {
                            id: maLinkColorOrig
                            anchors.fill: parent
                            acceptedButtons: Qt.LeftButton
                            onClicked: { linkColorDialog.open(); }
                        }
                    }
                    Rectangle {
                        id: rectLinkColorCurr
                        width: 100
                        height: 24
                        border.color: style.isLightTheme ? "black" : "white"
                        border.width: 1
                        visible: linkColorDialog.visible
                        onVisibleChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectLinkColorCurr.onVisibleChanged", visible);
                            if( visible ) { color = style.linkColor }
                            trcAdminObj.traceMethodLeave("EnterLeave", "rectLinkColorCurr.onVisibleChanged");
                        }
                        onColorChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectLinkColorCurr.onColorChanged", color);
                            trcAdminObj.traceMethodLeave("EnterLeave", "rectLinkColorCurr.onColorChanged");
                        }
                    }
                    CheckBox {
                        id: chkLinkColorDefault
                        text: "Default"
                        enabled: !checked
                        checked: style.isLinkColorSetToDefault()
                        onCheckedChanged: {
                            if( checked ) {
                                style.setLinkColorToDefault()
                            }
                        }
                    }
                    ColorDialog {
                        id: linkColorDialog
                        showAlphaChannel: true
                        onVisibleChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "linkColorDialog.onVisibleChanged", visible);
                            if( visible ) { color = style.linkColor }
                            trcAdminObj.traceMethodLeave("EnterLeave", "linkColorDialog.onVisibleChanged");
                        }
                        onAccepted: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "linkColorDialog.onAccepted", color);
                            style.linkColor = color
                            trcAdminObj.traceMethodLeave("EnterLeave", "linkColorDialog.onAccepted");
                        }
                        onCurrentColorChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "linkColorDialog.onCurrentColorChanged", currentColor);
                            rectLinkColorCurr.color = currentColor
                            trcAdminObj.traceMethodLeave("EnterLeave", "linkColorDialog.onCurrentColorChanged");
                        }
                        onColorChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "linkColorDialog.onColorChanged", color);
                            trcAdminObj.traceMethodLeave("EnterLeave", "linkColorDialog.onColorChanged");
                        }
                    }
                }

                // ButtonTextColor
                //----------------

                RowLayout {
                    spacing: gridLayout.rowLayoutSpacing
                    Label {
                        id: lblButtonTextColor
                        text: qsTr("ButtonTextColor:")
                        Layout.fillWidth: true
                        color: rectButtonTextColorOrig.border.color
                        onColorChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "lblButtonTextColor.onColorChanged", color);
                            trcAdminObj.traceMethodLeave("EnterLeave", "lblButtonTextColor.onColorChanged");
                        }
                    }
                    Rectangle {
                        id: rectButtonTextColorOrig
                        width: 100
                        height: 24
                        border.color: style.isLightTheme ? "black" : "white"
                        border.width: 1
                        visible: !buttonTextColorDialog.visible
                        onVisibleChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectButtonTextColorOrig.onVisibleChanged", visible);
                            trcAdminObj.traceMethodLeave("EnterLeave", "rectButtonTextColorOrig.onVisibleChanged");
                        }
                        color: style.buttonTextColor
                        onColorChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectButtonTextColorOrig.onColorChanged", color);
                            chkButtonTextColorDefault.checked = style.isButtonTextColorSetToDefault()
                            trcAdminObj.traceMethodLeave("EnterLeave", "rectButtonTextColorOrig.onColorChanged");
                        }
                        MouseArea {
                            id: maButtonTextColorOrig
                            anchors.fill: parent
                            acceptedButtons: Qt.LeftButton
                            onClicked: { buttonTextColorDialog.open(); }
                        }
                    }
                    Rectangle {
                        id: rectButtonTextColorCurr
                        width: 100
                        height: 24
                        border.color: style.isLightTheme ? "black" : "white"
                        border.width: 1
                        visible: buttonTextColorDialog.visible
                        onVisibleChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectButtonTextColorCurr.onVisibleChanged", visible);
                            if( visible ) { color = style.buttonTextColor }
                            trcAdminObj.traceMethodLeave("EnterLeave", "rectButtonTextColorCurr.onVisibleChanged");
                        }
                        onColorChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectButtonTextColorCurr.onColorChanged", color);
                            trcAdminObj.traceMethodLeave("EnterLeave", "rectButtonTextColorCurr.onColorChanged");
                        }
                    }
                    CheckBox {
                        id: chkButtonTextColorDefault
                        text: "Default"
                        enabled: !checked
                        checked: style.isButtonTextColorSetToDefault()
                        onCheckedChanged: {
                            if( checked ) {
                                style.setButtonTextColorToDefault()
                            }
                        }
                    }
                    ColorDialog {
                        id: buttonTextColorDialog
                        showAlphaChannel: true
                        onVisibleChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "buttonTextColorDialog.onVisibleChanged", visible);
                            if( visible ) { color = style.buttonTextColor }
                            trcAdminObj.traceMethodLeave("EnterLeave", "buttonTextColorDialog.onVisibleChanged");
                        }
                        onAccepted: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "buttonTextColorDialog.onAccepted", color);
                            style.buttonTextColor = color
                            trcAdminObj.traceMethodLeave("EnterLeave", "buttonTextColorDialog.onAccepted");
                        }
                        onCurrentColorChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "buttonTextColorDialog.onCurrentColorChanged", currentColor);
                            rectButtonTextColorCurr.color = currentColor
                            trcAdminObj.traceMethodLeave("EnterLeave", "buttonTextColorDialog.onCurrentColorChanged");
                        }
                        onColorChanged: {
                            trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "buttonTextColorDialog.onColorChanged", color);
                            trcAdminObj.traceMethodLeave("EnterLeave", "buttonTextColorDialog.onColorChanged");
                        }
                    }
                }

                Item { // Stretch at bottom of column layout
                    Layout.fillHeight: true
                }
            }
        }

        // Right Pane
        //-----------

        Pane {
            GridLayout {
                id: gridLayout
                columnSpacing: 20
                rowSpacing: 10
                property int rowLayoutSpacing: 5

                RowLayout {
                    spacing: gridLayout.rowLayoutSpacing
                    Label {
                        text: qsTr("Button:")
                    }
                    Button {
                        id: buttonControl
                        text: qsTr("Click Me")
                    }
                }
                /*
                RowLayout {
                    spacing: gridLayout.rowLayoutSpacing
                    Label {
                        text: qsTr("ButtonPanel:")
                    }
                    ButtonPanel {
                        id: buttonPanel
                        width: 100
                        height: 24
                    }
                } */
                /*
                RowLayout {
                    spacing: gridLayout.rowLayoutSpacing
                    Label {
                        text: qsTr("CheckBox:")
                    }
                    CheckBox {
                        id: checkBoxControl
                        text: qsTr("")
                        checked: true
                    }
                } */
                /*
                RowLayout {
                    spacing: gridLayout.rowLayoutSpacing
                    Label {
                        text: qsTr("ComboBox:")
                    }
                    ComboBox {
                        id: comboBoxControl
                        model: ["item1", "item2", "item3"]
                        Component.onCompleted: {
                            currentIndex = find("item2")
                        }
                    }
                } */
                /*
                RowLayout {
                    spacing: gridLayout.rowLayoutSpacing
                    Label {
                        text: qsTr("Label:")
                    }
                    Label {
                        id: labelControl
                        text: "hello world"
                    }
                } */
                /*
                RowLayout {
                    spacing: gridLayout.rowLayoutSpacing
                    Label {
                        text: qsTr("Pane:")
                    }
                    Pane {
                        id: paneControl
                        width: 100
                        height: 24
                    }
                } */
                /*
                RowLayout {
                    spacing: gridLayout.rowLayoutSpacing
                    Label {
                        text: qsTr("Switch:")
                    }
                    Switch {
                        id: switchControl
                    }
                } */
                /*
                RowLayout {
                    spacing: gridLayout.rowLayoutSpacing
                    Label {
                        text: qsTr("ToolButton:")
                    }
                    ToolButton {
                        id: toolButtonControl
                        text: qsTr("Click Me")
                    }
                } */
            }
        }
    }
}
