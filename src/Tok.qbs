QtApplication {
    name: "org.kde.Tok"
    install: true
    installDir: "bin"
    files: [
        "main.cpp",
    ]
    Depends { name: "tokInternal" }
}