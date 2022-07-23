Makefile
======================================
- Added the -std=c++17 flag to enable better constexpr support.
- Removed optimizations as the system would crash with them.
  The BlueScreen would show an empty callstack on crashes with -O2 so I guess the problem is the missing ebp or something like that.
- Added qemu-stdio target for easy access to the serial output
  
Changes
======================================
- Removed the Input.h/Input.cc files
- Made device classes singletons
- Changed the multitasking assembly (use stack instead of thread_state)
- Many changes to the scheduler (strategy is still round robin):
  - Merged dispatcher into the scheduler
  - Scheduler no longer uses queue but vector
  - Scheduler owns threads (no outside references)
  - Threads are addressed by tid from the outside
  - Threads are deallocated automatically when killed/exited
  - Retrieve threads from the scheduler without deallocation
  - Scheduler manages blocked threads

Additions
======================================
- Some stream manipulators for CGA_Stream (color) and OutStream (fixed width)
- Logging class with support for colored serial port output
- Main menu to start demos
- A very basic system to broadcast keyboard events to threads
- Basic implementation of vector with iterator support
- Basic implementation of unique_ptr
- Basic implementation of fixed size array (just a wrapper for C-style arrays)

Bugs
======================================
- Can't exit the PCSPKdemo (bluescreen)
- After exiting the VBEdemo the paging isn't reactivated (so the pagefault bluescreen no longer works)
- The PreemptiveThreadDemo LoopThreads can't be exited, the demo can't be launched multiple times
- Random bluescreens that are hard to pinpoint
- Tree Allocator (uses a red black tree to find best-fit blocks) crashes when freelist root is removed
