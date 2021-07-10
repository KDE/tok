{ mkDerivation, env, qtbase, qtdeclarative, qtmultimedia, kirigami2, ki18n
, knotifications, kconfigwidgets, kwindowsystem, kitemmodels, mauikit
, applet-window-buttons, pkg-config, qbs, cmake, tdlib, icu, zlib, rlottie
, jq
}:

let
  qtEnv = env "qt-tok-${qtbase.version}" [
    qtbase qtdeclarative qtmultimedia
  ];
in mkDerivation {
  pname = "tok";
  version = "nightly";

  buildInputs = [
    icu zlib tdlib rlottie 
    qtEnv kirigami2 ki18n knotifications
    kconfigwidgets kwindowsystem kitemmodels
    mauikit applet-window-buttons
  ];

  nativeBuildInputs = [ pkg-config qbs cmake jq ];

  src = ./.;

  configurePhase = ''
    qbs resolve qbs.installPrefix:/
  '';

  buildPhase = ''
    qbs build
  '';

  installPhase = ''
    qbs install --install-root $out
  '';
}
