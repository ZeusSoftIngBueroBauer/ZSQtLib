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
//import Theme 1.0

T.CheckBox {
    id: control

    font: Theme.font

    implicitWidth: Math.max(background ? background.implicitWidth : 0,
                                         contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0,
                                          Math.max(contentItem.implicitHeight,
                                                   indicator ? indicator.implicitHeight : 0) + topPadding + bottomPadding)
    leftPadding: 4
    indicator: Rectangle {
        id: checkboxHandle
        implicitWidth: Theme.baseSize * 2.6
        implicitHeight: Theme.baseSize * 2.6
        x: control.leftPadding
        anchors.verticalCenter: parent.verticalCenter
        radius: 2
        border.color: Theme.mainColor

        Rectangle {
            id: rectangle
            width: Theme.baseSize * 1.4
            height: Theme.baseSize * 1.4
            x: Theme.baseSize * 0.6
            y: Theme.baseSize * 0.6
            radius: Theme.baseSize * 0.4
            visible: false
            color: Theme.mainColor
        }

        states: [
            State {
                name: "unchecked"
                when: !control.checked && !control.down
            },
            State {
                name: "checked"
                when: control.checked && !control.down

                PropertyChanges {
                    target: rectangle
                    visible: true
                }
            },
            State {
                name: "unchecked_down"
                when: !control.checked && control.down

                PropertyChanges {
                    target: rectangle
                    color: Theme.mainColorDarker
                }

                PropertyChanges {
                    target: checkboxHandle
                    border.color: Theme.mainColorDarker
                }
            },
            State {
                name: "checked_down"
                extend: "unchecked_down"
                when: control.checked && control.down

                PropertyChanges {
                    target: rectangle
                    visible: true
                }
            }
        ]
    }

    background: Rectangle {
        implicitWidth: 140
        implicitHeight: Theme.baseSize * 3.8
        color: Theme.lightGray
        border.color: Theme.gray
    }

    contentItem: Text {
        leftPadding: control.indicator.width + 4

        text: control.text
        font: control.font
        color: Theme.dark
        elide: Text.ElideRight
        visible: control.text
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
    }
}

