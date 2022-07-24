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
#include "user/lib/Logger.h"
#include "user/lib/mem/UniquePointer.h"
#include "user/lib/Vector.h"

class Scheduler {
private:
    NamedLogger log;

    bse::vector<bse::unique_ptr<Thread>> ready_queue;
    bse::vector<bse::unique_ptr<Thread>> block_queue;

    // NOTE: It makes sense to keep track of the active thread through this as it makes handling the
    //       unique_ptr easier and reduces the copying in the vector when cycling through the threads
    bse::vector<bse::unique_ptr<Thread>>::iterator active = nullptr;

    // Scheduler wird evt. von einer Unterbrechung vom Zeitgeber gerufen,
    // bevor er initialisiert wurde
    unsigned int idle_tid = 0U;

    // Roughly the old dispatcher functionality
    /*[[noreturn]]*/ void start(bse::vector<bse::unique_ptr<Thread>>::iterator next);                        // Start next without prev
    /*[[noreturn]]*/ void switch_to(Thread* prev_raw, bse::vector<bse::unique_ptr<Thread>>::iterator next);  // Switch from prev to next

    // Kann nur vom Idle-Thread aufgerufen werden (erster Thread der vom Scheduler gestartet wird)
    void enable_preemption(unsigned int tid) { idle_tid = tid; }
    friend class IdleThread;

    void ready(bse::unique_ptr<Thread>&& thread);

public:
    Scheduler(const Scheduler& copy) = delete;  // Verhindere Kopieren

    Scheduler() : log("SCHED"), ready_queue(true), block_queue(true) {}  // lazy queues, wait for allocator

    // The scheduler has to init the queues explicitly after the allocator is available
    void init() {
        ready_queue.reserve();
        block_queue.reserve();
    }

    unsigned int get_active() const {
        return (*active)->tid;
    }

    // Scheduler initialisiert?
    // Zeitgeber-Unterbrechung kommt evt. bevor der Scheduler fertig
    // intiialisiert wurde!
    bool preemption_enabled() const { return idle_tid != 0U; }

    // Scheduler starten
    /*[[noreturn]]*/ void schedule();

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
    /* [[noreturn]] */ void exit();  // Returns on error because we don't have exceptions

    // Thread mit 'Gewalt' terminieren
    void kill(unsigned int tid, bse::unique_ptr<Thread>* ptr);
    void kill(unsigned int tid) { kill(tid, nullptr); }

    // Asks thread to exit
    // NOTE: I had many problems with killing threads that were stuck in some semaphore
    //       or were involved in any locking mechanisms, so with this a thread can make sure
    //       to "set things right" before exiting itself (but could also be ignored)
    void nice_kill(unsigned int tid, bse::unique_ptr<Thread>* ptr);
    void nice_kill(unsigned int tid) { nice_kill(tid, nullptr); }

    // CPU freiwillig abgeben und Auswahl des naechsten Threads
    /* [[noreturn]] */ void yield();  // Returns when only the idle thread runs

    // Thread umschalten; wird aus der ISR des PITs gerufen
    /* [[noreturn]] */ void preempt();  // Returns when only the idle thread runs

    // Blocks current thread (move to block_queue)
    /* [[noreturn]] */ void block();  // Returns on error because we don't have exceptions

    // Deblock by tid (move to ready_queue)
    void deblock(unsigned int tid);
};

#endif
