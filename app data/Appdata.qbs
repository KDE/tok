Product {
    name: "appdata"

    Group {
        files: ["icons/48/org.kde.Tok.svg"]
        qbs.install: true
        qbs.installDir: "share/icons/hicolor/scalable/apps"
    }
    Group {
        files: ["org.kde.Tok.notifyrc"]
        qbs.install: true
        qbs.installDir: "share/knotifications5"
    }
    Group {
        files: ["org.kde.Tok.desktop"]
        qbs.install: true
        qbs.installDir: "share/applications"
    }
}