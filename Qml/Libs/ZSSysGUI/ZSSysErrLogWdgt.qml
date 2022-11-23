import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import ZSSysGUI 1.0

Rectangle {
    property string nameSpace: "ZS::System::GUI"
    property string className: "ZSSysErrLogWdgt.qml"
    property string objectName: _errLogModel.objectName

    id: root
    color: "chartreuse"
    border.color: "black"

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        ToolBar {
            property string className: root.className + "::" + "ToolBar"
            property var myTrcAdminObj: _trcServer.getTraceAdminObj(
                root.nameSpace, className, root.objectName)
            Component.onDestruction: {
                _trcServer.releaseTraceAdminObj(myTrcAdminObj);
            }

            id: toolBar
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 20

            background: Rectangle {
                color: "aquamarine"
                border.color: "black"
            }

            RowLayout {
                anchors.fill: parent
                spacing: 0

                TextField {
                    id: tfFilter
                    Layout.minimumHeight: font.pixelSize + 4
                    Layout.fillWidth: true
                    //onTextEdited: tableView.contentY = 0
                }
            }
        }

        Rectangle {
            color: "darkgreen"
            border.color: "black"
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: header.height

            RowLayout {
                property string className: root.className + "::" + "Header"
                property var myTrcAdminObj: _trcServer.getTraceAdminObj(
                    root.nameSpace, className, root.objectName)
                Component.onDestruction: {
                    _trcServer.releaseTraceAdminObj(myTrcAdminObj);
                }

                id: header
                width: tableView.contentWidth
                height: 30
                spacing: 0

                Repeater {
                    id: headerRepeater
                    model: tableView.model.columnCount()

                    SortableColumnHeading {
                        id: clmHeading
                        width: Math.min(600, tableView.model.columnWidth(index))
                        height: 40 //header.height
                        text: tableView.model.headerData(index, Qt.Horizontal)
                        onSorting: {
                            for( var clm = 0; clm < headerRepeater.model; ++clm ) {
                                if( clm != index ) {
                                    headerRepeater.itemAt(clm).stopSorting()
                                }
                            }
                            tableView.model.sort(index, state == "up" ? Qt.AscendingOrder : Qt.DescendingOrder)
                        }
                    }
                }
            }
        }

        TableView {
            property string className: root.className + "::" + "TableView"
            property var myTrcAdminObj: _trcServer.getTraceAdminObj(
                root.nameSpace, className, root.objectName)
            Component.onDestruction: {
                _trcServer.releaseTraceAdminObj(myTrcAdminObj);
            }

            id: tableView
            model: _errLogModelProxy
            Layout.fillHeight: true
            Layout.preferredWidth: parent.width
            columnSpacing: 0
            rowSpacing: 0
            columnWidthProvider: function(column) {
                myTrcAdminObj.traceMethodEnterWithInArgs("EnterLeave", "columnWidthProvider", "Clm: " + column);
                var clmWidth = Math.min(600, model.columnWidth(column));
                myTrcAdminObj.traceMethodLeaveWithReturn("EnterLeave", "columnWidthProvider", clmWidth);
                return clmWidth;
            }

            delegate: Rectangle {
                implicitHeight: tableCellText.implicitHeight
                border.color: "black"
                border.width: 1
                color: Qt.rgba(0.9, 0.9, 0.9)

                Text {
                    id: tableCellText
                    text: model.display
                    width: parent.width
                    elide: Text.ElideRight
                    font.preferShaping: false
                }
            }

            ScrollBar.horizontal: ScrollBar {}
            ScrollBar.vertical: ScrollBar {}
        }
    }
}
