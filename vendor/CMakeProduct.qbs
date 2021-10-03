Product {
    property string findPackage
    property string linkPackage

    CMakeProbe {
        id: probe

        src: product.sourceDirectory
        findPackage: product.findPackage
        linkPackage: product.linkPackage
    }

    Export {
        cpp.driverLinkerFlags: probe.linkerFlags
        cpp.includePaths: probe.includeDirs

        Depends { name: "cpp" }
    }
}
