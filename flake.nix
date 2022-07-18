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
            glibc_multi
            nasm
            qemu
            gdb
            bear # To generate compilation database
            clang-tools_14
            cling # To try out my bullshit implementations
          ];

          # Build inputs are for target platform, app will be linked against those
          buildInputs = with pkgs; [ ];
        };
      });
}
