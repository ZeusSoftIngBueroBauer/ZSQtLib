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
import QtQuick.Templates 2.15 as T

T.Button {
    id: control

    property string nameSpace: "ZS::QuickControls::Qml"
    property string className: "StyleFlat::Button"
    property string objectName: "control"

    property var myTrcAdminObj: _ZSSys_trcServer.getTraceAdminObj(
        control.nameSpace, control.className, control.objectName);
    Component.onDestruction: {
        _ZSSys_trcServer.releaseTraceAdminObj(control.myTrcAdminObj);
    }

    Component.onCompleted: {
        console.debug("-> " + nameSpace + "::" + className + "::" + objectName + ".Component.onCompleted")
        console.debug("<- " + nameSpace + "::" + className + "::" + objectName + ".Component.onCompleted")
    }

    font: Theme.font

    property alias borderColor: buttonBackground.border.color
    onBorderColorChanged: {
        console.debug("-> Flat.Button.qml.Component.onBorderColorChanged")
        console.debug("<- Flat.Button.qml.Component.onBorderColorChanged")
    }

    implicitWidth: Math.max(background ? background.implicitWidth : 0,
                                         contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0,
                                          contentItem.implicitHeight + topPadding + bottomPadding)
    leftPadding: 4
    rightPadding: 4

    background: Rectangle {
        id: buttonBackground
        implicitWidth: 100
        implicitHeight: 40
        opacity: enabled ? 1 : 0.3
        border.color: Theme.mainColor
        border.width: 1
        radius: 10

        states: [
            State {
                name: "normal"
                when: !control.down
                PropertyChanges {
                    target: buttonBackground
                }
            },
            State {
                name: "down"
                when: control.down
                PropertyChanges {
                    target: buttonBackground
                    border.color: Theme.mainColorDarker
                }
            }
        ]
    }

    contentItem: Text {
        id: textItem
        text: control.text

        font: control.font
        opacity: enabled ? 1.0 : 0.3
        color: Theme.mainColor
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight

        states: [
            State {
                name: "normal"
                when: !control.down
            },
            State {
                name: "down"
                when: control.down
                PropertyChanges {
                    target: textItem
                    color: Theme.mainColorDarker
                }
            }
        ]
    }
}

