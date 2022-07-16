#include "user/MainMenu.h"
#include "user/demo/ArrayListDemo.h"
#include "user/demo/BlueScreenDemo.h"
#include "user/demo/HeapDemo.h"
#include "user/demo/KeyboardDemo.h"
#include "user/demo/PCSPKdemo.h"
#include "user/demo/PreemptiveThreadDemo.h"
#include "user/demo/TextDemo.h"
#include "user/demo/VBEdemo.h"

void print_demo_menu() {
    kout << "Demo Menu, press number to start, k to kill:\n"
         << "1 - Text Demo\n"
         << "2 - PCSPK Demo\n"
         << "3 - Keyboard Demo\n"
         << "4 - Heap Demo\n"
         << "5 - VBE Demo\n"
         << "6 - Bluescreen Demo\n"
         << "7 - Preemption Demo\n"
         << endl;
}

void MainMenu::run() {
    kout.clear();
    print_demo_menu();

    char input = '\0';
    Thread* choosen_demo = NULL;
    while (true) {

        input = this->listener.waitForKeyEvent();

        switch (input) {
        case '1':
            choosen_demo = new TextDemo();
            break;
        case '2':
            choosen_demo = new PCSPKdemo(&PCSPK::aerodynamic);
            break;
        case '3':
            choosen_demo = new KeyboardDemo();
            break;
        case '4':
            choosen_demo = new HeapDemo();
            break;
        case '5':
            choosen_demo = new VBEdemo();
            break;
        case '6':
            choosen_demo = new BlueScreenDemo();
            break;
        case '7':
            choosen_demo = new PreemptiveThreadDemo(3);
            break;
        case 'k':
            if (choosen_demo != NULL) {
                scheduler.kill(choosen_demo);
                delete choosen_demo;
                choosen_demo = NULL;
            }
            kout.clear();
            print_demo_menu();
            break;
        }

        if (choosen_demo != NULL) {
            scheduler.ready(choosen_demo);
        }
    }

    // Extra demos
    // scheduler.ready(new ArrayListDemo());

    scheduler.exit();
    // This thread won't be deleted...
}
