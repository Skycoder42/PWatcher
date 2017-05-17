import QtQuick 2.8
import QtQuick.Controls 2.1

AnimatedImage {
	id: animator

	anchors.centerIn: parent
	height: Math.min(sourceSize.height, parent.height)
	width: Math.min(sourceSize.width, parent.width)

	asynchronous: true
	cache: false
	mipmap: true
	fillMode: Image.PreserveAspectFit

	onPlayingChanged: {
		if(frameCount > 1)
			playing = true;
	}
	onStatusChanged: playing = (status == AnimatedImage.Ready)

	Label {
		id: errorLabel
		visible: status == AnimatedImage.Error

		text: qsTr("Failed to load image:\n%1").arg(source)
	}
}
