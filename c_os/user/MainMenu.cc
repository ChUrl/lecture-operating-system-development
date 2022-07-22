#include "user/MainMenu.h"
#include "user/demo/ArrayDemo.h"
#include "user/demo/BlueScreenDemo.h"
#include "user/demo/HeapDemo.h"
#include "user/demo/KeyboardDemo.h"
#include "user/demo/PCSPKdemo.h"
#include "user/demo/PreemptiveThreadDemo.h"
#include "user/demo/SmartPointerDemo.h"
#include "user/demo/TextDemo.h"
#include "user/demo/VBEdemo.h"
#include "user/demo/VectorDemo.h"

void print_demo_menu() {
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
}

void MainMenu::run() {
    print_demo_menu();

    char input = '\0';
    unsigned int running_demo = 0;
    while (true) {
        input = this->listener.waitForKeyEvent();

        if (running_demo == 0) {
            switch (input) {
            case '1':
                running_demo = scheduler.ready<TextDemo>();
                break;
            case '2':
                running_demo = scheduler.ready<PCSPKdemo>(&PCSPK::aerodynamic);
                break;
            case '3':
                running_demo = scheduler.ready<KeyboardDemo>();
                break;
            case '4':
                running_demo = scheduler.ready<HeapDemo>();
                break;
            case '5':
                running_demo = scheduler.ready<VBEdemo>();
                break;
            case '6':
                running_demo = scheduler.ready<BlueScreenDemo>();
                break;
            case '7':
                running_demo = scheduler.ready<PreemptiveThreadDemo>(3);
                break;

            case 'q':
                running_demo = scheduler.ready<VectorDemo>();
                break;
            case 'w':
                running_demo = scheduler.ready<ArrayDemo>();
                break;
            case 'e':
                running_demo = scheduler.ready<SmartPointerDemo>();
                break;
            }
        } else if (input == 'K') {
            scheduler.kill(running_demo);  // NOTE: If thread exits itself this will throw error
            running_demo = 0;
            print_demo_menu();
        }

        input = '\0';
    }

    scheduler.exit();
    // This thread won't be deleted...
}
