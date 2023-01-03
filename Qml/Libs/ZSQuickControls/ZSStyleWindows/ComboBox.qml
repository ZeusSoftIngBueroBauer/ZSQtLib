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
import QtQuick.Window 2.15
import QtQuick.Templates 2.15 as T
import QtQuick.Controls 2.15
import QtQuick.Controls.impl 2.15
import QtQuick.Controls.Fusion 2.15
import QtQuick.Controls.Fusion.impl 2.15

T.ComboBox {
    id: control

    readonly property var style: _ZSQuickControls_windowsStyle

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding,
                             implicitIndicatorHeight + topPadding + bottomPadding)

    leftPadding: padding + (!control.mirrored || !indicator || !indicator.visible ? 0 : indicator.width + spacing)
    rightPadding: padding + (control.mirrored || !indicator || !indicator.visible ? 0 : indicator.width + spacing)

    delegate: MenuItem {
        width: ListView.view.width
        text: control.textRole ? (Array.isArray(control.model) ? modelData[control.textRole] : model[control.textRole]) : modelData
        font.weight: control.currentIndex === index ? Font.DemiBold : Font.Normal
        highlighted: control.highlightedIndex === index
        hoverEnabled: control.hoverEnabled
    }

    indicator: ColorImage {
        x: control.mirrored ? control.padding : control.width - width - control.padding
        y: control.topPadding + (control.availableHeight - height) / 2
        //color: control.editable ? control.palette.text : control.palette.buttonText
        color: control.editable ? style.textColor : style.buttonTextColor
        source: "qrc:/qt-project.org/imports/QtQuick/Controls.2/Fusion/images/arrow.png"
        width: 20
        fillMode: Image.Pad
    }

    contentItem: T.TextField {
        topPadding: 4
        leftPadding: 4 - control.padding
        rightPadding: 4 - control.padding
        bottomPadding: 4

        text: control.editable ? control.editText : control.displayText

        enabled: control.editable
        autoScroll: control.editable
        readOnly: control.down
        inputMethodHints: control.inputMethodHints
        validator: control.validator
        selectByMouse: control.selectTextByMouse

        font: control.font
        //color: control.editable ? control.palette.text : control.palette.buttonText
        //selectionColor: control.palette.highlight
        //selectedTextColor: control.palette.highlightedText
        color: control.editable ? style.textColor : style.buttonTextColor
        selectionColor: style.highlightColor
        selectedTextColor: style.highlightedTextColor
        verticalAlignment: Text.AlignVCenter

        background: PaddedRectangle {
            id: rectTextFieldBackground
            clip: true
            radius: 2
            padding: 1
            leftPadding: control.mirrored ? -2 : padding
            rightPadding: !control.mirrored ? -2 : padding
            //color: control.palette.base
            color: style.baseColor
            visible: control.editable && !control.flat

            Rectangle {
                id: rectTextFieldBackgroundOutline
                x: parent.width - width
                y: 1
                width: 1
                height: parent.height - 2
                //color: Fusion.buttonOutline(control.palette, control.activeFocus, control.enabled)
                color: control.style.buttonOutlineColor(control.activeFocus, control.enabled)
            }

            Rectangle {
                id: rectTextFieldBackgroundTop
                x: 1
                y: 1
                width: parent.width - 3
                height: 1
                //color: Fusion.topShadow
                color: control.style.topShadowColor
            }
        }

        Rectangle {
            x: 1 - control.leftPadding
            y: 1
            width: control.width - 2
            height: control.height - 2
            color: "transparent"
            //border.color: Color.transparent(Fusion.highlightedOutline(control.palette), 40 / 255)
            border.color: Color.transparent(control.style.highlightedOutlineColor, 40 / 255)
            visible: control.activeFocus
            radius: 1.7
        }
    }

    background: ButtonPanel {
        implicitWidth: 120
        implicitHeight: 24

        control: control
        visible: !control.flat || control.down
        // ### TODO: fix control.contentItem.activeFocus
        highlighted: control.visualFocus || control.contentItem.activeFocus
    }

    popup: T.Popup {
        width: control.width
        height: Math.min(contentItem.implicitHeight + 2, control.Window.height - topMargin - bottomMargin)
        topMargin: 6
        bottomMargin: 6
        palette: control.palette
        padding: 1

        contentItem: ListView {
            clip: true
            implicitHeight: contentHeight
            model: control.delegateModel
            currentIndex: control.highlightedIndex
            highlightRangeMode: ListView.ApplyRange
            highlightMoveDuration: 0

            T.ScrollBar.vertical: ScrollBar { }
        }

        background: Rectangle {
            //color: control.popup.palette.window
            color: control.style.popupWindowColor
            //border.color: Fusion.outline(control.palette)
            border.color: control.style.outlineColor

            Rectangle {
                z: -1
                x: 1; y: 1
                width: parent.width
                height: parent.height
                //color: control.palette.shadow
                color: control.style.shadowColor
                opacity: 0.2
            }
        }
    }

    Connections {
        target: style
        function onHighlightedOutlineColorChanged() {
            rectTextFieldBackgroundOutline.color = control.style.buttonOutlineColor(
                control.activeFocus, control.enabled, style.currentTheme)
        }
        function onOutlineColorChanged() {
            rectTextFieldBackgroundOutline.color = control.style.buttonOutlineColor(
                control.activeFocus, control.enabled, style.currentTheme)
        }
    }
}
