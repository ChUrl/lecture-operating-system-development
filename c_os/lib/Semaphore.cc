#include "Semaphore.h"
#include "kernel/Globals.h"

void Semaphore::p() {
    // Lock to allow deterministic operations on counter
    this->lock.acquire();

    if (this->counter > 0) {
        // Semaphore can be acquired
        this->counter = this->counter - 1;
    } else {
        // Block and manage thread in semaphore queue until it's woken up by v()
        this->waitQueue.enqueue(scheduler.get_active());
        scheduler.block();
    }

    this->lock.release();
}

void Semaphore::v() {
    this->lock.acquire();

    if (!this->waitQueue.isEmpty()) {
        // Semaphore stays busy and unblocks next thread to work in critical section
        scheduler.deblock((Thread*)this->waitQueue.dequeue());
    } else {
        // No more threads want to work so free semaphore
        this->counter = this->counter + 1;
    }

    this->lock.release();
}
