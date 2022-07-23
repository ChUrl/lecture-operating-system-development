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
    ;;       1. Prev thread is running, pit interrupt triggers preemption, interrupt handler called
    ;;       2. Prev registers are pushed to prev stack after the return address
    ;;       3. Switch to next stack
    ;;       3. Registers are popped from stack, the esp now points
    ;;          to the return address (that was written to the stack when it
    ;;          was switched from)
    ;;       4. Return follows the return address to resume normal stack execution

    ;; == High address ==
    ;;        ESP_NEXT
    ;;        *ESP_PREV
    ;; SP --> RET ADDR
    ;; == Low address ==

    pusha
    pushf
    ;; == High address ==
    ;; + 0x2c ESP_NEXT
    ;; + 0x28 *ESP_PREV
    ;; + 0x24 RET ADDR
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

    mov eax, [esp + 0x28]       ; Point to *ESP_PREV (Address)
    mov [eax], esp              ; Update thread esp variable

    ;; ============================================================

    mov esp, [esp + 0x2c]        ; Move to next coroutines stack
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
