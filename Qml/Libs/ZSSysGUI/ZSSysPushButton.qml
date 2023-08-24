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

import QtQuick 2.15

Item {
    id: root

    property alias text: label.text

    implicitWidth: label.implicitWidth + __margin * 2
    implicitHeight: label.implicitHeight

    readonly property int __margin: 35

    BorderImage {
        id: background
        anchors.fill: parent
        border {
            left: root.__margin
            right: root.__margin
        }
        source: "qrc:/ZS/CnctState/LedCircleGreen.png"

        Text {
            id: label
            anchors.centerIn: parent
            font.pixelSize: 30
        }
    }
}
