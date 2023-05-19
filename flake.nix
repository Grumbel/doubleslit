{
  description = "A simply 2D ray casting toy application";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-22.05";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in {
        packages = rec {
          default = doubleslit;

          doubleslit = pkgs.stdenv.mkDerivation {
            pname = "doubleslit";
            version = "0.0.0";

            src = nixpkgs.lib.cleanSource ./.;

            nativeBuildInputs = with pkgs; [
              cmake
              pkgconfig
            ];

            buildInputs = with pkgs; [
              gtkmm3
              cairomm
              glm
            ];
           };
        };
      }
    );
}
