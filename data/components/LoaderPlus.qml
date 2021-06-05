// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.10
import QtQuick.Layouts 1.10

Loader {
    id: loader

    Binding { target: loader; property: "Layout.alignment"; when: loader.item !== null; value: ((loader.item || {}).Layout || {}).alignment }
    Binding { target: loader; property: "Layout.bottomMargin"; when: loader.item !== null; value: ((loader.item || {}).Layout || {}).bottomMargin }
    Binding { target: loader; property: "Layout.fillWidth"; when: loader.item !== null; value: ((loader.item || {}).Layout || {}).fillWidth }
    Binding { target: loader; property: "Layout.leftMargin"; when: loader.item !== null; value: ((loader.item || {}).Layout || {}).leftMargin }
    Binding { target: loader; property: "Layout.margins"; when: loader.item !== null; value: ((loader.item || {}).Layout || {}).margins }
    Binding { target: loader; property: "Layout.maximumWidth"; when: loader.item !== null; value: ((loader.item || {}).Layout || {}).maximumWidth }
    Binding { target: loader; property: "Layout.preferredHeight"; when: loader.item !== null; value: ((loader.item || {}).Layout || {}).preferredHeight }
    Binding { target: loader; property: "Layout.preferredWidth"; when: loader.item !== null; value: ((loader.item || {}).Layout || {}).preferredWidth }
    Binding { target: loader; property: "Layout.rightMargin"; when: loader.item !== null; value: ((loader.item || {}).Layout || {}).rightMargin }
    Binding { target: loader; property: "Layout.topMargin"; when: loader.item !== null; value: ((loader.item || {}).Layout || {}).topMargin }

}
