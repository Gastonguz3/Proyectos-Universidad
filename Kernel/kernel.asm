; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TALLER System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

GDT_DESC_MEMORIA: dw 0

%include "print.mac"
;%include "defines.h"
global start

extern GDT_DESC
extern IDT_DESC
extern idt_init
extern pic_reset
extern pic_enable 
extern pic_speedClock
extern pic_lowSpeedClock
extern screen_draw_layout

extern mmu_init_kernel_dir
extern mmu_init_task_dir
extern copy_page
extern tss_init
extern tasks_screen_draw
extern tss_create_user_task

extern sched_init
extern tasks_init

%define CS_RING_0_SEL (1 << 3)
%define DS_RING_0_SEL (3 << 3)
%define page_enable 0x80000000
%define TR_initial (11 << 3)
%define TR_idle (12 << 3)
%define TASK_A_CODE_START (0x00018000)
%define TASK_B_CODE_START (0x0001A000)

BITS 16
;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
start_rm_msg db     'Iniciando kernel en Modo Real'
start_rm_len equ    $ - start_rm_msg

start_pm_msg db     'Iniciando kernel en Modo Protegido'
start_pm_len equ    $ - start_pm_msg

;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.
BITS 16
start:

    ; Deshabilito interrupciones    
    cli

    ; Cambio modo de video a 80 X 50
    mov ax, 0003h
    int 10h ; set mode 03h
    xor bx, bx
    mov ax, 1112h
    int 10h ; load 8x8 font

    ; Imprimo mensaje de bienvenida - MODO REAL
    print_text_rm start_rm_msg, start_rm_len, 0x4D, 0, 39  

    ; Habilito A20
    call A20_enable

    ; Cargo la GDT
    lgdt [GDT_DESC]

    ; Seteo el bit PE del registro CR0
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; Salto a modo protegido (far jump)
    jmp CS_RING_0_SEL:modo_protegido

BITS 32
modo_protegido:
    ; A partir de aca, todo el codigo se va a ejectutar en modo protegido
    ; Establezco selectores de segmentos DS, ES, GS, FS y SS en el segmento de datos de nivel 0
    mov ax, DS_RING_0_SEL
    mov DS, ax
    mov ES, ax
    mov GS, ax
    mov FS, ax
    mov SS, ax
    
    ; Establezco el tope y la base de la pila
    mov esp, 0x25000
    mov ebp, 0x25000

    ; Inicializo la IDT
    call idt_init
    lidt [IDT_DESC]
    
    ; Imprimo mensaje de bienvenida - MODO PROTEGIDO
    print_text_pm start_pm_msg, start_pm_len, 0x004D, 1, 39  

    ; Inicializar pantalla
    call screen_draw_layout   

    ;Habilito paginacion
    call mmu_init_kernel_dir
    mov cr3, eax

    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
   
   ;Inicializo entradas de la GDT de tareas
    call tss_init

    call sched_init

    call tasks_screen_draw
    mov ax, TR_initial
    ltr ax

    ;Inicializo las tareas
    call tasks_init

    
    
    ;Inicializo interrupciones
    call pic_reset
    call pic_enable 

    ;duplico la velocidad del clock
    mov ax, 32768
    out 0x40, al
    shr eax, 8
    out 0x40, al

    ;Habilito interrupciones
    sti

    ;main
    mov edi, cr3
    push edi
    push 0x18000
    call mmu_init_task_dir
    add esp, 4
    mov cr3, eax
   
    jmp TR_idle:0   ;la tarea idle tiene en su TSS las interrupciones habilitadas en el registro EFLAGS

    ; Ciclar infinitamente 
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"
