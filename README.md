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

# Bug Reports

Due to manpower limitations and particularly volatile dependencies, Tok cannot promise to run on the same
amount of distros as other KDE software. The set of distros bug reports are accepted from is limited to
the list outlined in the [Frequently Questioned Answers](FQA.md#user-content-25-what-does-tok-run-on) section,
as I don't have the time to make sure other distros are okiedokie in regards to what they do to stuff Tok uses.

# Development Group

Tok's development group is at https://t.me/kdetok.

# FQA

Tok has frequently questioned answers available at [FQA.md](FQA.md).

# Translating Tok

Tok uses KDE's translation infrastructure.
If you have trouble using it, please come to Tok's development group for support in translating Tok.
