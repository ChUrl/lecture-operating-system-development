;*****************************************************************************
;*                                                                           *
;*                            C O R O U T I N E                              *
;*                                                                           *
;*---------------------------------------------------------------------------*
;* Beschreibung:    Assemblerdarstellung der 'struct CoroutineState' aus     *
;*                  CoroutineState.h                                         *
;*                                                                           *
;*                  Die Reihenfolge der Registerbezeichnungen muss unbedingt *
;*                  mit der von 'struct CoroutineState' uebereinstimmen.     *
;*                                                                           *
;* Autor:           Olaf Spinczyk, TU Dortmund                               *
;*****************************************************************************

; EXPORTIERTE FUNKTIONEN

[GLOBAL Thread_switch]
[GLOBAL Thread_start]

; IMPLEMENTIERUNG DER FUNKTIONEN

[SECTION .text]

Thread_start:
; *
; * Hier muss Code eingefuegt werden
; *

    ;; NOTE: New code with pusha/popa, restores all registers as I use this not only for first start
    ;; == High address ==
    ;;        ESP
    ;; SP --> RET ADDR
    ;; == Low address ==

    mov esp, [esp + 0x4]
    ;; == High address ==
    ;;        *OBJECT
    ;;        0x13115
    ;;        *KICKOFF
    ;;        EAX
    ;;        ECX
    ;;        EDX
    ;;        EBX
    ;;        ESP
    ;;        EBP
    ;;        ESI
    ;;        EDI
    ;; SP --> EFLAGS
    ;; == Low address ==

    popf
    ;; == High address ==
    ;;        *OBJECT
    ;;        0x13115
    ;;        *KICKOFF
    ;;        EAX
    ;;        ECX
    ;;        EDX
    ;;        EBX
    ;;        ESP
    ;;        EBP
    ;;        ESI
    ;; SP --> EDI
    ;; == Low address ==

    popa
    ;; == High address ==
    ;;        *OBJECT
    ;;        0x13115
    ;; SP --> *KICKOFF
    ;; == Low address ==

    sti
    ret


Thread_switch:
; *
; * Hier muss Code eingefuegt werden
; *

    ;; NOTE: The thread switching works like this:
    ;;       1. Prev thread is running, pit interrupt triggers preemption
    ;;       2. CPU switches to interrupt/scheduler stack and calls Thread_switch,
    ;;          two arguments and the return address are pushed before the call,
    ;;          the return address leads back to the prev thread
    ;;       3. Prev registers are pushed to prev stack
    ;;       4. Next registers are popped from next stack, the esp now points
    ;;          to the return address it pointed to when the next thread was
    ;;          exited in the past
    ;;       5. Return follows the return address to the next stack

    ;; NOTE: New code with pusha/popa
    ;;       1. Backup the current esp to the prev stack
    ;;       2. Switch to the prev stack and pusha, pushf
    ;;       3. Update the prev stack esp so popf, popa works on
    ;;          the correct address
    ;;       4. Jump back to the scheduler stack with the esp from 1.
    ;;       5. Jump to the next stack and popf, popa

    push eax                    ; Backup eax because we use it later to temporarily store the esp to jump back to
                                ; the interrupt stack
    ;; == High address ==       ; Scheduler stack
    ;;        ESP_NEXT
    ;; + 0x8  *ESP_PREV
    ;;        RET ADDR
    ;; SP --> EAX
    ;; == Low address ==

    mov eax, [esp + 0x8]        ; Could probably leave this out as this value already is in eax because it' the first
                                ; function parameter

    mov eax, [eax]              ; EAX points to the address of the thread's eax beacause we called with a pointer
    ;; == High address ==       ; Previous thread stack (thread that was running when the interrupt came)
    ;;        OLD
    ;;        THREAD
    ;;        STACK
    ;; EAX -> RET ADDR
    ;; == Low address ==

    sub eax, 0x28
    mov [eax], esp              ; Current esp to old thread stack, 0x24 is the amount pusha, pushf change the esp
                                ; We save it, push the current registers to the old threads stack, return and restore
    ;; == High address ==
    ;;        OLD
    ;;        THREAD
    ;;        STACK
    ;;   0x0  RET ADDR
    ;;   0x4
    ;;   0x8
    ;;   0xc
    ;;   0x10
    ;;   0x14
    ;;   0x18
    ;;   0x1c
    ;;   0x20
    ;;   0x24
    ;; EAX -> ESP (Points not to RET ADDR but the EAX we pushed as backup!!!)
    ;; == Low address ==

    pop eax                     ; ESP still points to the EAX we pushed before
    ;; == High address ==
    ;;        ESP_NEXT
    ;;        *ESP_PREV
    ;; SP --> RET ADDR
    ;; == Low address ==

    mov esp, [esp + 0x4]
    mov esp, [esp]
    ;; == High address ==
    ;;        OLD
    ;;        THREAD
    ;;        STACK
    ;; SP --> RET ADDR
    ;;   0x4
    ;;   0x8
    ;;   0xc
    ;;   0x10
    ;;   0x14
    ;;   0x18
    ;;   0x1c
    ;;   0x20
    ;;   0x24
    ;;   0x28 ESP
    ;; == Low address ==

    pusha                       ; Save current registers to stack
    pushf
    ;; == High address ==
    ;;        OLD
    ;;        THREAD
    ;;        STACK
    ;;        RET ADDR
    ;; 0x4    EAX
    ;; 0x8    ECX
    ;; 0xc    EDX
    ;; 0x10   EBX
    ;; 0x14   ESP
    ;; 0x18   EBP
    ;; 0x1c   ESI
    ;; 0x20   EDI
    ;; SP --> EFLAGS
    ;; 0x28   ESP
    ;; == Low address ==

    sub esp, 0x4
    pop esp                     ; The POP ESP instruction increments the stack pointer (ESP)
    add esp, 0x4                ; before data at the old top of stack is written into the destination.
    ;; == High address ==
    ;;        ESP_NEXT
    ;;        *ESP_PREV
    ;; SP --> RET ADDR
    ;; == Low address ==

    mov eax, [esp + 0x4]        ; EAX is backed up and free to use now
    mov ebx, eax                ; Duplicate because we dereference eax later
    mov eax, [eax]              ; EAX points to prev stack
    sub eax, 0x24               ; EAX points to EFLAGS that we pushf'd on prev stack
    ;; == High address ==
    ;;        OLD
    ;;        THREAD
    ;;        STACK
    ;;        RET ADDR
    ;; 0x4    EAX
    ;; 0x8    ECX
    ;; 0xc    EDX
    ;; 0x10   EBX
    ;; 0x14   ESP
    ;; 0x18   EBP
    ;; 0x1c   ESI
    ;; 0x20   EDI
    ;; EAX -> EFLAGS
    ;; == Low address ==

    mov [ebx], eax              ; Change prev thread's esp to point to the new bottom of stack
                                ; so popf, popa works when prev thread becomes active again

    ;; ============================================================

    mov esp, [esp + 0x8]        ; Move to next coroutines stack
    ;; == High address ==
    ;;        NEW
    ;;        THREAD
    ;;        STACK
    ;;        RET ADDR
    ;;        EAX
    ;;        ECX
    ;;        EDX
    ;;        EBX
    ;;        ESP
    ;;        EBP
    ;;        ESI
    ;;        EDI
    ;; SP --> EFLAGS
    ;; == Low address ==

    popf                        ; Load new registers from stack
    popa
    ;; == High address ==
    ;;        NEW
    ;;        THREAD
    ;;        STACK
    ;; SP --> RET ADDR
    ;; == Low address ==

    ;; Enable interrupts again
    sti
    ret
