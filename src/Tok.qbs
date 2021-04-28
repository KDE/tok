QtApplication {
    install: true
    installDir: "bin"
    files: [
        "main.cpp",
    ]
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
    Depends { name: "tokInternal" }
}