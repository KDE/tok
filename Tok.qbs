// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

Project {
    references: [
        "src/Shared.qbs",
        "src/Tok.qbs",
        "src/the rlottie plugin/rlottie.qbs",
        "vendor/KF5ConfigWidgetsCmake.qbs",
        "vendor/KF5I18nCmake.qbs",
        "vendor/KF5Kirigami2CMake.qbs",
        "vendor/KF5NotificationsCMake.qbs",
        "vendor/KF5SyntaxHighlighting.qbs",
        "vendor/KF5WindowSystemCMake.qbs",
        "vendor/TDLibCMake.qbs",
        "app data/Appdata.qbs",
    ]

    property bool withTests: false

    SubProject {
        filePath: "src/Test.qbs"
        condition: project.withTests
    }
}