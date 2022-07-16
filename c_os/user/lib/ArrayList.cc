#include "user/lib/ArrayList.h"
#include "kernel/Globals.h"
#include "kernel/threads/Thread.h"

// NOTE: Implement this here as we need to include globals for printing

template<typename T>
void ArrayList<T>::print(OutStream& out) const {
    if (this->buffer_pos == 0) {
        out << "Print List (0 elements)" << endl;
        return;
    }

    out << "Print List (" << dec << this->buffer_pos << " elements): ";
    for (unsigned int i = 0; i < this->buffer_pos; ++i) {
        out << dec << this->get(i) << " ";
    }
    out << endl;
}

// List all types that are used with print()
template class ArrayList<int>;
template class ArrayList<Thread*>;
