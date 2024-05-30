import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.platform

import FileStatsController

Item {
    id: root

    property url currentFile: ""

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Layout.alignment: Qt.AlignCenter

            Layout.fillHeight: true
            Layout.fillWidth: true

            Button {
                Layout.alignment: Qt.AlignCenter

                Layout.minimumHeight: 50
                Layout.minimumWidth: 100

                text: "Open file"

                onClicked: {
                    fileDialog.open();
                }
            }

            Button {
                Layout.alignment: Qt.AlignCenter

                Layout.minimumHeight: 50
                Layout.minimumWidth: 100

                enabled: root.currentFile != "" && !FileStatsController.isRunning

                text: "Start"

                onClicked: {
                    FileStatsController.start(root.currentFile);
                }
            }

            Button {
                id: pauseBtn

                Layout.alignment: Qt.AlignCenter

                Layout.minimumHeight: 50
                Layout.minimumWidth: 100

                enabled: FileStatsController.isRunning

                text: "Pause"

                onClicked: {
                    FileStatsController.pause();
                    pauseBtn.visible = false;
                }
            }

            Button {
                Layout.alignment: Qt.AlignCenter

                Layout.minimumHeight: 50
                Layout.minimumWidth: 100

                enabled: FileStatsController.isRunning
                visible: !pauseBtn.visible

                text: "Resume"

                onClicked: {
                    FileStatsController.resume();
                    pauseBtn.visible = true;
                }
            }

            Button {
                Layout.alignment: Qt.AlignCenter

                Layout.minimumHeight: 50
                Layout.minimumWidth: 100

                enabled: FileStatsController.isRunning

                text: "Stop"

                onClicked: {
                    FileStatsController.stop();

                    if (!pauseBtn.visible) {
                        pauseBtn.visible = true;
                    }
                }
            }
        }

        ProgressBar {
            Layout.fillWidth: true
            Layout.minimumHeight: 10
            Layout.maximumHeight: 10

            visible: FileStatsController.isRunning
            value: FileStatsController.progress
        }
    }

    FileDialog {
        id: fileDialog

        title: "Choose a file to get stats from"

        onAccepted: {
            root.currentFile = fileDialog.file;
        }
    }
}
