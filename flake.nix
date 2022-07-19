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
            gcc_multi # Compile with 32bit
            glibc_multi
            nasm
            binutils
            gnumake
            bear # To generate compilation database
            gdb
            qemu # Start os in virtual machine
            clang-tools_14 # Editor LSP
            # cling # To try out my bullshit implementations
          ];

          # Build inputs are for target platform, app will be linked against those
          buildInputs = with pkgs; [ ];
        };
      });
}
