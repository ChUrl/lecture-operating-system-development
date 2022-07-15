#include "user/MainMenu.h"
#include "user/demo/ArrayListDemo.h"
#include "user/demo/BlueScreenDemo.h"
#include "user/demo/HeapDemo.h"
#include "user/demo/KeyboardDemo.h"
#include "user/demo/PCSPKdemo.h"
#include "user/demo/PreemptiveThreadDemo.h"
#include "user/demo/TextDemo.h"
#include "user/demo/VBEdemo.h"

void MainMenu::run() {
    kout << "Main Menu" << endl;

    // Demos
    // scheduler.ready(new TextDemo());
    // scheduler.ready(new PCSPKdemo(&PCSPK::aerodynamic));
    // scheduler.ready(new KeyboardDemo());
    // scheduler.ready(new HeapDemo());
    // scheduler.ready(new VBEdemo());
    // scheduler.ready(new BlueScreenDemo());
    scheduler.ready(new PreemptiveThreadDemo(3));

    // Extra demos
    // scheduler.ready(new ArrayListDemo());

    scheduler.exit();
}
