Important
======================================
- This does not run in the latest "vanilla" C++ VM image, g++-multilib has to be installed extra

Makefile
======================================
- Added the -std=c++17 flag to enable better constexpr support.
- Removed optimizations as the system would crash with them.
  The BlueScreen would show an empty callstack on crashes with -O2, so I guess the problem is the missing ebp or something like that.
  
Changes
======================================
- Removed the Input.h/Input.cc files
- Removed the Queue.h/Queue.cc/Chain.h files
- Many changes to the scheduler (strategy is still round robin):
  - Merged dispatcher into the scheduler (removed Dispatch.h/Dispatch.cc)
  - Scheduler no longer uses queue but vector
  - Scheduler owns threads (no outside references)
  - Threads are addressed by tid from the outside
  - Retrieve threads when killing them
  - Threads are deallocated when killed and not retrieved
  - Scheduler manages blocked threads
- Simplified thread switching by storing registers on the stack (removed ThreadState.h, Thread.inc)

Additions
======================================
- Some stream manipulators for CGA_Stream (color) and OutStream (fixed width)
- Logging with serial port output
- Main menu to start demos
- A very basic system to broadcast keyboard events to threads
- Basic vector and iterator
- Basic unique_ptr
- Basic fixed size array (just a wrapper for C-style arrays)
- Basic dynamic string (heap allocated)
- Basic span

Bugs
======================================
- Can't exit the PCSPKdemo (bluescreen)
- After exiting the VBEdemo the paging isn't reactivated (so the pagefault bluescreen no longer works)
- The PreemptiveThreadDemo LoopThreads can't be exited, the demo can't be launched multiple times
- Tree Allocator (uses a red black tree to find best-fit blocks) crashes when freelist root is removed
- Can't restart the OS (bluescreen)
- Fast keyboard inputs sometimes trigger bluescreens
- Different combinations of demos also trigger bluescreens
- Other seemingly random bluescreens that are hard to pinpoint
- Key event management doesn't differentiate between press/type/release etc.
- Main menu doesn't work with multiple inputs (like holding a key)

Other
======================================
- Keyboard repeat rate and led functionality is missing
- Circle functionality in VESA mode is missing
- Singleton can't have a destructor, as C++ doesn't know how to deinitialize them after program exit.
  This happens because we compile with very limited libraries so "atexit" to register destructors is missing.
  I wanted to make the device/hardware classes (PCSPK, CPU etc) singleton because it doesn't make sense to have more than one.
