import QtQuick 2.8


AnimatedImage {
	id: animator

	anchors.centerIn: parent
	height: Math.min(sourceSize.height, parent.height)
	width: Math.min(sourceSize.width, parent.width)

	asynchronous: true
	cache: false
	fillMode: Image.PreserveAspectFit

	onPlayingChanged: {
		if(frameCount > 1)
			playing = true;
	}
	onStatusChanged: playing = (status == AnimatedImage.Ready)
}
