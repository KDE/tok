// SPDX-FileCopyrightText: 2020 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: AGPL-3.0-or-later

import QtQuick 2.10
import org.kde.kirigami 2.14 as Kirigami
import QtQuick.Controls 2.10 as QQC2

QQC2.Popup {
	id: imagePopup

	anchors.centerIn: QQC2.Overlay.overlay
	modal: true
	property alias source: popupImage.source

	background: Item {}
	Image {
		id: popupImage
		x: ((parent.QQC2.Overlay.overlay || {width: 0}).width / 2) - (this.implicitWidth / 2)
		y: ((parent.QQC2.Overlay.overlay || {height: 0}).height / 2) - (this.implicitHeight / 2)

		PinchArea {
			anchors.fill: parent
			pinch.target: popupImage
			pinch.minimumRotation: -360
			pinch.maximumRotation: 360
			pinch.minimumScale: 0.1
			pinch.maximumScale: 10
			pinch.dragAxis: Pinch.XAndYAxis
		}
		MouseArea {
			drag.target: parent
			anchors.fill: parent
			onWheel: {
				if (wheel.modifiers & Qt.ControlModifier) {
					popupImage.rotation += wheel.angleDelta.y / 120 * 5
					if (Math.abs(popupImage.rotation) < 4)
						popupImage.rotation = 0
				} else {
					popupImage.rotation += wheel.angleDelta.x / 120
					if (Math.abs(popupImage.rotation) < 0.6)
						popupImage.rotation = 0
					var scaleBefore = popupImage.scale
					popupImage.scale += popupImage.scale * wheel.angleDelta.y / 120 / 10
				}
			}
		}
	}

	width: QQC2.Overlay.overlay.width
	height: QQC2.Overlay.overlay.height
}
