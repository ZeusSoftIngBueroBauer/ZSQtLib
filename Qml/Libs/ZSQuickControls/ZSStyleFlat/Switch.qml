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

T.Switch {
    id: control

    property string nameSpace: "ZS::QuickControls::Qml"
    property string className: "StyleFlat::Switch"
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

    implicitWidth: indicator.implicitWidth
    implicitHeight: background.implicitHeight

    background: Rectangle {
        implicitWidth: 140
        implicitHeight: Theme.baseSize * 3.8
        color: Theme.lightGray
        border.color: Theme.gray
    }

    leftPadding: 4

    indicator: Rectangle {
        id: switchHandle
        implicitWidth: Theme.baseSize * 4.8
        implicitHeight: Theme.baseSize * 2.6
        x: control.leftPadding
        anchors.verticalCenter: parent.verticalCenter
        radius: Theme.baseSize * 1.3
        color: Theme.light
        border.color: Theme.lightGray

        Rectangle {
            id: rectangle

            width: Theme.baseSize * 2.6
            height: Theme.baseSize * 2.6
            radius: Theme.baseSize * 1.3
            color: Theme.light
            border.color: Theme.gray
        }

        states: [
            State {
                name: "off"
                when: !control.checked && !control.down
            },
            State {
                name: "on"
                when: control.checked && !control.down

                PropertyChanges {
                    target: switchHandle
                    color: Theme.mainColor
                    border.color: Theme.mainColor
                }

                PropertyChanges {
                    target: rectangle
                    x: parent.width - width

                }
            },
            State {
                name: "off_down"
                when: !control.checked && control.down

                PropertyChanges {
                    target: rectangle
                    color: Theme.light
                }

            },
            State {
                name: "on_down"
                extend: "off_down"
                when: control.checked && control.down

                PropertyChanges {
                    target: rectangle
                    x: parent.width - width
                    color: Theme.light
                }

                PropertyChanges {
                    target: switchHandle
                    color: Theme.mainColorDarker
                    border.color: Theme.mainColorDarker
                }
            }
        ]
    }
}
