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

#include "kernel/threads/Thread.h"
#include "lib/SpinLock.h"
#include "user/lib/Logger.h"
#include "user/lib/mem/UniquePointer.h"
#include "user/lib/Vector.h"

class Scheduler {
private:
    Scheduler(const Scheduler& copy) = delete;  // Verhindere Kopieren

    Logger log;

    // NOTE: Using this instead of the Queue is a side effect, I added the ArrayList for different reasons
    //       but my Queue was shit so I replaced it
    //       I want to use the queue again to get rid of the Vector overhead (copy + dynamic memory) but
    //       this is problematic with the unique_ptr, I guess I would have to wrap the threads in the "Chain"
    //       elements instead of a thread being a "Chain"
    bse::Vector<bse::unique_ptr<Thread>> ready_queue;
    bse::Vector<bse::unique_ptr<Thread>> block_queue;

    // NOTE: It makes sense to keep track of the active thread through this as it makes handling the
    //       unique_ptr easier and reduces the copying in the vector when cycling through the threads
    bse::Vector<bse::unique_ptr<Thread>>::Iterator active = nullptr;

    // Scheduler wird evt. von einer Unterbrechung vom Zeitgeber gerufen,
    // bevor er initialisiert wurde
    unsigned int idle_tid = 0U;

    // NOTE: I would have to release the lock when switching threads but I don't know exactly how to do this
    //       in the assembly function
    // SpinLock lock;  // Use spinlock instead of cpu.disable_int() because it still allows preemption
    //                 // for threads that don't use the scheduler

    void start(bse::Vector<bse::unique_ptr<Thread>>::Iterator next);                        // Switches from prev to current active
    void switch_to(Thread* prev_raw, bse::Vector<bse::unique_ptr<Thread>>::Iterator next);  // Switches from prev to current active

    // Kann nur vom Idle-Thread aufgerufen werden (erster Thread der vom Scheduler gestartet wird)
    void enable_preemption(unsigned int tid) { idle_tid = tid; }
    friend class IdleThread;

    void ready(bse::unique_ptr<Thread>&& thread);

public:
    Scheduler() : log("SCHED") {}

    unsigned int get_active() const {
        return (*active)->tid;
    }

    // Scheduler initialisiert?
    // Zeitgeber-Unterbrechung kommt evt. bevor der Scheduler fertig
    // intiialisiert wurde!
    bool preemption_enabled() const { return idle_tid != 0U; }

    // Scheduler starten
    void schedule();

    // Helper that directly constructs the thread, then readys it
    template<typename T, typename... Args>
    unsigned int ready(Args... args) {
        bse::unique_ptr<Thread> thread = bse::make_unique<T>(std::forward<Args>(args)...);
        unsigned int tid = thread->tid;

        ready(std::move(thread));

        return tid;
    }

    // Thread terminiert sich selbst
    // NOTE: When a thread exits itself it will disappear...
    //       Maybe put exited threads in an exited queue?
    //       Then they would have to be acquired from there to exit...
    void exit();

    // Thread mit 'Gewalt' terminieren
    void kill(unsigned int tid, bse::unique_ptr<Thread>* ptr);
    void kill(unsigned int tid) { kill(tid, nullptr); }

    // CPU freiwillig abgeben und Auswahl des naechsten Threads
    void yield();

    // Thread umschalten; wird aus der ISR des PITs gerufen
    void preempt();

    // Blocks current thread (move to block_queue)
    void block();

    // Deblock by tid (move to ready_queue)
    void deblock(unsigned int tid);
};

#endif
