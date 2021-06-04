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
        Depends { name: "Qt"; submodules: ["widgets", "qml", "qml-private", "core-private", "quick", "concurrent"] }
        Depends { name: "icu-uc" }
    }

    Probe {
        id: mu
        property string src: product.sourceDirectory
        property var linkerFlags
        property var includeDirs
        configure: {
            var proc = new Process()
            var exitCode = proc.exec(mu.src + "/extract_flags.sh", [])
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
    Depends { name: "Qt"; submodules: ["widgets", "qml", "qml-private", "core-private", "quick", "concurrent"] }
    Depends { name: "icu-uc" }
}
