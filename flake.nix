{
  description = "BSEos flake for development shell";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
  inputs.flake-utils.url = "github:numtide/flake-utils";
  inputs.devshell.url = "github:numtide/devshell";

  outputs = { self, nixpkgs, flake-utils, devshell }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs {
          inherit system;
          config.allowUnfree = true; # For clion
          overlays = [ devshell.overlay ];
        };

        # bintools with multilib
        bintools_multi = pkgs.wrapBintoolsWith {
          bintools =
            pkgs.bintools.bintools; # Get the unwrapped bintools from the wrapper
          libc = pkgs.glibc_multi;
        };

        # gcc12 with multilib
        gcc12_multi = pkgs.hiPrio (pkgs.wrapCCWith {
          cc = pkgs.gcc12.cc; # Get the unwrapped gcc from the wrapper
          libc = pkgs.glibc_multi;
          bintools = bintools_multi;
        });

        # clang14 with multilib for clang-tools
        clang14_multi = pkgs.wrapCCWith {
          cc = pkgs.clang_14.cc;
          libc = pkgs.glibc_multi;
          bintools = bintools_multi;
        };
      in {
        # devShell = pkgs.devshell.mkShell ...
        devShell = pkgs.devshell.mkShell {
          name = "BSEos";

          packages = with pkgs; [
            gcc12_multi
            bintools_multi
            clang14_multi
            # clang-tools_14 # clangd + clang-format + clang-tidy

            nasm
            gnumake
            bear # To generate compilation database
            gdb
            qemu # Start os in virtual machine
            doxygen # Generate docs + graphs

            # glibc_multi # Needed for lsp to find some headers
            # clang_14 # To view template generation, also alternative error messages, conflicts with gcc

            jetbrains.clion

            # TODO: Figure out what is needed to make cling work
            # llvmPackages_14.llvm
            # cling # To try out my bullshit implementations
            # root
          ];

          # Not for devshell
          # hardeningDisable = [ "fortify" ]; # FORTIFY_SOURCE needs -O2 but we compile with -O0

          commands = [
            {
              name = "ide";
              help = "Run clion for project";
              command = "clion &>/dev/null ./ &";
            }
            {
              name = "build";
              help = "Build the OS";
              command = "make --jobs 6";
            }
            {
              name = "build-clang";
              help = "Build the OS using clang";
              command = "GCCFLAGS='' CC=clang CXX=clang++ make --jobs 6";
            }
            {
              name = "run";
              help = "Start OS in qemu";
              command = "make qemu --jobs 6";
            }
            {
              name = "clean";
              help = "Cleanup build files";
              command = "make clean";
            }
            {
              name = "generate-compile-commands";
              help = "Generate the compilation database for clangd";
              command = "make clean && GCCFLAGS='' bear -- make --jobs 6";
            }
            {
              name = "debug";
              help = "Start OS for gdb connection and run gdb";
              command = "(make clean) && (make qemu-gdb --jobs 6 &) && (make gdb)";
            }
          ];
        };
      });
}
