import QtQuick 2.8
import QtQuick.Controls 2.1
import Qt.labs.platform 1.0 as Labs

Page {
	id: displayPage

	property double displayTime: 10.0
	property int animLoops: 0

	Rectangle {
		anchors.fill: parent
		color: "#111"
		z: -10
	}

	focus: true

	Keys.onRightPressed: animList.incrementCurrentIndex()
	Keys.onLeftPressed: animList.decrementCurrentIndex()
	Keys.onSpacePressed: {
		if(diashowTimer.running)
			diashowTimer.stop();
		else
			diashowTimer.start();
	}

	Keys.onReturnPressed: animList.toggleAnimation()
	Keys.onUpPressed: animList.jumpAnimation(1)
	Keys.onDownPressed: animList.jumpAnimation(-1)

	Keys.onDeletePressed: {
		imageModel.deleteImage(animList.currentIndex);
		animList.incrementCurrentIndex();
	}
	Keys.onEscapePressed: {
		mainStack.pop();
		rootWindow.showNormal();
	}

	CursorArea {
		anchors.fill: parent
		z: 5
		onContextClick: contextMenu.open()
	}

	Labs.Menu {
		id: contextMenu

		property bool displayed: false
		onAboutToShow: displayed = true
		onAboutToHide: displayed = false

		Labs.MenuItem {
			text: qsTr("Next")
			shortcut: "Right"
			onTriggered: animList.incrementCurrentIndex()
		}
		Labs.MenuItem {
			text: qsTr("Previous")
			shortcut: "Left"
			onTriggered: animList.decrementCurrentIndex()
		}
		Labs.MenuItem {
			text: diashowTimer.running ? qsTr("Pause") : qsTr("Play")
			shortcut: "Space"
			onTriggered: {
				if(diashowTimer.running)
					diashowTimer.stop();
				else
					diashowTimer.start();
			}
		}

		Labs.MenuSeparator {}

		Labs.MenuItem {
			text: qsTr("Toggle Animation")
			shortcut: "Enter"
			onTriggered: animList.toggleAnimation()
		}
		Labs.MenuItem {
			text: qsTr("Next Animation Frame")
			shortcut: "Up"
			onTriggered: animList.jumpAnimation(1)
		}
		Labs.MenuItem {
			text: qsTr("Previous Animation Frame")
			shortcut: "Down"
			onTriggered: animList.jumpAnimation(-1)
		}

		Labs.MenuSeparator {}

		Labs.MenuItem {
			text: qsTr("Delete Current Image")
			shortcut: "Del"
			onTriggered:  {
				imageModel.deleteImage(animList.currentIndex);
				animList.incrementCurrentIndex();
			}
		}

		Labs.MenuSeparator {}

		Labs.MenuItem {
			text: qsTr("Close Diashow")
			shortcut: "Escape"
			onTriggered: {
				mainStack.pop();
				rootWindow.showNormal();
			}
		}
		Labs.MenuItem {
			text: qsTr("Exit PWatcher")
			shortcut: "ALT+F4"
			onTriggered: Qt.quit()
		}
	}

	Timer {
		id: diashowTimer

		interval: displayTime * 1000
		repeat: true
		running: true

		function softRestart() {
			if(running)
				restart();
		}

		onTriggered: {
			if(!contextMenu.displayed)
				animList.autoNext()
		}
	}

	ListView {
		id: animList
		anchors.fill: parent
		snapMode: ListView.SnapOneItem
		orientation: ListView.Horizontal
		highlightRangeMode: ListView.StrictlyEnforceRange
		highlightMoveDuration: 250
		clip: true

		model: imageModel
		onCurrentIndexChanged: diashowTimer.softRestart();

		function toggleAnimation() {
			if(currentItem)
				currentItem.toggleAnim();
		}
		function jumpAnimation(count) {
			if(currentItem)
				currentItem.jumpAnim(count);
		}

		function autoNext() {
			if(currentItem) {
				if(currentItem.tryNext())
					animList.incrementCurrentIndex();
			}
		}

		delegate: DisplayDelegate {
			view: ListView.view
			isCurrent: ListView.isCurrentItem
			minLoops: displayPage.animLoops

			onAnimationPaused: diashowTimer.stop()
			onLoopTrigger: animList.autoNext()
		}
	}
}
