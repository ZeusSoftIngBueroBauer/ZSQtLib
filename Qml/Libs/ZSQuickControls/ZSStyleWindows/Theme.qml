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

pragma Singleton

import QtQuick 2.15

QtObject {
    id: root

    readonly property var __theme: typeof(_ZSQuickControls_themeWindowsStyle) === "undefined" ? null : _ZSQuickControls_themeWindowsStyle

    readonly property color gray: "#b2b1b1"
    readonly property color lightGray: "#dddddd"
    readonly property color light: "#ffffff"
    readonly property color blue: "#2d548b"
    property color mainColor: __theme ? __theme.mainColor : "#17a81a"
    readonly property color dark: "#222222"
    readonly property color mainColorDarker: Qt.darker(mainColor, 1.5)

    property int baseSize: 10

    readonly property int smallSize: 10
    readonly property int largeSize: 16

    property font font
    font.bold: true
    font.underline: false
    font.pixelSize: 14
    font.family: "arial"


    // ToolButton
    //------------

    property bool toolButtonBorderAlwaysVisible: true
    property color toolButtonBorderColor: gray
    property int toolButtonBorderRadius: 0
}
