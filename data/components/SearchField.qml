import QtQuick 2.15
import QtQuick.Controls 2.15 as QQC2
import org.kde.kirigami 2.15 as Kirigami

Kirigami.ActionTextField {
    id: control

    placeholderText: i18nc("placeholder text for a search field", "Search…")

    rightActions: [
        Kirigami.Action {
            icon.name: "edit-clear-locationbar" + (control.mirrored ? "rtl" : "ltr")
            visible: control.text.length > 0
            onTriggered: control.clear()
        }
    ]
}
