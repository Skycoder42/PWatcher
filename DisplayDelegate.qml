import QtQuick 2.8
import QtQuick.Controls 2.1

Item {
	width: view.width
	height: view.height

	property ListView view
	property bool isCurrent: false
	property alias minLoops: animator.minLoops

	signal animationPaused();
	signal loopTrigger();

	ListView.onIsCurrentItemChanged: {
		if(ListView.isCurrentItem) {//reset animation
			animator.reset();
		} else {
			animator.paused = true;//pause to save cpu
		}
	}

	function toggleAnim() {
		animator.paused = !animator.paused;
		if(animator.paused)
			animationPaused();
	}
	function jumpAnim(count) {
		animator.currentFrame = animator.currentFrame + count;
	}

	function tryNext() {
		return animator.autoNext();
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
		onMinLoopsReached: loopTrigger()
	}
}
