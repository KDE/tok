{ mkDerivation, env, qtbase, qtdeclarative, qtmultimedia, kirigami2, ki18n
, knotifications, kconfigwidgets, kwindowsystem, kitemmodels, mauikit
, applet-window-buttons, pkg-config, qbs, cmake, tdlib, icu, zlib, rlottie
, jq, extra-cmake-modules, syntax-highlighting
}:

let
  qtEnv = env "qt-tok-${qtbase.version}" [
    qtbase qtdeclarative qtmultimedia
  ];
in mkDerivation {
  pname = "tok";
  version = "nightly";

  buildInputs = [
    icu zlib tdlib rlottie extra-cmake-modules
    qtEnv kirigami2 ki18n knotifications
    kconfigwidgets kwindowsystem kitemmodels
    mauikit applet-window-buttons syntax-highlighting
  ];

  nativeBuildInputs = [ pkg-config qbs cmake jq ];

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
