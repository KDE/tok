{ mkDerivation, env, qtbase, qtdeclarative, qtmultimedia, kirigami2, ki18n
, knotifications, kconfigwidgets, kwindowsystem, kitemmodels, mauikit
, applet-window-buttons, pkg-config, qbs, cmake, tdlib, icu, zlib, rlottie
, jq, extra-cmake-modules, syntax-highlighting, sonnet, fetchgit, fetchFromGitHub
}:

let
  qbsMaster = qbs.overrideAttrs(_: {
    version = "unstable-2021-11-23";
    src = fetchFromGitHub {
      owner = "qbs";
      repo = "qbs";
      rev = "4afb8f1f37ca4f197375b5303ff21284069852cf";
      sha256 = "sha256-EMEHQV81fx0MceG8CKulfYGyve+cOJz42C0LN9c8MuY=";
    };
  });

  ecmMaster = extra-cmake-modules.overrideAttrs(_: {
    version = "unstable-2021-10-25";
    src = fetchgit {
      url = "https://invent.kde.org/frameworks/extra-cmake-modules.git";
      rev = "8f7831c83f11f38c060fd0717b925b1fa28cd3d8";
      sha256 = "sha256-nC8arJpwXxxvLhDiCkTm2XNiXUYkbZ2VsYc4gDNZFuY=";
    };
  });

  sonnetMaster = sonnet.overrideAttrs(oldAttrs: {
    version = "unstable-2021-10-24";
    buildInputs = oldAttrs.buildInputs ++ [ ecmMaster ];
    src = fetchgit {
      url = "https://invent.kde.org/frameworks/sonnet.git";
      rev = "89f03fdce7c23f72e1225012655cc53d2b216d4e";
      sha256 = "sha256-+/NJSCppqZICErQXWg3BH40zIujJmlhjeUSjSKZA0Kc=";
    };
    patches = [];
  });

  qtEnv = env "qt-tok-${qtbase.version}" [
    qtbase qtdeclarative qtmultimedia
  ];
in mkDerivation {
  pname = "tok";
  version = "nightly";

  buildInputs = [
    icu zlib tdlib rlottie sonnetMaster
    qtEnv kirigami2 ki18n knotifications
    kconfigwidgets kwindowsystem kitemmodels
    mauikit applet-window-buttons syntax-highlighting
  ];

  nativeBuildInputs = [ pkg-config qbsMaster cmake ecmMaster jq ];

  src = ./.;

  configurePhase = ''
    qbs resolve config:release qbs.installPrefix:/
  '';

  buildPhase = ''
    qbs build config:release
  '';

  installPhase = ''
    qbs install --install-root $out config:release
  '';
}
