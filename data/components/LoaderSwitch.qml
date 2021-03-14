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
}