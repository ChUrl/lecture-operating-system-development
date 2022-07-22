#include "Semaphore.h"
#include "kernel/Globals.h"

void Semaphore::p() {
    // Lock to allow deterministic operations on counter/queue
    this->lock.acquire();

    if (this->counter > 0) {
        // Semaphore can be acquired
        this->counter = this->counter - 1;
        this->lock.release();
    } else {
        // Block and manage thread in semaphore queue until it's woken up by v() again
        this->wait_queue.push_back(scheduler.get_active());

        cpu.disable_int();  // Make sure the block() comes through after releasing the lock
        this->lock.release();
        scheduler.block();  // Moves to next thread, enables int
    }
}

void Semaphore::v() {
    this->lock.acquire();

    if (!this->wait_queue.empty()) {
        // Semaphore stays busy and unblocks next thread to work in critical section
        unsigned int tid = this->wait_queue.front();
        this->wait_queue.erase(wait_queue.begin());

        cpu.disable_int();  // Make sure the deblock() comes through after releasing the lock
        this->lock.release();
        scheduler.deblock(tid);  // Enables int
    } else {
        // No more threads want to work so free semaphore
        this->counter = this->counter + 1;
        this->lock.release();
    }
}
