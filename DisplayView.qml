import QtQuick 2.8
import QtQuick.Controls 2.1

Page {
	id: displayPage

	Rectangle {
		anchors.fill: parent
		color: "#111"
		z: -10
	}

	Connections {
		target: mainStack
		onKeyPress: {
			switch(key) {
			case Qt.Key_Right:
				animList.incrementCurrentIndex();
				break;
			case Qt.Key_Left:
				animList.decrementCurrentIndex();
				break;
			}
		}
	}

	property var imageModel: [
		"file:///home/sky/etc/Mine/Anime/Top/aB1eV5A_460sv.apng",
		"file:///home/sky/etc/Mine/Anime/Top/a105tpyo.png",
		"file:///home/sky/etc/Mine/Anime/Top/pokemon-porn-r34-2039262.gif",
		"file:///home/sky/etc/Mine/Anime/Top/aB1eV5A_460sv.apng",
		"file:///home/sky/etc/Mine/Anime/Top/a105tpyo.png",
		"file:///home/sky/etc/Mine/Anime/Top/pokemon-porn-r34-2039262.gif",
		"file:///home/sky/etc/Mine/Anime/Top/aB1eV5A_460sv.apng",
		"file:///home/sky/etc/Mine/Anime/Top/a105tpyo.png",
		"file:///home/sky/etc/Mine/Anime/Top/pokemon-porn-r34-2039262.gif",
		"file:///home/sky/etc/Mine/Anime/Top/aB1eV5A_460sv.apng",
		"file:///home/sky/etc/Mine/Anime/Top/a105tpyo.png",
		"file:///home/sky/etc/Mine/Anime/Top/pokemon-porn-r34-2039262.gif",
		"file:///home/sky/etc/Mine/Anime/Top/aB1eV5A_460sv.apng",
		"file:///home/sky/etc/Mine/Anime/Top/a105tpyo.png",
		"file:///home/sky/etc/Mine/Anime/Top/pokemon-porn-r34-2039262.gif",
		"file:///home/sky/etc/Mine/Anime/Top/aB1eV5A_460sv.apng",
		"file:///home/sky/etc/Mine/Anime/Top/a105tpyo.png",
		"file:///home/sky/etc/Mine/Anime/Top/pokemon-porn-r34-2039262.gif",
		"file:///home/sky/etc/Mine/Anime/Top/aB1eV5A_460sv.apng",
		"file:///home/sky/etc/Mine/Anime/Top/a105tpyo.png",
		"file:///home/sky/etc/Mine/Anime/Top/pokemon-porn-r34-2039262.gif",
		"file:///home/sky/etc/Mine/Anime/Top/aB1eV5A_460sv.apng",
		"file:///home/sky/etc/Mine/Anime/Top/a105tpyo.png",
		"file:///home/sky/etc/Mine/Anime/Top/pokemon-porn-r34-2039262.gif",
		"file:///home/sky/etc/Mine/Anime/Top/aB1eV5A_460sv.apng",
		"file:///home/sky/etc/Mine/Anime/Top/a105tpyo.png",
		"file:///home/sky/etc/Mine/Anime/Top/pokemon-porn-r34-2039262.gif",
		"file:///home/sky/etc/Mine/Anime/Top/aB1eV5A_460sv.apng",
		"file:///home/sky/etc/Mine/Anime/Top/a105tpyo.png",
		"file:///home/sky/etc/Mine/Anime/Top/pokemon-porn-r34-2039262.gif",
		"file:///home/sky/etc/Mine/Anime/Top/aB1eV5A_460sv.apng",
		"file:///home/sky/etc/Mine/Anime/Top/a105tpyo.png",
		"file:///home/sky/etc/Mine/Anime/Top/pokemon-porn-r34-2039262.gif",
		"file:///home/sky/etc/Mine/Anime/Top/aB1eV5A_460sv.apng",
		"file:///home/sky/etc/Mine/Anime/Top/a105tpyo.png",
		"file:///home/sky/etc/Mine/Anime/Top/pokemon-porn-r34-2039262.gif"
	]

	ListView {
		id: animList
		anchors.fill: parent
		snapMode: ListView.SnapOneItem
		orientation: ListView.Horizontal
		highlightRangeMode: ListView.StrictlyEnforceRange

		clip: true

		model: imageModel

		delegate: Item {
			width: animList.width
			height: animList.height

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
				source: imageModel[index]
			}
		}
	}
}
