import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1
import QtQuick.Layouts 1.3
import Qt.labs.platform 1.0
import Qt.labs.settings 1.0
import de.skycoder42.quickextras 2.0

Page {
	id: selectionPage

	Settings {
		category: "selectionPage"

		property alias folder: dirField.text
		property alias recursive: recSwitch.checked
	}

	header: ActionBar {
		showMenuButton: false
		title: qsTr("PWatcher Setup")

		Material.foreground: "#FFF"
	}

	Pane {
		anchors.fill: parent

		GridLayout {
			id: grid
			anchors.fill: parent
			columns: 2
			columnSpacing: 14

			TextField {
				id: dirField
				Layout.fillWidth: true
				selectByMouse: true
				readOnly: true

				placeholderText: qsTr("Select the image folder…")
			}

			AppBarButton {
				size: dirField.height
				text: qsTr("Select a directory")
				imageSource: "image://svg/icons/ic_add"

				onClicked: dialog.open()

				FolderDialog {
					id: dialog
					options: FolderDialog.ShowDirsOnly
					folder: dirField.text

					onAccepted: dirField.text = folder
				}
			}

			Switch {
				id: recSwitch
				Layout.columnSpan: 2
				Layout.alignment: Qt.AlignLeft
				text: qsTr("Recursive")
			}

			Item {
				Layout.columnSpan: 2
				Layout.fillHeight: true
				Layout.fillWidth: true
			}
		}

		FloatingActionButton {
			id: fab
			anchors.right: parent.right
			anchors.bottom: parent.bottom

			text: qsTr("Start Diashow")
			imageSource: "image://svg/icons/ic_slideshow"
			Material.foreground: "#FFF"
		}
	}
}
