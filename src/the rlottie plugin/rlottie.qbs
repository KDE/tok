// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

StaticLibrary {
    name: "rlottieplugin"

    files: [
        "*.cpp",
        "*.h",
    ]

    Qt.core.pluginMetaData: ["uri=org.kde.rlottie"]

    cpp.defines: ["QT_STATICPLUGIN"]

    Depends { name: "cpp" }
    Depends { name: "rlottie" }
    Depends { name: "zlib" }
    Depends { name: "Qt"; submodules: ["core", "gui", "concurrent"] }
}
