import QtQuick 2.15
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.15
import Qt.labs.settings 1.0
import ZSSysGUI 1.0

Dialog {
    property string nameSpace: "ZS::System::GUI::Qml"
    property string className: "ErrLogDlg"
    property string objectName: _errLogModel.objectName

    id: root
    title: "Error Log"
    width: 640
    height: 280
    modality: Qt.NonModal

    Settings {
        id: idSettings
        category: root.className + "." + root.objectName
        property alias x: root.x
        property alias y: root.y
        property alias width: root.width
        property alias height: root.height
    }

    contentItem: ErrLogWdgt {
        id: idErrLogWdgt
        anchors.fill: parent
    }
}
