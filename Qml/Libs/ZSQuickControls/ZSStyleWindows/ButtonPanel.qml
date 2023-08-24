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

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.impl 2.12
import QtQuick.Controls.Fusion 2.12
import QtQuick.Controls.Fusion.impl 2.12

Rectangle {
    id: panel

    readonly property var style: _ZSQuickControls_windowsStyle

    property Item control
    property bool highlighted: control.highlighted

    visible: !control.flat || control.down || control.checked

    //color: Fusion.buttonColor(control.palette, panel.highlighted, control.down || control.checked, control.hovered)
    color: style.buttonColor(panel.highlighted, control.down || control.checked, control.hovered, style.currentTheme)
    gradient: control.down || control.checked ? null : buttonGradient

    Gradient {
        id: buttonGradient
        GradientStop {
            id: buttonGradientStart
            position: 0
            //color: Fusion.gradientStart(Fusion.buttonColor(panel.control.palette, panel.highlighted, panel.control.down, panel.control.hovered))
            color: style.panelStartColor(panel.highlighted, panel.control.down, panel.control.hovered, style.currentTheme)
        }
        GradientStop {
            id: buttonGradientStop
            position: 1
            //color: Fusion.gradientStop(Fusion.buttonColor(panel.control.palette, panel.highlighted, panel.control.down, panel.control.hovered))
            color: style.panelStopColor(panel.highlighted, panel.control.down, panel.control.hovered, style.currentTheme)
        }
    }

    radius: 2
    //border.color: Fusion.buttonOutline(control.palette, panel.highlighted || control.visualFocus, control.enabled)
    border.color: style.buttonOutlineColor(panel.highlighted || control.visualFocus, control.enabled)

    Rectangle {
        x: 1; y: 1
        width: parent.width - 2
        height: parent.height - 2
        //border.color: Fusion.innerContrastLine
        border.color: style.innerContrastLineColor
        color: "transparent"
        radius: 2
    }

    Connections {
        target: style
        function onBackgroundStartColorChanged() {
            buttonGradientStart.color = panel.style.panelStartColor(
                panel.highlighted, panel.control.down, panel.control.hovered, style.currentTheme)
        }
        function onBackgroundStopColorChanged() {
            buttonGradientStop.color = panel.style.panelStopColor(
                panel.highlighted, panel.control.down, panel.control.hovered, style.currentTheme)
        }
        function onButtonColorChanged() {
            panel.color = style.buttonColor(
                panel.highlighted, control.down || control.checked, control.hovered, style.currentTheme)
        }
        function onOutlineColorChanged() {
            panel.border.color = style.buttonOutlineColor(
                panel.highlighted || control.visualFocus, control.enabled, style.currentTheme)
        }
        function onHighlightedOutlineColorChanged() {
            panel.border.color = style.buttonOutlineColor(
                panel.highlighted || control.visualFocus, control.enabled, style.currentTheme)
        }
    }
}
