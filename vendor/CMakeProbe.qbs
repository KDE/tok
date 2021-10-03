// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import qbs.Process

Probe {
    id: cmakeProbe

    property string src
    property string findPackage
    property string linkPackage

    property var linkerFlags
    property var includeDirs

    configure: {
        var proc = new Process()
        var exitCode = proc.exec("bash", [cmakeProbe.src + "/extract_flags.sh",
            "find_package(" + findPackage + " REQUIRED)",
            linkPackage,
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
