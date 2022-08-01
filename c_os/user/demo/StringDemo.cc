#include "user/demo/StringDemo.h"
#include "kernel/Globals.h"
#include "user/lib/String.h"

void StringDemo::run() {
    kout.lock();
    kout.clear();

    log.info() << "Allocating new string" << endl;
    bse::string str1 = "This is a dynamically allocated string!";
    kout << str1 << endl;

    log.info() << "Reassign string" << endl;
    str1 = "Hello";
    kout << str1 << " has length " << dec << str1.size() << endl;
    kout << "Again with strlen: Hello has length " << dec << bse::strlen("Hello") << endl;

    kout << "Adding strings: " << str1 << " + World" << endl;
    log.info() << "Adding strings" << endl;
    str1 = str1 + " World";
    kout << str1 << endl;

    kout << "Hello += World" << endl;
    log.info() << "Hello += World" << endl;
    bse::string str3 = "Hello";
    str3 += " World";
    kout << str3 << endl;

    kout << "Hello World *= 3" << endl;
    str3 *= 3;
    kout << str3 << endl;

    kout << "String iterator!" << endl;
    for (const char c : str1) {
        kout << c << " ";
    }
    kout << endl;

    log.info() << "Allocating new string" << endl;
    bse::string str2 = "Hello World";
    kout << "str1 == str2: " << static_cast<int>(str1 == str2) << endl;
    kout << "strcmp(Hello, Hello): " << bse::strcmp("Hello", "Hello") << endl;

    log.info() << "Reassign str2" << endl;
    str2 = "Hello";

    bse::array<char, 5> arr{};
    arr[0] = 'H';
    arr[1] = 'e';
    arr[2] = 'l';
    arr[3] = 'l';
    arr[4] = 'o';
    kout << "bse::array<char, 5> to bse::string: " << static_cast<bse::string>(arr) << ", size: " << (bse::string(arr)).size() << endl;
    kout << "(bse::string)arr (" << static_cast<bse::string>(arr) << ") == str2 (" << str2 << "): " << static_cast<int>(bse::string(arr) == str2) << endl;

    kout.unlock();
    scheduler.exit();
}
