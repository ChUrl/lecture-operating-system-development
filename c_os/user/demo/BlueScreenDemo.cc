#include "user/demo/BlueScreenDemo.h"
#include "kernel/Paging.h"

void BlueScreenDemo::run() {
    kout << "Trigger Bluescreen, if you can read this it didn't work" << endl;

    // BlueScreen 1
    // asm("int $3");

    // BlueScreen 2
    unsigned int* page = pg_alloc_page();
    *page = 42;
    pg_write_protect_page(page);
    invalidate_tlb_entry(page); // If we don't invalidate after first access the write protection
                                // won't work as no lookup is performed (address in tlb)
    *page = 42; // We map logical to physical 1:1 so no need to do any lookup
                // If tlb is invalidated this access produces a pagefault

    scheduler.exit();
}
