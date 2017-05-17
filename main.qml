import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import de.skycoder42.quickextras 2.0

ApplicationWindow {
	id: rootWindow
	visible: true
	width: 400
	height: 600
	title: qsTr("PWatcher")

	StackView {
		anchors.fill: parent
		id: mainStack

		initialItem: SelectionView {
			id: selectView
		}

		focus: true
		onCurrentItemChanged: {
			if (currentItem)
				currentItem.focus = true
		}

	}

	Component {
		id: displayCmp

		DisplayView {
			focus: true
		}
	}
}
