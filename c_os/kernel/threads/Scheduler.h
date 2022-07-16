/*****************************************************************************
 *                                                                           *
 *                          S C H E D U L E R                                *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Implementierung eines einfachen Zeitscheiben-Schedulers. *
 *                  Rechenbereite Threads werden in 'readQueue' verwaltet.   *
 *                                                                           *
 * Autor:           Michael, Schoettner, HHU, 22.8.2016                      *
 *****************************************************************************/

#ifndef __Scheduler_include__
#define __Scheduler_include__

#include "devices/CGA_Stream.h"
#include "kernel/threads/Dispatch.h"
#include "kernel/threads/Thread.h"
#include "lib/Queue.h"
#include "lib/SpinLock.h"
#include "user/lib/ArrayList.h"
#include "user/lib/Logger.h"

class Scheduler : public Dispatcher {
private:
    Scheduler(const Scheduler& copy) = delete;  // Verhindere Kopieren

    // Queue readyQueue;  // auf die CPU wartende Threads

    // Scheduler wird evt. von einer Unterbrechung vom Zeitgeber gerufen,
    // bevor er initialisiert wurde
    bool has_idle_thread;

    Logger log;

    // NOTE: I would have to release the lock when switching threads but I don't know exactly how to do this
    //       in the assembly function
    // SpinLock lock;  // Use spinlock instead of cpu.disable_int() because it still allows preemption
    //                 // for threads that don't use the scheduler

    // NOTE: Using this instead of the Queue is a side effect, I added the ArrayList for different reasons
    //       but my Queue was shit so I replaced it (and didn't fix the Queue)
    ArrayList<Thread*> ready_queue;

public:
    Scheduler() : has_idle_thread(false), log("SCHED") {}

    // Scheduler initialisiert?
    // Zeitgeber-Unterbrechung kommt evt. bevor der Scheduler fertig
    // intiialisiert wurde!
    bool preemption_enabled() const { return has_idle_thread; }

    // ruft nur der Idle-Thread (erster Thread der vom Scheduler gestartet wird)
    void enable_preemption() { has_idle_thread = true; }

    // Scheduler starten
    void schedule();

    // Thread in readyQueue eintragen
    void ready(Thread* that);

    // Thread terminiert sich selbst
    void exit();

    // Thread mit 'Gewalt' terminieren
    void kill(Thread* that);

    // CPU freiwillig abgeben und Auswahl des naechsten Threads
    void yield();

    // Thread umschalten; wird aus der ISR des PITs gerufen
    void preempt();

    void block();
    void deblock(Thread* that);
};

#endif
