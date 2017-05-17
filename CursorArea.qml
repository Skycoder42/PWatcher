import QtQuick 2.8
import de.skycoder42.pwatcher 1.1

MouseArea {
	id: mouseArea
	acceptedButtons: Qt.RightButton
	cursorShape: Qt.BlankCursor
	hoverEnabled: true

	signal contextClick();

	onClicked: {
		if(mouse.button == Qt.RightButton)
			contextClick();
	}

	onPositionChanged: {
		cursorShape = Qt.ArrowCursor;
		hideMouseTimer.restart();
	}

	Timer {
		id: hideMouseTimer
		interval: 1000
		repeat: false
		running: true
		onTriggered: mouseArea.cursorShape = Qt.BlankCursor
	}

	Component.onCompleted: CppHelper.moveCursor()
}
