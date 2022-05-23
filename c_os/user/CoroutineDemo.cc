/*****************************************************************************
 *                                                                           *
 *                      C O R O U T I N E D E M O                            *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Einstieg in eine Anwendung.                              *
 *                                                                           *
 * Autor:           Michael Schoettner, HHU, 15.8.2016                       *
 *****************************************************************************/

#include "user/CoroutineDemo.h"
#include "kernel/Globals.h"
#include "user/CoroutineLoop.h"

/*****************************************************************************
 * Methode:         CoroutineDemo::main                                      *
 *---------------------------------------------------------------------------*
 * Beschreibung:    main-Methode der Anwendung.                              *
 *****************************************************************************/
void CoroutineDemo::main() {

    /* 
     * Hier muss Code eingefuegt werden 
     *
     * Die 3 Koroutinen einrichten, verketten und die 1. starten
     *
     */
    unsigned int(*stack)[1024] = new unsigned int[3][1024];  // No delete since it is never returned

    CoroutineLoop corout1(stack[0] + 1024, 0);
    CoroutineLoop corout2(stack[1] + 1024, 1);
    CoroutineLoop corout3(stack[2] + 1024, 2);

    corout1.setNext(&corout2);
    corout2.setNext(&corout3);
    corout3.setNext(&corout1);

    corout1.start();
}
