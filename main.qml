import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import de.skycoder42.quickextras 2.0

ApplicationWindow {
	visible: true
	width: 400
	height: 480
	title: qsTr("PWatcher")

	SelectionView {
		id: selectView

		anchors.fill: parent
	}
}
