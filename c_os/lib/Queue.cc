#include "Queue.h"
#include "kernel/Globals.h"
#include <cstddef>

void Queue::enqueue(Chain* item) {
    // kout << "Enqueue " << (unsigned int)item << endl;

    *this->tail = item;        // If queue is empty tail points to head pointer,
    this->tail = &item->next;  // otherwise to last next pointer
}

Chain* Queue::dequeue() {
    if (this->isEmpty()) {
        // This should not ever happen as the idle thread exists always
        // kout << "Dequeue called on empty queue." << endl;
        return NULL;
    }

    Chain* item = this->head;
    this->head = this->head->next;  // If *item was the only item this->head->next
                                    // is equal to this->tail

    if (this->isEmpty()) {
        // Reset after last element was removed
        this->head = 0;
        this->tail = &this->head;
    }

    // kout << "Dequeue " << (unsigned int)item << endl;

    return item;
}

void Queue::remove(Chain* item) {
    if (this->isEmpty()) {
        // kout << "Remove called on empty queue" << endl;
        return;
    }

    Chain* current = this->head;
    Chain* lastnext = NULL;
    while (current != item) {
        if (current->next == *this->tail) {
            // kout << "Element not in queue." << endl;
            return;
        }

        lastnext = current;
        current = current->next;
    }

    // std::cout << "Remove " << item->val << std::endl;

    if (current == this->head) {
        // current is first element
        this->head = current->next;
    } else if (current->next == *this->tail) {
        // current is last element
        this->tail = &lastnext->next;
    } else {
        // current is in the middle
        lastnext->next = current->next;
    }

    if (this->isEmpty()) {
        // Reset after last element was removed
        this->head = 0;
        this->tail = &this->head;
    }
}

void Queue::print() const {
    if (this->isEmpty()) {
        // Queue is empty
        kout << "List is empty!" << endl;
        return;
    }

    kout << "List: ";

    Chain* current = this->head;
    kout << (unsigned int)current << " ";  // At least one element in queue

    while (current->next != *this->tail) {
        // More than one element in queue
        current = current->next;
        kout << (unsigned int)current << " ";
    }

    kout << endl;
}

bool Queue::isEmpty() const {
    return *this->tail == this->head;
}
