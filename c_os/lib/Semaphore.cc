#include "Semaphore.h"
#include "kernel/Globals.h"

void Semaphore::p() {
    // Lock to allow deterministic operations on counter
    this->lock.acquire();

    if (this->counter > 0) {
        // Semaphore can be acquired
        this->counter = this->counter - 1;
        this->lock.release();
    } else {
        // Block and manage thread in semaphore queue until it's woken up by v() again
        this->wait_queue.push_back(scheduler.get_active());
        this->lock.release();
        scheduler.block();  // Moves to next thread
    }
}

void Semaphore::v() {
    this->lock.acquire();

    if (!this->wait_queue.empty()) {
        // Semaphore stays busy and unblocks next thread to work in critical section
        if (this->wait_queue.empty()) {
            this->lock.release();
            return;
        }
        unsigned int tid = this->wait_queue.front();
        this->wait_queue.erase(wait_queue.begin());
        this->lock.release();
        scheduler.deblock(tid);
    } else {
        // No more threads want to work so free semaphore
        this->counter = this->counter + 1;
        this->lock.release();
    }
}
