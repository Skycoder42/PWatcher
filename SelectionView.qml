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
		id: settings
		category: "selectionPage"

		property alias folder: dirField.text
		property alias recursive: recSwitch.checked
		property int mode: 0
		property bool loop: true
		property alias displayTime: displayBox.dValue
		property alias loops: loopBox.value
	}

	header: ActionBar {
		showMenuButton: false
		title: qsTr("PWatcher Setup")

		Material.foreground: "#FFF"
	}

	Flickable {
		anchors.fill: parent
		contentWidth: width
		contentHeight: pane.implicitHeight

		ScrollBar.vertical: ScrollBar {}

		Pane {
			id: pane
			anchors.left: parent.left
			anchors.right: parent.right

			ColumnLayout {
				id: grid
				anchors.fill: parent

				RowLayout {
					Layout.fillWidth: true

					TextField {
						id: dirField
						Layout.fillWidth: true
						selectByMouse: true
						readOnly: true

						placeholderText: qsTr("Select the image folder…")
					}

					AppBarButton {
						text: qsTr("Select a directory")
						imageSource: "image://svg/icons/ic_folder_open"

						onClicked: dialog.open()

						FolderDialog {
							id: dialog
							options: FolderDialog.ShowDirsOnly
							folder: dirField.text

							onAccepted: dirField.text = folder
						}
					}
				}

				Switch {
					id: recSwitch
					Layout.alignment: Qt.AlignLeft
					text: qsTr("Recursive")
				}

				TextField {
					id: limitField
					Layout.fillWidth: true
					selectByMouse: true
					placeholderText: qsTr("Allowed types (space seperated)")
				}

				GridLayout {
					columns: 2
					Layout.fillWidth: true

					RadioButton {
						id: playLinear
						text: qsTr("Playmode: Linear")
						checked: settings.mode == 0
						onCheckedChanged: {
							if(checked)
								settings.mode = 0;
						}
					}

					Item {
						Layout.fillWidth: true
						Layout.fillHeight: true
						CheckBox {
							id: linearLoopBox
							anchors.fill: parent
							visible: playLinear.checked
							text: qsTr("Loop")
							checked: settings.loop
							onCheckedChanged: settings.loop = checked
						}
					}

					RadioButton {
						id: playShuffle
						text: qsTr("Playmode: Shuffle")
						checked: settings.mode == 1
						onCheckedChanged: {
							if(checked)
								settings.mode = 1;
						}
					}

					Item {
						Layout.fillWidth: true
						Layout.fillHeight: true
						CheckBox {
							id: shuffleLoopBox
							anchors.fill: parent
							visible: playShuffle.checked
							text: qsTr("Loop")
							checked: settings.loop
							onCheckedChanged: settings.loop = checked
						}
					}

					RadioButton {
						id: playRandom
						Layout.columnSpan: 2
						text: qsTr("Playmode: Random")
						checked: settings.mode == 2
						onCheckedChanged: {
							if(checked)
								settings.mode = 2;
						}
					}
				}

				RowLayout {
					Layout.fillWidth: true

					Label {
						text: qsTr("Display time (sec):")
					}

					DoubleSpinBox {
						id: displayBox
						dFrom: 1.0
						dTo: 600.0
						dStepSize: 0.5
						dValue: 5.0
						decimals: 3
						Layout.fillWidth: true
					}
				}

				RowLayout {
					Layout.fillWidth: true

					Label {
						text: qsTr("Min. animation loops:")
					}

					SpinBox {
						id: loopBox
						from: 0
						to: 100
						Layout.fillWidth: true
					}
				}

				Item {
					Layout.fillHeight: true
					Layout.fillWidth: true
					Layout.minimumHeight: 56 + 2*14
				}
			}
		}
	}

	FloatingActionButton {
		id: fab
		anchors.right: parent.right
		anchors.rightMargin: 14
		anchors.bottom: parent.bottom
		anchors.bottomMargin: 14

		text: qsTr("Start Diashow")
		imageSource: "image://svg/icons/ic_slideshow"
		Material.foreground: "#FFF"

		onClicked: mainStack.push(displayCmp)
	}
}
