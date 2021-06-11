{ devShell
, stdenv
, wrapQtAppsHook
, makeWrapper
, libsForQt5
, qbs
, cmake
, tdlib
, icu
, zlib
, rlottie
, jq
, bash
}: stdenv.mkDerivation {
  pname = "tok";
  version = "nightly";

  buildInputs = [ icu.dev zlib tdlib rlottie ] ++ (with libsForQt5; [ full kirigami2 ki18n knotifications kconfigwidgets kwindowsystem ]);
  nativeBuildInputs = [ wrapQtAppsHook makeWrapper qbs cmake jq bash ];

  src = ./.;

  configurePhase = ''
    source ${devShell}
  '';

  buildPhase = ''
    qbs build
  '';

  installPhase = ''
    mkdir -p $out
    qbs install --install-root $out
    mv $out/usr/local/* $out

    wrapProgram $out/bin/org.kde.Tok --set LD_LIBRARY_PATH $LD_LIBRARY_PATH
  '';
}
