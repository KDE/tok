// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

import Qt.labs.platform 1.1 as QQC2
import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Layouts 1.10
import org.kde.Tok 1.0 as Tok
import org.kde.kirigami 2.15 as Kirigami

QQC2.RoundButton {
    id: _button

    radius: height/2

    required property bool isDown
    required property ListView view

    states: [
        State {
            name: "at-beginning"
            when: isDown ? _button.view.atYEnd : _button.view.atYBeginning
        },
        State {
            name: "not-at-beginning"
            when: isDown ? !_button.view.atYEnd : !_button.view.atYBeginning
        }
    ]

    transitions: [
        Transition {
            to: "at-beginning"

            SequentialAnimation {
                NumberAnimation {
                    target: _button
                    property: "opacity"
                    to: 0
                    from: 1
                    duration: Kirigami.Units.shortDuration
                    easing.type: Easing.InOutQuad
                }
                PropertyAction {
                    target: _button
                    property: "visible"
                    value: false
                }
            }
        },
        Transition {
            to: "not-at-beginning"

            SequentialAnimation {
                PropertyAction {
                    target: _button
                    property: "visible"
                    value: true
                }
                NumberAnimation {
                    target: _button
                    property: "opacity"
                    from: 0
                    to: 1
                    duration: Kirigami.Units.shortDuration
                    easing.type: Easing.InOutQuad
                }
            }
        }
    ]

    icon.name: isDown ? "arrow-down" : "arrow-up"
    onClicked: view.positionViewAtBeginning()
}
