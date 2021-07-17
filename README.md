# Tok

KDE's Telegram client, featuring a desktop and mobile version.
Explicitly not convergent.

# Building

Tok uses Qbs to build.

```
qbs
```

The binary will be output in ./default, assuming you have all dependencies.
Tok depends on the latest version of [Td](https://github.com/tdlib/td), and Qt >= 5.15, with `widgets`, `qml`, `quick`, and `concurrent` modules installed.
CMake is required in order to utilise Td.

# Development Group

Tok's development group is at https://t.me/kdetok.

# FQA

Tok has frequently questioned answers available at [FQA.md](FQA.md).

# Translating Tok

Tok uses KDE's translation infrastructure.
If you have trouble using it, please come to Tok's development group for support in translating Tok.
