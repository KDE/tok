// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import qbs.Process

StaticLibrary {
    name: "tokInternal"

    Export {
        cpp.driverLinkerFlags: ["-pthread"]
        cpp.includePaths: ["yoinked from qt automotive"]
        Group {
            files: ["../data/main.qrc"]
        }

        Depends { name: "kf5_configwidgets" }
        Depends { name: "kf5_i18n" }
        Depends { name: "kf5_kirigami2" }
        Depends { name: "kf5_notifications" }
        Depends { name: "kf5_syntaxhighlighting" }
        Depends { name: "kf5_windowsystem" }
        Depends { name: "tdlib_cmake" }

        Depends { name: "cpp" }
        Depends { name: "rlottieplugin" }
        Depends { name: "Qt"; submodules: ["widgets", "qml", "qml-private", "core-private", "quick", "quick-private", "concurrent", "multimedia"] }
        Depends { name: "icu-uc" }
    }

    cpp.includePaths: ["yoinked from qt automotive"]
    cpp.cxxLanguageVersion: "c++20"

    files: [
        "*.cpp",
        "*.h",
        "internallib/*.cpp",
        "internallib/*.h",
        "yoinked from qt automotive/*.cpp",
        "yoinked from qt automotive/*.h",
    ]
    excludeFiles: ["main.cpp", "test_main.cpp"]

    Depends { name: "kf5_configwidgets" }
    Depends { name: "kf5_i18n" }
    Depends { name: "kf5_kirigami2" }
    Depends { name: "kf5_notifications" }
    Depends { name: "kf5_syntaxhighlighting" }
    Depends { name: "kf5_windowsystem" }
    Depends { name: "tdlib_cmake" }

    Depends { name: "cpp" }
    Depends { name: "rlottieplugin" }
    Depends { name: "Qt"; submodules: ["widgets", "qml", "qml-private", "core-private", "quick", "quick-private", "concurrent", "multimedia"] }
    Depends { name: "icu-uc" }
    Depends { name: "icu-i18n" }
}
