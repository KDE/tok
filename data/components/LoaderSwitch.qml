import QtQuick 2.10

LoaderPlus {
    property var cases: {}
    property var defaultCase: ""
    property string value: ""

    source: {
        if (!(this.value in this.cases)) {
            return this.defaultCase
        }
        return this.cases[this.value]
    }
    onStatusChanged: if (this.status == Loader.Error) Qt.quit(1)
}