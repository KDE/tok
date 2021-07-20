// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import qbs.Process

StaticLibrary {
    name: "tokInternal"

    Export {
        cpp.driverLinkerFlags: mu.linkerFlags.concat(["-pthread"])
        cpp.includePaths: mu.includeDirs.concat(["yoinked from qt automotive"])
        Group {
            files: ["../data/main.qrc"]
        }
        Depends { name: "cpp" }
        Depends { name: "rlottieplugin" }
        Depends { name: "Qt"; submodules: ["widgets", "qml", "qml-private", "core-private", "quick", "quick-private", "concurrent", "multimedia"] }
        Depends { name: "icu-uc" }
    }

    Probe {
        id: mu
        property string src: product.sourceDirectory
        property var linkerFlags
        property var includeDirs
        configure: {
            var proc = new Process()
            var exitCode = proc.exec("bash", [mu.src + "/extract_flags.sh",
                "find_package(KF5Kirigami2 REQUIRED)\n"+
                "find_package(KF5I18n REQUIRED)\n"+
                "find_package(KF5Notifications REQUIRED)\n"+
                "find_package(KF5ConfigWidgets REQUIRED)\n"+
                "find_package(KF5WindowSystem REQUIRED)\n"+
                "find_package(KF5Wayland REQUIRED)\n"+
                "find_package(Td REQUIRED)\n",

                "KF5::Kirigami2 KF5::I18n KF5::Notifications KF5::ConfigWidgets KF5::WindowSystem KF5::WaylandClient Td::TdStatic",
            ])
            if (exitCode != 0) {
            	console.error(proc.readStdOut())
            	throw "extracting flags from CMake libraries failed"
            }
            var stdout = proc.readStdOut()
            stdout = stdout.split("====")
            linkerFlags = stdout[0].split("\n").filter(function(it) { return Boolean(it) && !it.contains("rpath") && (it.startsWith("/") || it.startsWith("-l")) }).map(function(it) { return it.replace("-Wl,", "") })
            includeDirs = stdout[1].split("\n").filter(function(it) { return Boolean(it) && !it.contains("rpath") && (it.startsWith("/") || it.startsWith("-l")) }).map(function(it) { return it.replace("-Wl,", "") })
        }
    }
    cpp.driverLinkerFlags: mu.linkerFlags
    cpp.includePaths: mu.includeDirs.concat(["yoinked from qt automotive"])
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

    Depends { name: "cpp" }
    Depends { name: "rlottieplugin" }
    Depends { name: "Qt"; submodules: ["widgets", "qml", "qml-private", "core-private", "quick", "quick-private", "concurrent", "multimedia"] }
    Depends { name: "icu-uc" }
}
