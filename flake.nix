{
  description = "Flake for Tok";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
    flakeUtils.url = "github:numtide/flake-utils";
    flakeCompat = {
      url = "github:edolstra/flake-compat";
      flake = false;
    };
    devshell.url = "github:numtide/devshell";
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
            inputs.devshell.overlay
            (final: prev: {
              tdlib = prev.tdlib.overrideAttrs (_: {
                version = prev.lib.substring 0 8 inputs.tdlibSrc.rev;
                src = inputs.tdlibSrc;
              });
            })
          ];
        };
        devShell = pkgs.devshell.fromTOML ./devshell.toml;
        packages = {
          tok = pkgs.libsForQt5.callPackage ./build.nix {
            inherit devShell;
          };
        };
        apps = {
          tok = mkApp {
            name = "Tok";
            drv = packages.tok;
          };
        };
      in
      {
        inherit devShell packages apps;

        defaultPackage = packages.tok;
        defaultApp = apps.tok;
      }
    );
}
