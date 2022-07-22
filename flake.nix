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
            glibc_multi # Needed for lsp to find some headers (can cause compilation errors when trying to
                        # compile some file standalone)
            nasm
            binutils

            gnumake
            bear # To generate compilation database
            gdb
            qemu # Start os in virtual machine

            clang-tools_14 # clangd + clang-format + clang-tidy, the counterparts included in clang_14 package
                           # don't function correctly somehow (don't find headers, probably some conflict between
                           # nix clang and gcc environments)
            clang_14 # To view template generation, also alternative error messages

            # TODO: Figure out what is needed to make cling work
            # llvmPackages_14.llvm
            # cling # To try out my bullshit implementations
            # root
          ];

          # Build inputs are for target platform, app will be linked against those
          buildInputs = with pkgs; [ ];

          # shellHook = ''
          #   alias makeg="CC=gcc CXX=g++ make -j 8"
          #   alias makec="CC=clang CXX=clang++ make -j 8"
          # '';
        };
      });
}
