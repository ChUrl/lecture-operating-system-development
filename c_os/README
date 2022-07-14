Makefile
======================================
I added the -std=c++17 flag to enable better constexpr support.

Clang Tidy
======================================
The compile_commands.json is used by clang-tidy.
To generate it from the makefile, use bear: "bear -- make".
There are two options that clang doesn't know: -mpreferred-stack-boundary=2 and -mmanual-endbr.
These should be removed from the compile_commands.json so clang-tidy won't throw errors for every file.
