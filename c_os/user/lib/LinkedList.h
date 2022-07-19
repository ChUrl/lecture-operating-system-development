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

    Wrapper<T>* next = nullptr;
    Wrapper<T>* prev = nullptr;

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

    // Allow the iterator to traverse the links
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
    using WrapperType = typename Iterator::Type;

private:
    unsigned int num_elements = 0;
    WrapperType* head = nullptr;
    WrapperType* tail = nullptr;

    std::optional<WrapperType*> get_wrapper(unsigned int i) {
        WrapperType* current = head;
        unsigned int pos = 0;
        while (current != nullptr) {
            if (pos == i) {
                return current;
            }

            current = current->next;
            pos = pos + 1;
        }

        return std::nullopt;
    }

public:
    ~LinkedList() {
        WrapperType* current = head;
        WrapperType* next;

        while (current != nullptr) {
            next = current->next;
            delete current;
            current = next;
        }
    }

    Iterator begin() override {
        return Iterator(head);
    }

    Iterator end() override {
        return Iterator(tail->next);
    }

    unsigned int insert_at(Type e, unsigned int i) override {
        if (i > size()) {
            return -1;
        }

        if (i == 0) {
            return insert_first(std::forward<Type>(e));
        }

        if (i == size()) {
            return insert_last(std::forward<Type>(e));
        }

        WrapperType* old_e = get_wrapper(i).value_or(nullptr);
        if (old_e == nullptr) {
            return -1;
        }

        WrapperType* new_e = new WrapperType(e);

        new_e->prev = old_e->prev;
        new_e->next = old_e;
        new_e->prev->next = new_e;
        new_e->next->prev = new_e;

        num_elements = num_elements + 1;
        return size();
    }

    unsigned int insert_first(Type e) override {
        WrapperType* old_head = head;
        head = new WrapperType(e);

        head->prev = nullptr;
        head->next = old_head;
        if (old_head != nullptr) {
            old_head->prev = head;
        }

        if (tail == nullptr) {
            tail = head;
        }

        num_elements = num_elements + 1;
        return size();
    }

    unsigned int insert_last(Type e) override {
        WrapperType* old_tail = tail;
        tail = new WrapperType(e);

        tail->next = nullptr;
        tail->prev = old_tail;
        if (old_tail != nullptr) {
            old_tail->next = tail;
        }

        if (head == nullptr) {
            head = tail;
        }

        num_elements = num_elements + 1;
        return size();
    }

    std::optional<Type> remove_at(unsigned int i) override {
        if (empty() || i >= size()) {
            return std::nullopt;
        }

        if (i == 0) {
            return remove_first();
        }

        if (i == size() - 1) {
            return remove_last();
        }

        WrapperType* e = get_wrapper(i).value_or(nullptr);
        if (e == nullptr) {
            return std::nullopt;
        }

        Type ret = *e;

        e->next->prev = e->prev;
        e->prev->next = e->next;

        delete e;
        num_elements = num_elements - 1;

        return ret;
    }

    std::optional<Type> remove_first() override {
        if (empty()) {
            return std::nullopt;
        }

        Type e = *head;
        WrapperType* old_head = head;

        head = head->next;
        if (head != nullptr) {
            head->prev = nullptr;
        } else {
            tail = nullptr;
        }

        delete old_head;
        num_elements = num_elements - 1;

        return e;
    }

    std::optional<Type> remove_last() override {
        if (empty()) {
            return std::nullopt;
        }

        Type e = *tail;
        WrapperType* old_tail = tail;

        tail = tail->prev;
        if (tail != nullptr) {
            tail->next = nullptr;
        } else {
            head = nullptr;
        }

        delete old_tail;
        num_elements = num_elements - 1;

        return e;
    }

    bool remove(Type e) override {
        unsigned int pos = 0;
        WrapperType* wrapper = head;
        while (wrapper != nullptr) {
            if (*wrapper == e) {
                return remove_at(pos).has_value();
            }

            wrapper = wrapper->next;
            pos++;
        }

        return false;
    }

    std::optional<Type> get(unsigned int i) const override {
        if (i >= size()) {
            return std::nullopt;
        }

        if (i == 0) {
            return first();
        }
        if (i == size() - 1) {
            return last();
        }

        WrapperType* wrapper = head;
        for (unsigned int pos = 0; pos < i; ++pos) {
            wrapper = wrapper->next;
        }
        return *wrapper;
    }

    std::optional<Type> first() const override {
        if (empty()) {
            return std::nullopt;
        }

        return *head;
    }

    std::optional<Type> last() const override {
        if (empty()) {
            return std::nullopt;
        }

        return *tail;
    }

    bool empty() const override {
        return !size();
    }
    unsigned int size() const override {
        return num_elements;
    }

    void print(OutStream& out) const override {
        // Our stream cannot print all types so enable this only for debugging purposes (only int)
        if constexpr (std::is_same<Type, int>::value) {
            if (empty()) {
                out << "Print List (0 elements)" << endl;
                return;
            }

            out << "Print List (" << dec << size() << " elements): ";
            typename Iterator::Type* current = head;
            while (current != nullptr) {
                out << dec << *current << " ";
                current = current->next;
            }
            out << endl;
        }
    }
};

#endif
