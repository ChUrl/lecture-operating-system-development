#include "Queue.h"

void Queue::enqueue(Chain* item) {
    // std::cout << "Enqueue " << item->val << std::endl;

    *this->tail = item;        // If queue is empty tail points to head pointer,
    this->tail = &item->next;  // otherwise to last next pointer
}

Chain* Queue::dequeue() {
    if (this->isEmpty()) {
        // This should not ever happen as the idle thread exists always
        // std::cout << "Dequeue called on empty queue." << std::endl;
        return nullptr;
    }

    Chain* item = this->head;
    this->head = this->head->next;  // If *item was the only item this->head->next
                                    // is equal to this->tail

    if (this->isEmpty()) {
        // Reset after last element was removed
        this->head = 0;
        this->tail = &this->head;
    }

    // std::cout << "Dequeue " << item->val << std::endl;

    return item;
}

void Queue::remove(Chain* item) {
    if (this->isEmpty()) {
        // std::cout << "Remove called on empty queue" << std::endl;
        return;
    }

    Chain* current = this->head;
    Chain* lastnext = nullptr;
    while (current != item) {
        if (current->next == *this->tail) {
            // std::cout << "Element not in queue." << std::endl;
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
        // std::cout << "List is empty!" << std::endl;
        return;
    }

    // std::cout << "List: ";

    Chain* current = this->head;
    // std::cout << current->val << " "; // At least one element in queue

    while (current->next != *this->tail) {
        // More than one element in queue
        current = current->next;
        // std::cout << current->val << " ";
    }

    // std::cout << std::endl;
}

bool Queue::isEmpty() const {
    return *this->tail == this->head;
}
