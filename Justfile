default:
  @just --list

vendor-dir:
    mkdir -p vendor/tdlib-android

download-tdlib-android: vendor-dir
    cd vendor/tdlib-android; wget -N https://core.telegram.org/tdlib/tdlib.zip
    cd vendor/tdlib-android; rm -rf libtd
    cd vendor/tdlib-android; unzip tdlib.zip
    cd vendor/tdlib-android; mv ./libtd/src/main/libs/* ./

resolve-android: download-tdlib-android
    qbs resolve profile:android -d _build/android

build-android: resolve-android
    qbs build -d _build/android