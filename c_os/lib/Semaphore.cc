#include "Semaphore.h"
#include "kernel/Globals.h"

void Semaphore::p() {
    // Lock to allow deterministic operations on counter/queue
    lock.acquire();

    if (counter > 0) {
        // Semaphore can be acquired
        counter = counter - 1;
        lock.release();
    } else {
        // Block and manage thread in semaphore queue until it's woken up by v() again
        if (!wait_queue.initialized()) {  // TODO: I will replace this suboptimal datastructure in the future
            wait_queue.reserve();
        }
        wait_queue.push_back(scheduler.get_active());

        CPU::disable_int();  // Make sure the block() comes through after releasing the lock
        lock.release();
        scheduler.block();  // Moves to next thread, enables int
    }
}

void Semaphore::v() {
    lock.acquire();

    if (!wait_queue.empty()) {
        // Semaphore stays busy and unblocks next thread to work in critical section
        unsigned int tid = wait_queue.front();
        wait_queue.erase(wait_queue.begin());

        CPU::disable_int();  // Make sure the deblock() comes through after releasing the lock
        lock.release();
        scheduler.deblock(tid);  // Enables int
    } else {
        // No more threads want to work so free semaphore
        counter = counter + 1;
        lock.release();
    }
}
