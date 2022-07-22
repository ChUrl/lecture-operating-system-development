#include "kernel/Globals.h"
#include "user/lib/Vector.h"

void print(const bse::Vector<int>& vec) {
    kout << "Print: ";
    for (const int i : vec) {
        kout << i << " ";
    }
    kout << endl;
}

void run_test() {
    bse::Vector<int> vec;
    constexpr const int count = 10;
    bool success = true;

    // =================================

    kout.clear();
    kout << "Vector Test" << endl;

    // =================================

    kout << "Basic push_back" << endl;
    for (int i = 0; i < count; ++i) {
        vec.push_back(i);
    }
    for (unsigned int i = 0; i < count; ++i) {
        success = success && (vec[i] == i);
    }
    kout << "Success: " << static_cast<int>(success) << endl;
    success = true;

    // =================================

    kout << "Basic iteration" << endl;
    int current = 0;
    for (int i : vec) {
        success = success && i == current;
        current++;
    }
    kout << "Success: " << static_cast<int>(success) << endl;
    success = true;

    // =================================

    kout << "Basic functions" << endl;
    kout << "Size: " << vec.size() << endl;
    kout << "Empty: " << static_cast<int>(vec.empty()) << endl;

    // =================================

    kout << "Basic deletion" << endl;
    bse::Vector<int>::Iterator it = vec.begin();
    while (it != vec.end()) {
        it = vec.erase(it);
    }
    kout << "Size: " << vec.size() << endl;
    kout << "Empty: " << static_cast<int>(vec.empty()) << endl;

    // =================================

    kout << "Index insertion in order" << endl;
    for (int i = 0; i < count; ++i) {
        vec.insert(vec.begin() + i, i);
    }
    for (unsigned int i = 0; i < count; ++i) {
        success = success && (vec[i] == i);
    }
    kout << "Success: " << static_cast<int>(success) << endl;
    success = true;
    vec.clear();

    // =================================

    kout << "Index insertion at front" << endl;
    for (int i = 0; i < count; ++i) {
        vec.insert(vec.begin(), i);
    }
    // print(vec);
    for (unsigned int i = 0; i < count; ++i) {
        // kout << vec[i] << " == " << count - i - 1 << "?" << endl;
        success = success && (vec[i] == count - i - 1);
    }
    kout << "Success: " << static_cast<int>(success) << endl;
    success = true;
    vec.clear();

    // =================================

    kout << "Index insertion in back" << endl;
    for (int i = 0; i < count; ++i) {
        vec.insert(vec.end(), i);
    }
    // print(vec);
    for (unsigned int i = 0; i < count; ++i) {
        // kout << vec[i] << " == " << i << "?" << endl;
        success = success && (vec[i] == i);
    }
    kout << "Success: " << static_cast<int>(success) << endl;
    success = true;

    // =================================
    // 1 2 4 5 6 7 8

    kout << "Index removal" << endl;
    bse::Vector<int>::Iterator ret = vec.erase(vec.begin());
    success = success && (ret == vec.begin());
    ret = vec.erase(vec.end() - 1);
    success = success && (ret == vec.end());
    ret = vec.erase(vec.begin() + 2);
    success = success && (vec.size() == count - 3);
    success = success && (vec[0] == 1);
    success = success && (vec[1] == 2);
    success = success && (vec[2] == 4);
    success = success && (*(vec.end() - 1) == 8);
    kout << "Success: " << static_cast<int>(success) << endl;
    success = true;
}
