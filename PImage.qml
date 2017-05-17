import QtQuick 2.8
import QtQuick.Controls 2.1

AnimatedImage {
	id: animator

	property int loops: 0
	property int minLoops: 0
	property bool reportLoops: false
	property double zoomFactor: 1.0
	property point zoomCenter: Qt.point(0, 0)

	signal minLoopsReached();

	function reset() {
		reportLoops = false;
		currentFrame = 0;
		loops = 0;
		paused = false;
		zoomFactor = 1.0;
	}

	function autoNext() {
		if(frameCount <= 1 || loops >= minLoops)
			return true;
		else {
			reportLoops = true;
			return false;
		}
	}

	anchors.centerIn: parent
	height: Math.min(sourceSize.height, parent.height)
	width: Math.min(sourceSize.width, parent.width)

	asynchronous: true
	cache: false
	mipmap: true
	fillMode: Image.PreserveAspectFit

	transform: Scale {
		property point localCenter: mapFromItem(parent, zoomCenter.x, zoomCenter.y)
		origin.x: localCenter.x
		origin.y: localCenter.y
		xScale: zoomFactor
		yScale: zoomFactor
	}

	onPlayingChanged: {
		if(frameCount > 1)
			playing = true;
	}
	onStatusChanged: playing = (status == AnimatedImage.Ready)
	onFrameChanged: {
		if(currentFrame == 0)
			loops++;
	}

	onLoopsChanged: {
		if(reportLoops && loops >= minLoops) {
			reportLoops = false;
			minLoopsReached();
		}
	}

	Label {
		id: errorLabel
		visible: status == AnimatedImage.Error
		anchors.centerIn: parent
		color: "red"

		text: qsTr("Failed to load image:\n%1").arg(source)
		horizontalAlignment: Qt.AlignHCenter
	}
}
