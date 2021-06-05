// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

QtApplication {
    name: "org.kde.Tok"
    install: true
    installDir: "bin"
    files: [
        "main.cpp",
    ]
    Depends { name: "tokInternal" }
}