// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

Product {
    name: "tdlib_android"

    Export {
        Qt.android_support.extraLibs: [
            product.sourceDirectory+"/tdlib-android/arm64-v8a/libtdjni.so",
            product.sourceDirectory+"/tdlib-android/armeabi-v7a/libtdjni.so",
            product.sourceDirectory+"/tdlib-android/x86/libtdjni.so",
            product.sourceDirectory+"/tdlib-android/x86_64/libtdjni.so",
        ]
        Depends { name: "Qt.android_support" }
    }
}