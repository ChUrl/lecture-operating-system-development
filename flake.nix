{
  description = "A basic flake with a shell";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
  inputs.flake-utils.url = "github:numtide/flake-utils";

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let pkgs = nixpkgs.legacyPackages.${system};
      in {
        devShell = pkgs.mkShell {
          # Native inputs run on host
          nativeBuildInputs = with pkgs; [
            bashInteractive
            gnumake
            gcc
            nasm
            glibc_multi
            qemu
          ];

          # Build inputs are for target platform, app will be linked against those
          buildInputs = with pkgs; [ ];
        };
      });
}
