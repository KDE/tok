// SPDX-FileCopyrightText: 2020 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: AGPL-3.0-or-later

import QtQuick 2.15
import org.kde.kirigami 2.14 as Kirigami
import QtQuick.Controls 2.10 as QQC2

import org.kde.Tok 1.0 as Tok

QQC2.Popup {
	id: imagePopup

	anchors.centerIn: QQC2.Overlay.overlay
	modal: true
	property alias key: imageData.key

	Tok.RelationalListener {
		id: imageData

		model: tClient.messagesStore
		key: [del.mChatID, del.mID]
		shape: QtObject {
			required property string imageURL
			required property string imageCaption
		}
	}

	background: Item {
		TapHandler {
			target: imagePopup.QQC2.Overlay.overlay
			onTapped: imagePopup.close()
		}

		QQC2.Label {
			id: raberu

			parent: imagePopup.QQC2.Overlay.overlay
			visible: imagePopup.visible && text !== ""
			z: 999

			text: imageData.data.imageCaption
			color: "white"
			padding: Kirigami.Units.gridUnit
			background: Rectangle {
				radius: 4
				color: Qt.rgba(0, 0, 0, 0.3)
			}

			anchors {
				bottom: parent.bottom
				horizontalCenter: parent.horizontalCenter
				margins: Kirigami.Units.gridUnit
			}
		}

		// QQC2.Button {
		// 	icon.name: "download"
		// 	anchors {
		// 		bottom: parent.bottom
		// 		right: parent.right
		// 		margins: Kirigami.Units.gridUnit
		// 	}
		// }
	}
	Image {
		id: popupImage

		source: imageData.data.imageURL
		smooth: true
		mipmap: true

		fillMode: Image.PreserveAspectFit
		horizontalAlignment: Image.AlignHCenter

		anchors {
			fill: parent
			topMargin: Kirigami.Units.gridUnit*2
			bottomMargin: (raberu.visible ? raberu.height : 0) + Kirigami.Units.gridUnit*2
		}

		MouseArea {
			anchors.fill: parent
			onWheel: {
				if (wheel.modifiers & Qt.ControlModifier) {
					if (Math.abs(popupImage.rotation) < 0.6)
						popupImage.rotation = 0
					popupImage.scale += popupImage.scale * wheel.angleDelta.y / 120 / 10
				}
			}
		}
	}

	QQC2.Button {
		icon.name: "dialog-close"
		onClicked: imagePopup.close()
		anchors {
			top: parent.top
			right: parent.right
			margins: Kirigami.Units.gridUnit
		}
	}

	width: QQC2.Overlay.overlay.width
	height: QQC2.Overlay.overlay.height
}
