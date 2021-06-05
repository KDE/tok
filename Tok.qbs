// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

Project {
    references: [
        "src/Shared.qbs",
        "src/Tok.qbs",
        "src/the rlottie plugin/rlottie.qbs",
        "app data/Appdata.qbs",
    ]

    property bool withTests: false

    SubProject {
        filePath: "src/Test.qbs"
        condition: project.withTests
    }
}