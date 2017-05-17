import QtQuick 2.8
import QtQuick.Controls 2.1

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

	Timer {
		id: diashowTimer

		interval: displayTime * 1000
		repeat: running
		running: true

		function softRestart() {
			if(running)
				restart();
		}

		onTriggered: {console.log("triggered"); animList.incrementCurrentIndex();}
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

		delegate: Item {
			width: animList.width
			height: animList.height

			function toggleAnim() {
				animator.paused = !animator.paused;
				if(animator.paused)
					diashowTimer.stop();
			}
			function jumpAnim(count) {
				animator.currentFrame = animator.currentFrame + count;
			}

			ProgressBar {
				anchors.left: parent.left
				anchors.right: parent.right
				anchors.top: parent.top
				z: 5

				value: animator.progress
				visible: value != 1.0
			}

			PImage {
				id: animator
				source: imageUrl
			}
		}
	}
}
