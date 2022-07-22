Makefile
======================================
- Added the -std=c++17 flag to enable better constexpr support.
- Removed optimizations as the system would crash with them.
  The BlueScreen would show an empty callstack on crashes with -O2 so I guess the problem is the missing ebp or something like that.
  
Changes
======================================
- Removed the Input.h/Input.cc files
- Made device classes singletons
- Changed the multitasking assembly (use stack instead of thread_state)
- Extensive changes to the scheduling:
  - Merged dispatcher into the scheduler
  - Scheduler no longer uses queue but vector
  - Scheduler owns threads
  - Threads are addressed by tid
  - Threads are deallocated automatically when killed/exited
  - Retrieve threads from the scheduler without deallocation
  - Scheduler manages blocked threads

Additions
======================================
- Some stream manipulators for CGA_Stream (color) and OutStream (fixed width)
- Logging class with support for colored serial port output
- Main menu to start demo
- A very basic system to broadcast keyboard events to threads
- Basic implementation of vector with iterator support
- Basic implementation of unique_ptr
- Basic implementation of fixed size array

Bugs
======================================
- Can't exit the PCSPKdemo
- Random bluescreens that are hard to pinpoint
- Tree Allocator (uses a red black tree to find best-fit blocks) crashes when freelist root is removed
