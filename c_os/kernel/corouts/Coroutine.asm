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

%include "kernel/corouts/Coroutine.inc"

; EXPORTIERTE FUNKTIONEN

[GLOBAL Coroutine_switch]
[GLOBAL Coroutine_start]

; IMPLEMENTIERUNG DER FUNKTIONEN

[SECTION .text]

; COROUTINE_START : Startet die erste Coroutine ueberhaupt.
;
; C Prototyp: void Coroutine_start (struct CoroutineState* regs);

;; Coroutine_start is called with one arg: CoroutineState* regs, so the main stack looks like this:
;; == High address ==
;;      *REGS
;; SP:  RET ADDR
;; == Low address ==
Coroutine_start:
; *
; * Hier muss Code eingefuegt werden
; *

    ;; Set eax to the address where the CoroutineState struct is in memory:
    mov eax, [esp + 0x4]

    ;; Now eax points to the beginning of CoroutineState:
    ;; struct CoroutineState {
    ;; == Low address ==
    ;; EAX: void *ebx;
    ;;      void *esi;
    ;;      void *edi;
    ;;      void *ebp;
    ;;      void *esp;
    ;; == High address ==
    ;; };

    ;; Load the contents of CoroutineState to the registers
    mov esp, [eax + esp_offset]

    ;; The stackpointer now points to the coroutine stack
    ;; struct CoroutineState {
    ;; == High address ==
    ;;     *OBJECT
    ;;     0x13115
    ;; SP: *KICKOFF
    ;; == Low address ==
    ;; };

    ;; Return to kickoff
    ret


; COROUTINE_SWITCH : Coroutinenumschaltung. Der aktuelle Registersatz wird
;                    gesichert und der Registersatz der neuen Coroutine
;                    wird in den Prozessor eingelesen.
;
; C Prototyp: void Coroutine_switch (struct CoroutineState* regs_now, struct CoroutineState* reg_then);
;
; Achtung: Die Parameter werden von rechts nach links uebergeben.
;
;; == High address ==
;;        *REGS_THEN
;;        *REGS_NOW
;; SP --> RET ADDR
;; == Low address ==
Coroutine_switch:
; *
; * Hier muss Code eingefuegt werden
; *

    ;; Make eax point to beginning of regs_now in memory
    mov eax, [esp + 0x4]

    ;; Save current coroutine registers
    mov [eax + ebx_offset], ebx
    mov [eax + esi_offset], esi
    mov [eax + edi_offset], edi
    mov [eax + ebp_offset], ebp
    mov [eax + esp_offset], esp

    ;; Make eax point to beginning of regs_then in memory
    mov eax, [esp + 0x8]

    ;; Load next coroutine registers
    mov ebx, [eax + ebx_offset]
    mov esi, [eax + esi_offset]
    mov edi, [eax + edi_offset]
    mov ebp, [eax + ebp_offset]
    mov esp, [eax + esp_offset]

    ;; Stackpointer now points to kickoff address, ret jumps there
    ;; NOTE: The stackpointer only points to kickoff if the next coroutine was just initialized.
    ;;       Otherwise it just points somewhere in the next coroutines stack
    ret
