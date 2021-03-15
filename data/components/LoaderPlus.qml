import QtQuick 2.10
import QtQuick.Layouts 1.10

Loader {
    id: loader

    Binding { target: loader; property: "Layout.alignment"; when: loader.item !== null; value: ((loader.item || {}).Layout || {}).alignment }
    Binding { target: loader; property: "Layout.bottomMargin"; when: loader.item !== null; value: ((loader.item || {}).Layout || {}).bottomMargin }
    Binding { target: loader; property: "Layout.column"; when: loader.item !== null; value: ((loader.item || {}).Layout || {}).column }
    Binding { target: loader; property: "Layout.columnSpan"; when: loader.item !== null; value: ((loader.item || {}).Layout || {}).columnSpan }
    Binding { target: loader; property: "Layout.fillHeight"; when: loader.item !== null; value: ((loader.item || {}).Layout || {}).fillHeight }
    Binding { target: loader; property: "Layout.fillWidth"; when: loader.item !== null; value: ((loader.item || {}).Layout || {}).fillWidth }
    Binding { target: loader; property: "Layout.leftMargin"; when: loader.item !== null; value: ((loader.item || {}).Layout || {}).leftMargin }
    Binding { target: loader; property: "Layout.margins"; when: loader.item !== null; value: ((loader.item || {}).Layout || {}).margins }
    Binding { target: loader; property: "Layout.maximumHeight"; when: loader.item !== null; value: ((loader.item || {}).Layout || {}).maximumHeight }
    Binding { target: loader; property: "Layout.maximumWidth"; when: loader.item !== null; value: ((loader.item || {}).Layout || {}).maximumWidth }
    Binding { target: loader; property: "Layout.minimumHeight"; when: loader.item !== null; value: ((loader.item || {}).Layout || {}).minimumHeight }
    Binding { target: loader; property: "Layout.minimumWidth"; when: loader.item !== null; value: ((loader.item || {}).Layout || {}).minimumWidth }
    Binding { target: loader; property: "Layout.preferredHeight"; when: loader.item !== null; value: ((loader.item || {}).Layout || {}).preferredHeight }
    Binding { target: loader; property: "Layout.preferredWidth"; when: loader.item !== null; value: ((loader.item || {}).Layout || {}).preferredWidth }
    Binding { target: loader; property: "Layout.rightMargin"; when: loader.item !== null; value: ((loader.item || {}).Layout || {}).rightMargin }
    Binding { target: loader; property: "Layout.row"; when: loader.item !== null; value: ((loader.item || {}).Layout || {}).row }
    Binding { target: loader; property: "Layout.rowSpan"; when: loader.item !== null; value: ((loader.item || {}).Layout || {}).rowSpan }
    Binding { target: loader; property: "Layout.topMargin"; when: loader.item !== null; value: ((loader.item || {}).Layout || {}).topMargin }

}
