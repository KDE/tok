# Frequently Questioned Answers

## 0: What is Telegram?

### 0.1: Telegram

Telegram is a gratis messaging service with questionable funding methods and too many features.
It is known for its performance and being OK with third-party developers making clients, of which Tok is one of.

## 1: Why Telegram?

Why not?

### 1.1: What do people like about Telegram?

It's gratis, it runs well in poor internet, and the apps (usually) aren't bad.

### 1.2: What do you use Telegram for?

Talking.

### 1.3: What do people hate about Telegram?

Spam.

Also, too complex. Nobody knows the difference between all the channel types.

And what's up with bots instead of relying on tailored UI to deliver features?

And the phone-number requirement.

The server is propietary.

## 2: What is Tok?

### 2.1: The Name

Tok comes from the word "tok" in Tok Pisin, where it means "language" or "speech."
It's also one letter removed from "toki" in *toki pona*, where it also means "language" or "speech."

### 2.2: Who is Tok for?

Anyone that wants to use it.

Except Nazis, antisemites, transphobes, homophobes, islamophobes, biphobes, and all other kinds of bigots that want to use it.
You're allowed to use it, but please fuck off, read the KDE Code of Conduct, and become a not-asshole before coming into community areas.
Thanks in advance.

Tok is not for hardcore OSS puritans, as it uses a propietary chat service.
They may have more fun with [NeoChat](https://invent.kde.org/network/neochat), which connects to free and open source servers.

### 2.3: Why does Tok exist?

Janet felt like making an app for Telegram that's good.

### 2.4: Why doesn't Tok have $FEATURE?

Janet hasn't implemented it yet.
Open an issue, and she'll get around to implementing it sometime.
Maybe someone else will contribute it.

### 2.5: What does Tok run on?

Currently, it's actively tested against:
- Fedora (desktop, mobile)
- nixpkgs

You can consider these the "officially supported platforms" of Tok.
I'll make sure that Tok always works on these, but you can run Tok on other platforms.
If there's a portability issue, please contact me in the Tok group and we can talk about it.

At some point, I intend to "officially support" Android, Windows, and macOS.

### 2.6: What does Tok not run on?

Tok doesn't run on:
- smartwatches
- smart televisions
- Plan 9

It is regrettable that Tok cannot run on these platforms, but I do not have the time to get Tok to work on them.

### 2.7: Why Qbs?

Tok's purpose in life is to be good.
Something cannot be good if the build system is bad.
Therefore, Tok uses a good build system and not a bad build system.

#### 2.7.1: Isn't Qbs dead?

No, it is not.
Qbs is alive.

#### 2.7.2: Qbs deprecation blogpost

No.
That is incorrect information.
The Qbs project is alive.

## 3: Building Tok from source

## 3.1: Dependencies

In RPM notation, Tok depends on:
- `cmake(KF5Kirigami2)`
- `cmake(KF5I18n)`
- `cmake(KF5Notifications)`
- `cmake(KF5ConfigWidgets)`
- `cmake(KF5WindowSystem)`
- `cmake(Td)`
- `pkgconfig(rlottie)`
- `pkgconfig(icu-uc)`
- `cmake(Qt5Widgets)`
- `cmake(Qt5Qml)`
- `cmake(Qt5Quick)`
- `cmake(Qt5Concurrenet)`
- `cmake(Qt5Multimedia)`
- `qbs`

## 3.2: Building

Tok uses Qbs for the build system.
This means that Tok is very simple to build.

Run

```
qbs
```

in the root directory of Tok's repo after obtaining all dependencies, and wait for a binary to be spat out.

Common options you may want to configure:
```
qbs.installPrefix:/foo/bar
```

You can change configuration options by simply adding them to an invocation of `qbs resolve` in the root of the repo.
After that, run `qbs` to build.

## 3.3: Clang?

Clang.

```
qbs setup-toolchains --detect
```

```
qbs config-ui
```

Look under profiles for the generated profile that corresponds to your version of clang.

```
qbs resolve profile:clang-XX
```

Now build.

```
qbs
```

## 10: FQA?

This is in the style of 9front's [fqa.9front.org](http://fqa.9front.org/)
