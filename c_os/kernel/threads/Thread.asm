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
    ;;        *ESP
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

    ;; NOTE: New code with pusha/popa
    ;; == High address ==
    ;;        *ESP_NEXT
    ;;        *ESP_PREV
    ;; SP --> RET ADDR
    ;; == Low address ==

    push eax                    ; Backup eax
    ;; == High address ==       ; Scheduler stack
    ;;        *ESP_NEXT
    ;; + 0x8  *ESP_PREV
    ;;        RET ADDR
    ;; SP --> EAX
    ;; == Low address ==

    mov eax, [esp + 0x8]
    ;; == High address ==       ; Previous thread stack (thread that was running when the interrupt came)
    ;;        OLD
    ;;        THREAD
    ;;        STACK
    ;; EAX -> RET ADDR
    ;; == Low address ==

    sub eax, 0x28
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
    ;; EAX ->
    ;; == Low address ==

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

    ;; BUG: Not the correct value
    pop eax                     ; ESP still points to the EAX we pushed before
    ;; == High address ==
    ;;        *ESP_NEXT
    ;;        *ESP_PREV
    ;; SP --> RET ADDR
    ;; == Low address ==

    mov esp, [esp + 0x4]
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
    ;; == High address ==
    ;;        OLD
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
    ;; SP -->
    ;;        ESP
    ;; == Low address ==

    pushf
    ;; == High address ==
    ;;        OLD
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
    ;;        EFLAGS
    ;; SP --> ESP
    ;; == Low address ==

    pop esp                     ; The POP ESP instruction increments the stack pointer (ESP)
                                ; before data at the old top of stack is written into the destination.
    ;; == High address ==
    ;;        *ESP_NEXT
    ;;        *ESP_PREV
    ;;        RET ADDR
    ;; SP -->
    ;; == Low address ==

    mov esp, [esp + 0xc]        ; Move to next coroutines stack
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
    ;; SP --> EDI
    ;; == Low address ==

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
