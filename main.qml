import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import de.skycoder42.quickextras 2.0

ApplicationWindow {
	visible: true
	width: 400
	height: 600
	title: qsTr("PWatcher")

	StackView {
		anchors.fill: parent
		id: mainStack

		signal keyPress(int key);

		initialItem: SelectionView {
			id: selectView
		}

		focus: true
		Keys.onPressed: keyPress(event.key)
	}

	Component {
		id: displayCmp

		DisplayView {
			focus: true
		}
	}
}
