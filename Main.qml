import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Graph

Window {
    id: root

    minimumWidth: 760
    minimumHeight: 600

    visible: true
    title: qsTr("file stats")

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.maximumHeight: 20
            Layout.margins: 20

            Text {
                Layout.fillWidth: true
                Layout.fillHeight: true
                text: footer.currentFile
            }
        }

        Graph {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 20
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.maximumHeight: 1
            Layout.minimumHeight: 1

            border.width: 1
        }

        Footer {
            id: footer

            Layout.fillWidth: true
            Layout.preferredHeight: root.height * 0.1
        }
    }
}
