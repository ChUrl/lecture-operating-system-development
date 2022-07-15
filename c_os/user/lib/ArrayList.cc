#include "user/lib/ArrayList.h"
#include "kernel/Globals.h"
#include "kernel/threads/Thread.h"

// NOTE: Implement this here as we need to include globals for printing

template<typename T>
void ArrayList<T>::print() const {
    if (this->buffer_pos == 0) {
        kout << "Print List (0 elements)" << endl;
        return;
    }

    kout << "Print List (" << dec << this->buffer_pos << " elements): ";
    for (unsigned int i = 0; i < this->buffer_pos; ++i) {
        kout << dec << this->get(i) << " ";
    }
    kout << endl;
}

template class ArrayList<int>;
template class ArrayList<Thread*>;
