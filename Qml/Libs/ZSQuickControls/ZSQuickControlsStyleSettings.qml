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
import ZSSys 1.0

Pane {
    property string nameSpace: "ZS::QuickControls::Qml"
    readonly property string className: "QuickControlsStyleSettings"
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

    ColumnLayout {
        id: mainLayout
        spacing: 4
        anchors.fill: parent
        property int rowLayoutSpacing: 5

        // BaseColor
        //----------

        RowLayout {
            spacing: mainLayout.rowLayoutSpacing
            Label {
                id: lblBaseColor
                text: qsTr("BaseColor:")
                Layout.fillWidth: true
                color: rectBaseColorOrig.border.color
                onColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "lblBaseColor.onColorChanged", color);
                    trcAdminObj.traceMethodLeave("EnterLeave", "lblBaseColor.onColorChanged");
                }
            }
            Rectangle {
                id: rectBaseColorOrig
                width: 100
                height: 24
                border.color: style.isLightTheme ? "black" : "white"
                border.width: 1
                visible: !baseColorDialog.visible
                onVisibleChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectBaseColorOrig.onVisibleChanged", visible);
                    trcAdminObj.traceMethodLeave("EnterLeave", "rectBaseColorOrig.onVisibleChanged");
                }
                color: style.baseColor
                onColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectBaseColorOrig.onColorChanged", color);
                    chkBaseColorDefault.checked = style.isBaseColorSetToDefault()
                    trcAdminObj.traceMethodLeave("EnterLeave", "rectBaseColorOrig.onColorChanged");
                }
                MouseArea {
                    id: maBaseColorOrig
                    anchors.fill: parent
                    acceptedButtons: Qt.LeftButton
                    onClicked: { baseColorDialog.open(); }
                }
            }
            Rectangle {
                id: rectBaseColorCurr
                width: 100
                height: 24
                border.color: style.isLightTheme ? "black" : "white"
                border.width: 1
                visible: baseColorDialog.visible
                onVisibleChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectBaseColorCurr.onVisibleChanged", visible);
                    if( visible ) { color = style.baseColor }
                    trcAdminObj.traceMethodLeave("EnterLeave", "rectBaseColorCurr.onVisibleChanged");
                }
                onColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectBaseColorCurr.onColorChanged", color);
                    trcAdminObj.traceMethodLeave("EnterLeave", "rectBaseColorCurr.onColorChanged");
                }
            }
            CheckBox {
                id: chkBaseColorDefault
                text: "Default"
                enabled: !checked
                checked: style.isBaseColorSetToDefault()
                onCheckedChanged: {
                    if( checked ) {
                        style.setBaseColorToDefault()
                    }
                }
            }
            ColorDialog {
                id: baseColorDialog
                showAlphaChannel: true
                onVisibleChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "baseColorDialog.onVisibleChanged", visible);
                    if( visible ) { color = style.baseColor }
                    trcAdminObj.traceMethodLeave("EnterLeave", "baseColorDialog.onVisibleChanged");
                }
                onAccepted: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "baseColorDialog.onAccepted", color);
                    style.setBaseColor(color)
                    trcAdminObj.traceMethodLeave("EnterLeave", "baseColorDialog.onAccepted");
                }
                onCurrentColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "baseColorDialog.onCurrentColorChanged", currentColor);
                    rectBaseColorCurr.color = currentColor
                    trcAdminObj.traceMethodLeave("EnterLeave", "baseColorDialog.onCurrentColorChanged");
                }
                onColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "baseColorDialog.onColorChanged", color);
                    trcAdminObj.traceMethodLeave("EnterLeave", "baseColorDialog.onColorChanged");
                }
            }
        }

        // PopupWindowColor
        //-----------------

        RowLayout {
            spacing: mainLayout.rowLayoutSpacing
            Label {
                id: lblPopupWindowColor
                text: qsTr("PopupWindowColor:")
                Layout.fillWidth: true
                color: rectPopupWindowColorOrig.border.color
                onColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "lblPopupWindowColor.onColorChanged", color);
                    trcAdminObj.traceMethodLeave("EnterLeave", "lblPopupWindowColor.onColorChanged");
                }
            }
            Rectangle {
                id: rectPopupWindowColorOrig
                width: 100
                height: 24
                border.color: style.isLightTheme ? "black" : "white"
                border.width: 1
                visible: !popupWindowColorDialog.visible
                onVisibleChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectPopupWindowColorOrig.onVisibleChanged", visible);
                    trcAdminObj.traceMethodLeave("EnterLeave", "rectPopupWindowColorOrig.onVisibleChanged");
                }
                color: style.popupWindowColor
                onColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectPopupWindowColorOrig.onColorChanged", color);
                    chkPopupWindowColorDefault.checked = style.isPopupWindowColorSetToDefault()
                    trcAdminObj.traceMethodLeave("EnterLeave", "rectPopupWindowColorOrig.onColorChanged");
                }
                MouseArea {
                    id: maPopupWindowColorOrig
                    anchors.fill: parent
                    acceptedButtons: Qt.LeftButton
                    onClicked: { popupWindowColorDialog.open(); }
                }
            }
            Rectangle {
                id: rectPopupWindowColorCurr
                width: 100
                height: 24
                border.color: style.isLightTheme ? "black" : "white"
                border.width: 1
                visible: popupWindowColorDialog.visible
                onVisibleChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectPopupWindowColorCurr.onVisibleChanged", visible);
                    if( visible ) { color = style.popupWindowColor }
                    trcAdminObj.traceMethodLeave("EnterLeave", "rectPopupWindowColorCurr.onVisibleChanged");
                }
                onColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectPopupWindowColorCurr.onColorChanged", color);
                    trcAdminObj.traceMethodLeave("EnterLeave", "rectPopupWindowColorCurr.onColorChanged");
                }
            }
            CheckBox {
                id: chkPopupWindowColorDefault
                text: "Default"
                enabled: !checked
                checked: style.isPopupWindowColorSetToDefault()
                onCheckedChanged: {
                    if( checked ) {
                        style.setPopupWindowColorToDefault()
                    }
                }
            }
            ColorDialog {
                id: popupWindowColorDialog
                showAlphaChannel: true
                onVisibleChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "popupWindowColorDialog.onVisibleChanged", visible);
                    if( visible ) { color = style.popupWindowColor }
                    trcAdminObj.traceMethodLeave("EnterLeave", "popupWindowColorDialog.onVisibleChanged");
                }
                onAccepted: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "popupWindowColorDialog.onAccepted", color);
                    style.setPopupWindowColor(color)
                    trcAdminObj.traceMethodLeave("EnterLeave", "popupWindowColorDialog.onAccepted");
                }
                onCurrentColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "popupWindowColorDialog.onCurrentColorChanged", currentColor);
                    rectPopupWindowColorCurr.color = currentColor
                    trcAdminObj.traceMethodLeave("EnterLeave", "popupWindowColorDialog.onCurrentColorChanged");
                }
                onColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "popupWindowColorDialog.onColorChanged", color);
                    trcAdminObj.traceMethodLeave("EnterLeave", "popupWindowColorDialog.onColorChanged");
                }
            }
        }

        // BackgroundStartColor
        //----------------------

        RowLayout {
            spacing: mainLayout.rowLayoutSpacing
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
            spacing: mainLayout.rowLayoutSpacing
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
            spacing: mainLayout.rowLayoutSpacing
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
                    if( visible ) { color = style.buttonBaseColor }
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
                    if( visible ) { color = style.buttonBaseColor }
                    trcAdminObj.traceMethodLeave("EnterLeave", "buttonColorDialog.onVisibleChanged");
                }
                onAccepted: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "buttonColorDialog.onAccepted", color);
                    style.buttonBaseColor = color
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

        // ShadowColor
        //----------------------

        RowLayout {
            spacing: mainLayout.rowLayoutSpacing
            Label {
                id: lblShadowColor
                text: qsTr("ShadowColor:")
                Layout.fillWidth: true
                color: rectShadowColorOrig.border.color
                onColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "lblShadowColor.onColorChanged", color);
                    trcAdminObj.traceMethodLeave("EnterLeave", "lblShadowColor.onColorChanged");
                }
            }
            Rectangle {
                id: rectShadowColorOrig
                width: 100
                height: 24
                border.color: style.isLightTheme ? "black" : "white"
                border.width: 1
                visible: !shadowColorDialog.visible
                onVisibleChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectShadowColorOrig.onVisibleChanged", visible);
                    trcAdminObj.traceMethodLeave("EnterLeave", "rectShadowColorOrig.onVisibleChanged");
                }
                color: style.shadowColor
                onColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectShadowColorOrig.onColorChanged", color);
                    chkShadowColorDefault.checked = style.isShadowColorSetToDefault()
                    trcAdminObj.traceMethodLeave("EnterLeave", "rectShadowColorOrig.onColorChanged");
                }
                MouseArea {
                    id: maShadowColorOrig
                    anchors.fill: parent
                    acceptedButtons: Qt.LeftButton
                    onClicked: { shadowColorDialog.open(); }
                }
            }
            Rectangle {
                id: rectShadowColorCurr
                width: 100
                height: 24
                border.color: style.isLightTheme ? "black" : "white"
                border.width: 1
                visible: shadowColorDialog.visible
                onVisibleChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectShadowColorCurr.onVisibleChanged", visible);
                    if( visible ) { color = style.shadowColor }
                    trcAdminObj.traceMethodLeave("EnterLeave", "rectShadowColorCurr.onVisibleChanged");
                }
                onColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectShadowColorCurr.onColorChanged", color);
                    trcAdminObj.traceMethodLeave("EnterLeave", "rectShadowColorCurr.onColorChanged");
                }
            }
            CheckBox {
                id: chkShadowColorDefault
                text: "Default"
                enabled: !checked
                checked: style.isShadowColorSetToDefault()
                onCheckedChanged: {
                    if( checked ) {
                        style.setShadowColorToDefault()
                    }
                }
            }
            ColorDialog {
                id: shadowColorDialog
                showAlphaChannel: true
                onVisibleChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "shadowColorDialog.onVisibleChanged", visible);
                    if( visible ) { color = style.shadowColor }
                    trcAdminObj.traceMethodLeave("EnterLeave", "shadowColorDialog.onVisibleChanged");
                }
                onAccepted: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "shadowColorDialog.onAccepted", color);
                    style.setShadowColor(color)
                    trcAdminObj.traceMethodLeave("EnterLeave", "shadowColorDialog.onAccepted");
                }
                onCurrentColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "shadowColorDialog.onCurrentColorChanged", currentColor);
                    rectShadowColorCurr.color = currentColor
                    trcAdminObj.traceMethodLeave("EnterLeave", "shadowColorDialog.onCurrentColorChanged");
                }
                onColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "shadowColorDialog.onColorChanged", color);
                    trcAdminObj.traceMethodLeave("EnterLeave", "shadowColorDialog.onColorChanged");
                }
            }
        }

        // TopShadowColor
        //---------------

        RowLayout {
            spacing: mainLayout.rowLayoutSpacing
            Label {
                id: lblTopShadowColor
                text: qsTr("TopShadowColor:")
                Layout.fillWidth: true
                color: rectTopShadowColorOrig.border.color
                onColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "lblTopShadowColor.onColorChanged", color);
                    trcAdminObj.traceMethodLeave("EnterLeave", "lblTopShadowColor.onColorChanged");
                }
            }
            Rectangle {
                id: rectTopShadowColorOrig
                width: 100
                height: 24
                border.color: style.isLightTheme ? "black" : "white"
                border.width: 1
                visible: !topShadowColorDialog.visible
                onVisibleChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectTopShadowColorOrig.onVisibleChanged", visible);
                    trcAdminObj.traceMethodLeave("EnterLeave", "rectTopShadowColorOrig.onVisibleChanged");
                }
                color: style.topShadowColor
                onColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectTopShadowColorOrig.onColorChanged", color);
                    chkTopShadowColorDefault.checked = style.isTopShadowColorSetToDefault()
                    trcAdminObj.traceMethodLeave("EnterLeave", "rectTopShadowColorOrig.onColorChanged");
                }
                MouseArea {
                    id: maTopShadowColorOrig
                    anchors.fill: parent
                    acceptedButtons: Qt.LeftButton
                    onClicked: { topShadowColorDialog.open(); }
                }
            }
            Rectangle {
                id: rectTopShadowColorCurr
                width: 100
                height: 24
                border.color: style.isLightTheme ? "black" : "white"
                border.width: 1
                visible: topShadowColorDialog.visible
                onVisibleChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectTopShadowColorCurr.onVisibleChanged", visible);
                    if( visible ) { color = style.topShadowColor }
                    trcAdminObj.traceMethodLeave("EnterLeave", "rectTopShadowColorCurr.onVisibleChanged");
                }
                onColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectTopShadowColorCurr.onColorChanged", color);
                    trcAdminObj.traceMethodLeave("EnterLeave", "rectTopShadowColorCurr.onColorChanged");
                }
            }
            CheckBox {
                id: chkTopShadowColorDefault
                text: "Default"
                enabled: !checked
                checked: style.isTopShadowColorSetToDefault()
                onCheckedChanged: {
                    if( checked ) {
                        style.setTopShadowColorToDefault()
                    }
                }
            }
            ColorDialog {
                id: topShadowColorDialog
                showAlphaChannel: true
                onVisibleChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "topShadowColorDialog.onVisibleChanged", visible);
                    if( visible ) { color = style.topShadowColor }
                    trcAdminObj.traceMethodLeave("EnterLeave", "topShadowColorDialog.onVisibleChanged");
                }
                onAccepted: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "topShadowColorDialog.onAccepted", color);
                    style.topShadowColor = color
                    trcAdminObj.traceMethodLeave("EnterLeave", "topShadowColorDialog.onAccepted");
                }
                onCurrentColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "topShadowColorDialog.onCurrentColorChanged", currentColor);
                    rectTopShadowColorCurr.color = currentColor
                    trcAdminObj.traceMethodLeave("EnterLeave", "topShadowColorDialog.onCurrentColorChanged");
                }
                onColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "topShadowColorDialog.onColorChanged", color);
                    trcAdminObj.traceMethodLeave("EnterLeave", "topShadowColorDialog.onColorChanged");
                }
            }
        }

        // InnerContrastLineColor
        //-----------------------

        RowLayout {
            spacing: mainLayout.rowLayoutSpacing
            Label {
                id: lblInnerContrastLineColor
                text: qsTr("InnerContrastLineColor:")
                Layout.fillWidth: true
                color: rectInnerContrastLineColorOrig.border.color
                onColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "lblInnerContrastLineColor.onColorChanged", color);
                    trcAdminObj.traceMethodLeave("EnterLeave", "lblInnerContrastLineColor.onColorChanged");
                }
            }
            Rectangle {
                id: rectInnerContrastLineColorOrig
                width: 100
                height: 24
                border.color: style.isLightTheme ? "black" : "white"
                border.width: 1
                visible: !innerContrastLineColorDialog.visible
                onVisibleChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectInnerContrastLineColorOrig.onVisibleChanged", visible);
                    trcAdminObj.traceMethodLeave("EnterLeave", "rectInnerContrastLineColorOrig.onVisibleChanged");
                }
                color: style.innerContrastLineColor
                onColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectInnerContrastLineColorOrig.onColorChanged", color);
                    chkInnerContrastLineColorDefault.checked = style.isInnerContrastLineColorSetToDefault()
                    trcAdminObj.traceMethodLeave("EnterLeave", "rectInnerContrastLineColorOrig.onColorChanged");
                }
                MouseArea {
                    id: maInnerContrastLineColorOrig
                    anchors.fill: parent
                    acceptedButtons: Qt.LeftButton
                    onClicked: { innerContrastLineColorDialog.open(); }
                }
            }
            Rectangle {
                id: rectInnerContrastLineColorCurr
                width: 100
                height: 24
                border.color: style.isLightTheme ? "black" : "white"
                border.width: 1
                visible: innerContrastLineColorDialog.visible
                onVisibleChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectInnerContrastLineColorCurr.onVisibleChanged", visible);
                    if( visible ) { color = style.innerContrastLineColor }
                    trcAdminObj.traceMethodLeave("EnterLeave", "rectInnerContrastLineColorCurr.onVisibleChanged");
                }
                onColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectInnerContrastLineColorCurr.onColorChanged", color);
                    trcAdminObj.traceMethodLeave("EnterLeave", "rectInnerContrastLineColorCurr.onColorChanged");
                }
            }
            CheckBox {
                id: chkInnerContrastLineColorDefault
                text: "Default"
                enabled: !checked
                checked: style.isInnerContrastLineColorSetToDefault()
                onCheckedChanged: {
                    if( checked ) {
                        style.setInnerContrastLineColorToDefault()
                    }
                }
            }
            ColorDialog {
                id: innerContrastLineColorDialog
                showAlphaChannel: true
                onVisibleChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "innerContrastLineColorDialog.onVisibleChanged", visible);
                    if( visible ) { color = style.innerContrastLineColor }
                    trcAdminObj.traceMethodLeave("EnterLeave", "innerContrastLineColorDialog.onVisibleChanged");
                }
                onAccepted: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "innerContrastLineColorDialog.onAccepted", color);
                    style.innerContrastLineColor = color
                    trcAdminObj.traceMethodLeave("EnterLeave", "innerContrastLineColorDialog.onAccepted");
                }
                onCurrentColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "innerContrastLineColorDialog.onCurrentColorChanged", currentColor);
                    rectInnerContrastLineColorCurr.color = currentColor
                    trcAdminObj.traceMethodLeave("EnterLeave", "innerContrastLineColorDialog.onCurrentColorChanged");
                }
                onColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "innerContrastLineColorDialog.onColorChanged", color);
                    trcAdminObj.traceMethodLeave("EnterLeave", "innerContrastLineColorDialog.onColorChanged");
                }
            }
        }

        // HighlightColor
        //---------------

        RowLayout {
            spacing: mainLayout.rowLayoutSpacing
            Label {
                id: lblHighlightColor
                text: qsTr("HighlightColor:")
                Layout.fillWidth: true
                color: rectHighlightColorOrig.border.color
                onColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "lblHighlightColor.onColorChanged", color);
                    trcAdminObj.traceMethodLeave("EnterLeave", "lblHighlightColor.onColorChanged");
                }
            }
            Rectangle {
                id: rectHighlightColorOrig
                width: 100
                height: 24
                border.color: style.isLightTheme ? "black" : "white"
                border.width: 1
                visible: !highlightColorDialog.visible
                onVisibleChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectHighlightColorOrig.onVisibleChanged", visible);
                    trcAdminObj.traceMethodLeave("EnterLeave", "rectHighlightColorOrig.onVisibleChanged");
                }
                color: style.highlightColor
                onColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectHighlightColorOrig.onColorChanged", color);
                    chkHighlightColorDefault.checked = style.isHighlightColorSetToDefault()
                    trcAdminObj.traceMethodLeave("EnterLeave", "rectHighlightColorOrig.onColorChanged");
                }
                MouseArea {
                    id: maHighlightColorOrig
                    anchors.fill: parent
                    acceptedButtons: Qt.LeftButton
                    onClicked: { highlightColorDialog.open(); }
                }
            }
            Rectangle {
                id: rectHighlightColorCurr
                width: 100
                height: 24
                border.color: style.isLightTheme ? "black" : "white"
                border.width: 1
                visible: highlightColorDialog.visible
                onVisibleChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectHighlightColorCurr.onVisibleChanged", visible);
                    if( visible ) { color = style.highlightColor }
                    trcAdminObj.traceMethodLeave("EnterLeave", "rectHighlightColorCurr.onVisibleChanged");
                }
                onColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectHighlightColorCurr.onColorChanged", color);
                    trcAdminObj.traceMethodLeave("EnterLeave", "rectHighlightColorCurr.onColorChanged");
                }
            }
            CheckBox {
                id: chkHighlightColorDefault
                text: "Default"
                enabled: !checked
                checked: style.isHighlightColorSetToDefault()
                onCheckedChanged: {
                    if( checked ) {
                        style.setHighlightColorToDefault()
                    }
                }
            }
            ColorDialog {
                id: highlightColorDialog
                showAlphaChannel: true
                onVisibleChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "highlightColorDialog.onVisibleChanged", visible);
                    if( visible ) { color = style.highlightColor }
                    trcAdminObj.traceMethodLeave("EnterLeave", "highlightColorDialog.onVisibleChanged");
                }
                onAccepted: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "highlightColorDialog.onAccepted", color);
                    style.highlightColor = color
                    trcAdminObj.traceMethodLeave("EnterLeave", "highlightColorDialog.onAccepted");
                }
                onCurrentColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "highlightColorDialog.onCurrentColorChanged", currentColor);
                    rectHighlightColorCurr.color = currentColor
                    trcAdminObj.traceMethodLeave("EnterLeave", "highlightColorDialog.onCurrentColorChanged");
                }
                onColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "highlightColorDialog.onColorChanged", color);
                    trcAdminObj.traceMethodLeave("EnterLeave", "highlightColorDialog.onColorChanged");
                }
            }
        }

        // HighlightedTextColor
        //--------------------

        RowLayout {
            spacing: mainLayout.rowLayoutSpacing
            Label {
                id: lblHighlightedTextColor
                text: qsTr("HighlightedTextColor:")
                Layout.fillWidth: true
                color: rectHighlightedTextColorOrig.border.color
                onColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "lblHighlightedTextColor.onColorChanged", color);
                    trcAdminObj.traceMethodLeave("EnterLeave", "lblHighlightedTextColor.onColorChanged");
                }
            }
            Rectangle {
                id: rectHighlightedTextColorOrig
                width: 100
                height: 24
                border.color: style.isLightTheme ? "black" : "white"
                border.width: 1
                visible: !highlightedTextColorDialog.visible
                onVisibleChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectHighlightedTextColorOrig.onVisibleChanged", visible);
                    trcAdminObj.traceMethodLeave("EnterLeave", "rectHighlightedTextColorOrig.onVisibleChanged");
                }
                color: style.highlightedTextColor
                onColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectHighlightedTextColorOrig.onColorChanged", color);
                    chkHighlightedTextColorDefault.checked = style.isHighlightedTextColorSetToDefault()
                    trcAdminObj.traceMethodLeave("EnterLeave", "rectHighlightedTextColorOrig.onColorChanged");
                }
                MouseArea {
                    id: maHighlightedTextColorOrig
                    anchors.fill: parent
                    acceptedButtons: Qt.LeftButton
                    onClicked: { highlightedTextColorDialog.open(); }
                }
            }
            Rectangle {
                id: rectHighlightedTextColorCurr
                width: 100
                height: 24
                border.color: style.isLightTheme ? "black" : "white"
                border.width: 1
                visible: highlightedTextColorDialog.visible
                onVisibleChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectHighlightedTextColorCurr.onVisibleChanged", visible);
                    if( visible ) { color = style.highlightedTextColor }
                    trcAdminObj.traceMethodLeave("EnterLeave", "rectHighlightedTextColorCurr.onVisibleChanged");
                }
                onColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectHighlightedTextColorCurr.onColorChanged", color);
                    trcAdminObj.traceMethodLeave("EnterLeave", "rectHighlightedTextColorCurr.onColorChanged");
                }
            }
            CheckBox {
                id: chkHighlightedTextColorDefault
                text: "Default"
                enabled: !checked
                checked: style.isHighlightedTextColorSetToDefault()
                onCheckedChanged: {
                    if( checked ) {
                        style.setHighlightedTextColorToDefault()
                    }
                }
            }
            ColorDialog {
                id: highlightedTextColorDialog
                showAlphaChannel: true
                onVisibleChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "highlightedTextColorDialog.onVisibleChanged", visible);
                    if( visible ) { color = style.highlightedTextColor }
                    trcAdminObj.traceMethodLeave("EnterLeave", "highlightedTextColorDialog.onVisibleChanged");
                }
                onAccepted: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "highlightedTextColorDialog.onAccepted", color);
                    style.highlightedTextColor = color
                    trcAdminObj.traceMethodLeave("EnterLeave", "highlightedTextColorDialog.onAccepted");
                }
                onCurrentColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "highlightedTextColorDialog.onCurrentColorChanged", currentColor);
                    rectHighlightedTextColorCurr.color = currentColor
                    trcAdminObj.traceMethodLeave("EnterLeave", "highlightedTextColorDialog.onCurrentColorChanged");
                }
                onColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "highlightedTextColorDialog.onColorChanged", color);
                    trcAdminObj.traceMethodLeave("EnterLeave", "highlightedTextColorDialog.onColorChanged");
                }
            }
        }

        // OutlineColor
        //-------------

        RowLayout {
            spacing: mainLayout.rowLayoutSpacing
            Label {
                id: lblOutlineColor
                text: qsTr("OutlineColor:")
                Layout.fillWidth: true
                color: rectOutlineColorOrig.border.color
                onColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "lblOutlineColor.onColorChanged", color);
                    trcAdminObj.traceMethodLeave("EnterLeave", "lblOutlineColor.onColorChanged");
                }
            }
            Rectangle {
                id: rectOutlineColorOrig
                width: 100
                height: 24
                border.color: style.isLightTheme ? "black" : "white"
                border.width: 1
                visible: !outlineColorDialog.visible
                onVisibleChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectOutlineColorOrig.onVisibleChanged", visible);
                    trcAdminObj.traceMethodLeave("EnterLeave", "rectOutlineColorOrig.onVisibleChanged");
                }
                color: style.outlineColor
                onColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectOutlineColorOrig.onColorChanged", color);
                    chkOutlineColorDefault.checked = style.isOutlineColorSetToDefault()
                    trcAdminObj.traceMethodLeave("EnterLeave", "rectOutlineColorOrig.onColorChanged");
                }
                MouseArea {
                    id: maOutlineColorOrig
                    anchors.fill: parent
                    acceptedButtons: Qt.LeftButton
                    onClicked: { outlineColorDialog.open(); }
                }
            }
            Rectangle {
                id: rectOutlineColorCurr
                width: 100
                height: 24
                border.color: style.isLightTheme ? "black" : "white"
                border.width: 1
                visible: outlineColorDialog.visible
                onVisibleChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectOutlineColorCurr.onVisibleChanged", visible);
                    if( visible ) { color = style.outlineColor }
                    trcAdminObj.traceMethodLeave("EnterLeave", "rectOutlineColorCurr.onVisibleChanged");
                }
                onColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectOutlineColorCurr.onColorChanged", color);
                    trcAdminObj.traceMethodLeave("EnterLeave", "rectOutlineColorCurr.onColorChanged");
                }
            }
            CheckBox {
                id: chkOutlineColorDefault
                text: "Default"
                enabled: !checked
                checked: style.isOutlineColorSetToDefault()
                onCheckedChanged: {
                    if( checked ) {
                        style.setOutlineColorToDefault()
                    }
                }
            }
            ColorDialog {
                id: outlineColorDialog
                showAlphaChannel: true
                onVisibleChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "outlineColorDialog.onVisibleChanged", visible);
                    if( visible ) { color = style.outlineColor }
                    trcAdminObj.traceMethodLeave("EnterLeave", "outlineColorDialog.onVisibleChanged");
                }
                onAccepted: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "outlineColorDialog.onAccepted", color);
                    style.outlineColor = color
                    trcAdminObj.traceMethodLeave("EnterLeave", "outlineColorDialog.onAccepted");
                }
                onCurrentColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "outlineColorDialog.onCurrentColorChanged", currentColor);
                    rectOutlineColorCurr.color = currentColor
                    trcAdminObj.traceMethodLeave("EnterLeave", "outlineColorDialog.onCurrentColorChanged");
                }
                onColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "outlineColorDialog.onColorChanged", color);
                    trcAdminObj.traceMethodLeave("EnterLeave", "outlineColorDialog.onColorChanged");
                }
            }
        }

        // HighlightedOutlineColor
        //------------------------

        RowLayout {
            spacing: mainLayout.rowLayoutSpacing
            Label {
                id: lblHighlightedOutlineColor
                text: qsTr("HighlightedOutlineColor:")
                Layout.fillWidth: true
                color: rectHighlightedOutlineColorOrig.border.color
                onColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "lblHighlightedOutlineColor.onColorChanged", color);
                    trcAdminObj.traceMethodLeave("EnterLeave", "lblHighlightedOutlineColor.onColorChanged");
                }
            }
            Rectangle {
                id: rectHighlightedOutlineColorOrig
                width: 100
                height: 24
                border.color: style.isLightTheme ? "black" : "white"
                border.width: 1
                visible: !highlightedOutlineColorDialog.visible
                onVisibleChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectHighlightedOutlineColorOrig.onVisibleChanged", visible);
                    trcAdminObj.traceMethodLeave("EnterLeave", "rectHighlightedOutlineColorOrig.onVisibleChanged");
                }
                color: style.highlightedOutlineColor
                onColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectHighlightedOutlineColorOrig.onColorChanged", color);
                    chkHighlightedOutlineColorDefault.checked = style.isHighlightedOutlineColorSetToDefault()
                    trcAdminObj.traceMethodLeave("EnterLeave", "rectHighlightedOutlineColorOrig.onColorChanged");
                }
                MouseArea {
                    id: maHighlightedOutlineColorOrig
                    anchors.fill: parent
                    acceptedButtons: Qt.LeftButton
                    onClicked: { highlightedOutlineColorDialog.open(); }
                }
            }
            Rectangle {
                id: rectHighlightedOutlineColorCurr
                width: 100
                height: 24
                border.color: style.isLightTheme ? "black" : "white"
                border.width: 1
                visible: highlightedOutlineColorDialog.visible
                onVisibleChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectHighlightedOutlineColorCurr.onVisibleChanged", visible);
                    if( visible ) { color = style.highlightedOutlineColor }
                    trcAdminObj.traceMethodLeave("EnterLeave", "rectHighlightedOutlineColorCurr.onVisibleChanged");
                }
                onColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectHighlightedOutlineColorCurr.onColorChanged", color);
                    trcAdminObj.traceMethodLeave("EnterLeave", "rectHighlightedOutlineColorCurr.onColorChanged");
                }
            }
            CheckBox {
                id: chkHighlightedOutlineColorDefault
                text: "Default"
                enabled: !checked
                checked: style.isHighlightedOutlineColorSetToDefault()
                onCheckedChanged: {
                    if( checked ) {
                        style.setHighlightedOutlineColorToDefault()
                    }
                }
            }
            ColorDialog {
                id: highlightedOutlineColorDialog
                showAlphaChannel: true
                onVisibleChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "highlightedOutlineColorDialog.onVisibleChanged", visible);
                    if( visible ) { color = style.highlightedOutlineColor }
                    trcAdminObj.traceMethodLeave("EnterLeave", "highlightedOutlineColorDialog.onVisibleChanged");
                }
                onAccepted: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "highlightedOutlineColorDialog.onAccepted", color);
                    style.highlightedOutlineColor = color
                    trcAdminObj.traceMethodLeave("EnterLeave", "highlightedOutlineColorDialog.onAccepted");
                }
                onCurrentColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "highlightedOutlineColorDialog.onCurrentColorChanged", currentColor);
                    rectHighlightedOutlineColorCurr.color = currentColor
                    trcAdminObj.traceMethodLeave("EnterLeave", "highlightedOutlineColorDialog.onCurrentColorChanged");
                }
                onColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "highlightedOutlineColorDialog.onColorChanged", color);
                    trcAdminObj.traceMethodLeave("EnterLeave", "highlightedOutlineColorDialog.onColorChanged");
                }
            }
        }

        // TextColor
        //----------------

        RowLayout {
            spacing: mainLayout.rowLayoutSpacing
            Label {
                id: lblTextColor
                text: qsTr("TextColor:")
                Layout.fillWidth: true
                color: rectTextColorOrig.border.color
                onColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "lblTextColor.onColorChanged", color);
                    trcAdminObj.traceMethodLeave("EnterLeave", "lblTextColor.onColorChanged");
                }
            }
            Rectangle {
                id: rectTextColorOrig
                width: 100
                height: 24
                border.color: style.isLightTheme ? "black" : "white"
                border.width: 1
                visible: !textColorDialog.visible
                onVisibleChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectTextColorOrig.onVisibleChanged", visible);
                    trcAdminObj.traceMethodLeave("EnterLeave", "rectTextColorOrig.onVisibleChanged");
                }
                color: style.textColor
                onColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectTextColorOrig.onColorChanged", color);
                    chkTextColorDefault.checked = style.isTextColorSetToDefault()
                    trcAdminObj.traceMethodLeave("EnterLeave", "rectTextColorOrig.onColorChanged");
                }
                MouseArea {
                    id: maTextColorOrig
                    anchors.fill: parent
                    acceptedButtons: Qt.LeftButton
                    onClicked: { textColorDialog.open(); }
                }
            }
            Rectangle {
                id: rectTextColorCurr
                width: 100
                height: 24
                border.color: style.isLightTheme ? "black" : "white"
                border.width: 1
                visible: textColorDialog.visible
                onVisibleChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectTextColorCurr.onVisibleChanged", visible);
                    if( visible ) { color = style.textColor }
                    trcAdminObj.traceMethodLeave("EnterLeave", "rectTextColorCurr.onVisibleChanged");
                }
                onColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "rectTextColorCurr.onColorChanged", color);
                    trcAdminObj.traceMethodLeave("EnterLeave", "rectTextColorCurr.onColorChanged");
                }
            }
            CheckBox {
                id: chkTextColorDefault
                text: "Default"
                enabled: !checked
                checked: style.isTextColorSetToDefault()
                onCheckedChanged: {
                    if( checked ) {
                        style.setTextColorToDefault()
                    }
                }
            }
            ColorDialog {
                id: textColorDialog
                showAlphaChannel: true
                onVisibleChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "textColorDialog.onVisibleChanged", visible);
                    if( visible ) { color = style.textColor }
                    trcAdminObj.traceMethodLeave("EnterLeave", "textColorDialog.onVisibleChanged");
                }
                onAccepted: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "textColorDialog.onAccepted", color);
                    style.textColor = color
                    trcAdminObj.traceMethodLeave("EnterLeave", "textColorDialog.onAccepted");
                }
                onCurrentColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "textColorDialog.onCurrentColorChanged", currentColor);
                    rectTextColorCurr.color = currentColor
                    trcAdminObj.traceMethodLeave("EnterLeave", "textColorDialog.onCurrentColorChanged");
                }
                onColorChanged: {
                    trcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "textColorDialog.onColorChanged", color);
                    trcAdminObj.traceMethodLeave("EnterLeave", "textColorDialog.onColorChanged");
                }
            }
        }

        // WindowTextColor
        //----------------

        RowLayout {
            spacing: mainLayout.rowLayoutSpacing
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
            spacing: mainLayout.rowLayoutSpacing
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
            spacing: mainLayout.rowLayoutSpacing
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
