// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

QtApplication {
    type: ["application", "autotest"]
    files: [
        "test_main.cpp",
    ]
    Depends { name: "tokInternal" }
}