#ifndef __LinkedLIST_INCLUDE_H_
#define __LinkedLIST_INCLUDE_H_

#include "user/lib/List.h"
#include <cstddef>

template<typename T>
class Wrapper {
private:
    T value = NULL;

public:
    Wrapper(T value) : value(value) {}

    Wrapper<T>* next = NULL;
    Wrapper<T>* prev = NULL;

    // Allow conversion to make the ListIterator dereferencing work
    operator T() {
        return value;
    }
};

// Implement linked traversal by extending the ListIterator
template<typename T>
class LinkedListIterator : public Iterator<T> {
public:
    using Type = typename Iterator<T>::Type;

    LinkedListIterator(Type* ptr) : Iterator<T>(ptr) {}

    LinkedListIterator& operator++() override {
        // ptr is of type Wrapper<T>*
        this->ptr = this->ptr->next;
        return *this;
    }
};

template<typename T>
class LinkedList : public List<T, LinkedListIterator<Wrapper<T>>> {
public:
    // Type is T
    using Type = typename List<T, LinkedListIterator<Wrapper<T>>>::Type;  // T is different from the List type (Wrapper<T>)
    using Iterator = typename List<T, LinkedListIterator<Wrapper<T>>>::Iterator;

private:
    unsigned int num_elements = 0;
    typename Iterator::Type* head = NULL;
    typename Iterator::Type* tail = NULL;

    typename Iterator::Type* get_wrapper(unsigned int i) {
        typename Iterator::Type* current = this->head;
        unsigned int pos = 0;
        while (current != NULL) {
            if (pos == i) {
                return current;
            }

            current = current->next;
            pos = pos + 1;
        }

        return NULL;
    }

protected:
    typename Iterator::Type* begin_ptr() override {
        return this->head;
    }

    typename Iterator::Type* end_ptr() override {
        return this->tail->next;
    }

public:
    unsigned int insert_at(Type e, unsigned int i) override {
        if (i > this->size()) {
            return -1;
        }

        if (i == 0) {
            return this->insert_first(e);
        }

        if (i == this->size()) {
            return this->insert_last(e);
        }

        typename Iterator::Type* old_e = this->get_wrapper(i);
        typename Iterator::Type* new_e = new typename Iterator::Type(e);

        new_e->prev = old_e->prev;
        new_e->next = old_e;
        new_e->prev->next = new_e;
        new_e->next->prev = new_e;

        this->num_elements = this->num_elements + 1;
        return this->size();
    }

    unsigned int insert_first(Type e) override {
        typename Iterator::Type* old_head = this->head;
        this->head = new typename Iterator::Type(e);

        this->head->prev = NULL;
        this->head->next = old_head;
        if (old_head != NULL) {
            old_head->prev = this->head;
        }

        if (this->tail == NULL) {
            this->tail = this->head;
        }

        this->num_elements = this->num_elements + 1;
        return this->size();
    }

    unsigned int insert_last(Type e) override {
        typename Iterator::Type* old_tail = this->tail;
        this->tail = new typename Iterator::Type(e);

        this->tail->next = NULL;
        this->tail->prev = old_tail;
        if (old_tail != NULL) {
            old_tail->next = this->tail;
        }

        if (this->head == NULL) {
            this->head = this->tail;
        }

        this->num_elements = this->num_elements + 1;
        return this->size();
    }

    Type remove_at(unsigned int i) override {
        if (this->empty() || i >= this->size()) {
            return NULL;
        }

        if (i == 0) {
            return this->remove_first();
        }

        if (i == this->size() - 1) {
            return this->remove_last();
        }

        typename Iterator::Type* e = this->get_wrapper(i);
        Type ret = *e;

        e->next->prev = e->prev;
        e->prev->next = e->next;

        delete e;
        this->num_elements = this->num_elements - 1;

        return ret;
    }

    Type remove_first() override {
        if (this->empty()) {
            return NULL;
        }

        Type e = *this->head;
        typename Iterator::Type* old_head = this->head;

        this->head = this->head->next;
        if (this->head != NULL) {
            this->head->prev = NULL;
        } else {
            this->tail = NULL;
        }

        delete old_head;
        this->num_elements = this->num_elements - 1;

        return e;
    }

    Type remove_last() override {
        if (this->empty()) {
            return NULL;
        }

        Type e = *this->tail;
        typename Iterator::Type* old_tail = this->tail;

        this->tail = this->tail->prev;
        if (this->tail != NULL) {
            this->tail->next = NULL;
        } else {
            this->head == NULL;
        }

        delete old_tail;
        this->num_elements = this->num_elements - 1;
    }

    bool remove(Type e) override {
        unsigned int pos = 0;
        typename Iterator::Type* wrapper = this->head;
        while (wrapper != NULL) {
            if (*wrapper == e) {
                return remove_at(pos);
            }

            wrapper = wrapper->next;
            pos++;
        }
    }

    Type get(unsigned int i) const override {
        if (i >= this->size()) {
            return NULL;
        }

        if (i == 0) {
            return *head;
        }
        if (i == this->size() - 1) {
            return *tail;
        }

        typename Iterator::Type* wrapper = this->head;
        for (unsigned int pos = 0; pos < i; ++pos) {
            wrapper = wrapper->next;
        }
        return *wrapper;
    }

    Type first() const override {
        if (this->empty()) {
            return NULL;
        }

        return *this->head;
    }

    Type last() const override {
        if (this->empty()) {
            return NULL;
        }

        return *this->tail;
    }

    bool empty() const override {
        return this->size() == 0;
    }
    unsigned int size() const override {
        return this->num_elements;
    }

    void print(OutStream& out) const override {
        if (this->empty()) {
            out << "Print List (0 elements)" << endl;
            return;
        }

        out << "Print List (" << dec << this->size() << " elements): ";
        typename Iterator::Type* current = this->head;
        while (current != NULL) {
            out << dec << *current << " ";
            current = current->next;
        }
        out << endl;
    }
};

#endif
