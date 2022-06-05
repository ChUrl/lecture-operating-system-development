#include "Queue.h"
#include <cstddef>
// #include <iostream>

void Queue::enqueue(Chain* item) {
    // std::cout << "Enqueue " << item->val << std::endl;

    *this->tail = item;        // If queue is empty tail points to head pointer,
    this->tail = &item->next;  // otherwise to last next pointer
}

Chain* Queue::dequeue() {
    if (*this->tail == this->head) {
        // This should not ever happen as the idle thread exists always
        // std::cout << "Dequeue called on empty queue." << std::endl;
        return NULL;
    }

    Chain* item = this->head;
    this->head = this->head->next;  // If *item was the only item this->head->next
                                    // is equal to this->tail

    if (*this->tail == this->head) {
        // Reset after last element was removed
        this->head = 0;
        this->tail = &this->head;
    }

    // std::cout << "Dequeue " << item->val << std::endl;

    return item;
}

void Queue::remove(Chain* item) {}

/*
void Queue::print() const {
    if (*this->tail == this->head) {
        // Queue is empty
        // std::cout << "List is empty!" << std::endl;
        return;
    }

    // std::cout << "List: ";

    Chain* current = this->head;
    // std::cout << current->val << " ";  // At least one element in queue

    while (&current->next != this->tail) {
        // More than one element in queue
        current = current->next;
        // std::cout << current->val << " ";
    }

    // std::cout << std::endl;
}
*/
