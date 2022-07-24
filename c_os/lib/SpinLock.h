/*****************************************************************************
 *                                                                           *
 *                              S P I N L O C K                              *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Implementierung eines Spinlocks mithilfe der cmpxchg     *
 *                  Instruktion.                                             *
 *                                                                           *
 * Autor:           Michael Schoettner, 2.2.2018                             *
 *****************************************************************************/

#ifndef SpinLock_include__
#define SpinLock_include__

class SpinLock {
private:
    unsigned long lock;
    unsigned long* ptr;

public:
    SpinLock(const SpinLock& copy) = delete;  // Verhindere Kopieren

    SpinLock() : lock(0), ptr(&lock) {}

    void acquire();

    void release();
};

#endif
