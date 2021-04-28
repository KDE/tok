Project {
    references: [
        "src/Shared.qbs",
        "src/Tok.qbs",
    ]

    property bool withTests: false

    SubProject {
        filePath: "src/Test.qbs"
        condition: project.withTests
    }
}