{
  description = "A simply 2D ray casting toy application";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-21.11";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in rec {
        packages = flake-utils.lib.flattenTree {
          doubleslit = pkgs.stdenv.mkDerivation {
            pname = "doubleslit";
            version = "0.0.0";
            src = nixpkgs.lib.cleanSource ./.;
            nativeBuildInputs = [
              pkgs.cmake
              pkgs.pkgconfig
            ];
            buildInputs = [
              pkgs.gtkmm3
              pkgs.cairomm
              pkgs.glm
            ];
           };
        };
        defaultPackage = packages.doubleslit;
      });
}
