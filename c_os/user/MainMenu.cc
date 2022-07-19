#include "user/MainMenu.h"
#include "user/demo/ArrayDemo.h"
#include "user/demo/ArrayListDemo.h"
#include "user/demo/BlueScreenDemo.h"
#include "user/demo/HeapDemo.h"
#include "user/demo/KeyboardDemo.h"
#include "user/demo/LinkedListDemo.h"
#include "user/demo/PCSPKdemo.h"
#include "user/demo/PreemptiveThreadDemo.h"
#include "user/demo/SmartPointerDemo.h"
#include "user/demo/TextDemo.h"
#include "user/demo/VBEdemo.h"

void print_demo_menu() {
    kout.lock();
    kout.clear();
    kout << "Demo Menu, press number to start, K to kill:\n"
         << "1 - Text Demo\n"
         << "2 - PCSPK Demo\n"
         << "3 - Keyboard Demo\n"
         << "4 - Heap Demo\n"
         << "5 - VBE Demo\n"
         << "6 - Bluescreen Demo\n"
         << "7 - Preemption Demo\n"
         << endl;
    kout.unlock();
}

void MainMenu::run() {
    print_demo_menu();

    char input = '\0';
    Thread* choosen_demo = NULL;
    while (true) {
        input = this->listener.waitForKeyEvent();

        if (choosen_demo == NULL) {
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

            case 'q':
                choosen_demo = new ArrayListDemo();
                break;
            case 'w':
                // NOTE: The LinkedListDemo will take considerably longer for the threadswitching part
                //       because for every insertion/deletion memory will be allocated for the wrappers
                choosen_demo = new LinkedListDemo();
                break;
            case 'e':
                choosen_demo = new ArrayDemo();
                break;
            case 'r':
                choosen_demo = new SmartPointerDemo();
                break;
            }

            if (choosen_demo != NULL) {
                // We actually chose something
                scheduler.ready(choosen_demo);
            }
        } else if (input == 'K') {
            scheduler.kill(choosen_demo);  // NOTE: If thread exits itself this will throw error
            choosen_demo = NULL;
            print_demo_menu();
        }

        input = '\0';
    }

    scheduler.exit();
    // This thread won't be deleted...
}
