{
  description = "Flake for Tok";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
    flakeUtils.url = "github:numtide/flake-utils";
    flakeCompat = {
      url = "github:edolstra/flake-compat";
      flake = false;
    };
    tdlibSrc = {
      url = "github:tdlib/td/master";
      flake = false;
    };
  };

  outputs = inputs:
    with inputs.flakeUtils.lib; eachSystem [ "x86_64-linux" ] (system:
      let
        pkgs = import inputs.nixpkgs {
          inherit system;
          overlays = [
            (final: prev: {
              tdlib = prev.tdlib.overrideAttrs (_: {
                version = prev.lib.substring 0 8 inputs.tdlibSrc.rev;
                src = inputs.tdlibSrc;
              });
            })
          ];
        };
        packages = {
          tok = pkgs.libsForQt5.callPackage ./build.nix {};
        };
        apps = {
          tok = mkApp {
            name = "Tok";
            drv = packages.tok;
          };
        };
      in
      {
        inherit packages apps;

        defaultPackage = packages.tok;
        defaultApp = apps.tok;
      }
    );
}
