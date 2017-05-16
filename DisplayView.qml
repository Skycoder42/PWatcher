import QtQuick 2.8
import QtQuick.Controls 2.1

Page {
	id: displayPage

	Rectangle {
		anchors.fill: parent
		color: "#111"
		z: -10
	}

	ProgressBar {
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.top: parent.top
		z: 5

		value: animator.progress
		visible: value != 1.0
	}

	AnimatedImage {
		id: animator

		anchors.centerIn: parent
		height: Math.min(sourceSize.height, parent.height)
		width: Math.min(sourceSize.width, parent.width)

		source: "file:///home/sky/etc/Mine/Anime/Top/aB1eV5A_460sv.apng"
		asynchronous: true
		cache: false
		fillMode: Image.PreserveAspectFit

		onPlayingChanged: {
			if(frameCount > 1)
				playing = true;
		}
		onStatusChanged: playing = (status == AnimatedImage.Ready)
	}
}
