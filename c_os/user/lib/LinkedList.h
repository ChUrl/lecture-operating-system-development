#ifndef __LinkedLIST_INCLUDE_H_
#define __LinkedLIST_INCLUDE_H_

#include "user/lib/List.h"
#include <cstddef>

template<typename T>
class Wrapper {
public:
    Wrapper(T value) : value(value) {}

    T value;
    Wrapper<T>* next;
    Wrapper<T>* prev;

    // Allow implicit conversion to make the ListIterator dereferencing work
    operator T() {
        return value;
    }
};

// Implement linked traversal by extending the ListIterator
template<typename T>
class LinkedListIterator : public ListIterator<T> {
public:
    LinkedListIterator& operator++() override {
        // ptr is of type Wrapper<T>*
        this->ptr = this->ptr->next;
        return *this;
    }
};

template<typename T>
class LinkedList : public List<Wrapper<T>, LinkedListIterator<Wrapper<T>>> {
public:
    using Type = typename List<Wrapper<T>, LinkedListIterator<Wrapper<T>>>::Type;  // T is different from the List type (Wrapper<T>)
                                                                                   // so take the type out of the base class
    using Iterator = typename List<Wrapper<T>, LinkedListIterator<Wrapper<T>>>::Iterator;

private:
    Type* head;
    Type* tail;

protected:
    Type* begin_ptr() override {
        return this->head;
    }

    Type* end_ptr() override {
        return this->tail->next;
    }

public:
    LinkedList() : head(NULL), tail(NULL) {}

    unsigned int insert(Type e) override {}
    unsigned int insert_at(Type e, unsigned int i) override {}

    Type remove_at(unsigned int i) override {}
    Type remove_first() override {}
    Type remove_last() override {}
    bool remove(Type e) override {}

    Type get(unsigned int i) const override {}
    Type first() const override {}
    Type last() const override {}

    bool empty() const override {}
    unsigned int size() const override {}

    void print(OutStream& out) const override {}
};

#endif
