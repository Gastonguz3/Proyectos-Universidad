global temperature_asm

section .rodata
tres: times 2 dd 3  
mascara: dq 0xFFFFFFFF00FFFFFF, 0xFFFFFFFFFFFFFFFF
section .text
;void temperature_asm(unsigned char *src,   ->rdi
;              unsigned char *dst,          ->rsi
;              int width,                   ->rdx
;              int height,                  ->rcx
;              int src_row_size,            ->r8
;              int dst_row_size);           ->r9

temperature_asm:
    mov rax, rcx    ; rax = height        
    mul edx         ; rax = height(32) | height*width (32) baja 
                    ; rdx = width (32) | height*width (32) alta       
    shl rdx, 32     ; rdx = height*width (32) alta | 0 (32)     
    mov edx, eax    ; rdx = height*width (32) alta | height*width (32) baja
                    ; rdx = height*width         
    mov rcx, rdx    ; rcx = height*width 
    shr rcx, 1      ; rcx = (height*width)/2
    movdqu xmm3, [tres]     ; xxm3 = xmm3 = 0 (32) | 0 (32) | 3 (32) | 3 (32)         
    cvtdq2pd xmm3, xmm3     ; xmm3 = 3 (64) | 3 (64)    
    movdqu xmm4, [mascara]      
    xorps xmm5, xmm5        ; xmm5 = 0 (128)

    loop_temperature:
        movd xmm0, [rdi]        ; xmm0 = 0 (32) | 0 (32) | 0 (32) | pixel argb
        movd xmm1, [rdi + 4]    ; xmm1 = 0 (32) | 0 (32) | 0 (32) | pixel argb 
        add rdi, 8              ; leer los proximos 2 pixeles
        andps xmm0, xmm4        ; xmm0 = 0 (32) | 0 (32) | 0 (32) | pixel 0rgb         
        andps xmm1, xmm4        ; xmm1 = 0 (32) | 0 (32) | 0 (32) | pixel 0rgb 
        pmovzxbd xmm0, xmm0     ; xmm0 = 0 (32) | r (32) | g (32) | b (32)  -> primer pixel   
        pmovzxbd xmm1, xmm1     ; xmm1 = 0 (32) | r (32) | g (32) | b (32)  -> segundo pixel
        phaddd xmm0, xmm1       ; xmm0 = 0+r (32) | g+b (32) | 0+r (32) | g+b (32)
        phaddd xmm0, xmm5       ; xmm0 =  0 (32) | 0 (32) | 0+r+g+b (32) | 0+r+g+b (32) 
        cvtdq2pd xmm0, xmm0     ; xmm0 = 0+r+g+b (64) | 0+r+g+b (64)
        divpd xmm0, xmm3        ; xmm0 = (0+r+g+b)/3 (64) | (0+r+g+b)/3 (64)
        cvttpd2dq xmm0, xmm0    ; xmm0 = 0 (32) | 0 (32) | (0+r+g+b)/3 (32) | (0+r+g+b)/3 (32) -> truncado
        mov r11, 0
        
        comparacion:        ; T = (r+g+b)/3 
        movd r10D, xmm0     ; r10  = 0 (32) | (0+r+g+b)/3 (32)  -> primer pixel

        psrldq xmm0, 4      ; xmm0 = 0 (32) | 0 (32) | 0 (32) | (0+r+g+b)/3 (32)    -> segundo pixel
        pslldq xmm6, 4      ; para poder procesar los 2 pixeles (en la segunda iteracion pongo el pixel en la segunda posicion para poder procesar el otro)
        cmp r10B, 32d       
        jb primerCaso       ; ¿T < 32?
        cmp r10B, 96d
        jb segundoCaso      ; ¿32 =< T < 96?
        cmp r10B, 160d      
        jb tercerCaso       ; ¿96 =< T < 160?
        cmp r10B, 224d     
        jb cuartoCaso       ; ¿160 =< T < 224?
        jmp quintoCaso      ; sino

        primerCaso:         ; a = 255, r = 0, g = 0, b = 128+T*4
            inc r11
            shl r10, 2      ; T * 4
            add r10, 128    ; 128 + T * 4
            mov r8, 0xFF   

            pinsrb xmm6, r8B, 0x03  ; a = 255
            xor r8, r8
            pinsrb xmm6, r8B, 0x02  ; r = 0
            pinsrb xmm6, r8B, 0x01  ; g = 0
            pinsrb xmm6, r10B, 0x00 ; b = 128 + T * 4
            cmp r11, 2     
            jne comparacion
            jmp fin_comparacion
        segundoCaso:         ; a = 255, r = 0, g = (T-32)*4, b = 255
            inc r11
            sub r10, 32      ; T - 32
            shl r10, 2       ; (T - 32) * 4
            mov r8, 0xFF
            pinsrb xmm6, r8B, 0x03  ; a = 255
            pinsrb xmm6, r10B, 0x01 ; g = (T - 32) * 4
            pinsrb xmm6, r8B, 0x0   ; b = 255
            xor r8, r8
            pinsrb xmm6, r8B, 0x02  ; r = 0
            cmp r11, 2
            jne comparacion
            jmp fin_comparacion
        tercerCaso:         ; a = 255, r = (T-96)*4, g = 255, b = 255-(T-96)*4
            inc r11
            mov r8, 0xFF
            sub r10, 96     ; T - 96
            shl r10, 2      ; r10 = (T - 96) * 4
            sub r8, r10     ; r8 = 255 - (T - 96) * 4
            mov r9, 0xFF
            pinsrb xmm6, r9B, 0x03  ; a = 255
            pinsrb xmm6, r10B, 0x02 ; r = (T - 96) * 4
            pinsrb xmm6, r9B, 0x01  ; g = 255
            pinsrb xmm6, r8B, 0x0   ; b = 255 - (T - 96) * 4
            cmp r11, 2
            jne comparacion
            jmp fin_comparacion
        cuartoCaso:         ; a = 255, r = 255, g = 255-(T-160)*4, b = 0 
            inc r11
            mov r8, 0xFF
            sub r10, 160    ; T - 160
            shl r10, 2      ; (T - 160) * 4
            sub r8, r10     ; 255 - (T - 160) * 4
            mov r9, 0xFF
            pinsrb xmm6, r9B, 0x03  ; a = 255
            xor r10, r10
            pinsrb xmm6, r9B, 0x02  ; r = 255
            pinsrb xmm6, r8B, 0x01  ; g = 255 - (T - 160) * 4
            pinsrb xmm6, r10B, 0x0  ; b = 0
            cmp r11, 2
            jne comparacion
            jmp fin_comparacion
        quintoCaso:         ; a = 255, r = 255-(T-224)*4, g = 0, b = 0
            inc r11
            mov r8, 0xFF
            sub r10, 224    ; T - 224
            shl r10, 2      ; (T - 224) * 4
            sub r8, r10     ; 255 - (T - 224) * 4
            mov r9, 0xFF
            pinsrb xmm6, r9B, 0x03  ; a = 255
            xor r9, r9
            pinsrb xmm6, r8B, 0x02  ; r = 255 - (T - 224) * 4
            pinsrb xmm6, r9B, 0x01  ; g = 0
            pinsrb xmm6, r9B, 0x00  ; b = 0
            cmp r11, 2
            jne comparacion
            jmp fin_comparacion

    fin_comparacion:
        pshufd xmm6, xmm6, 11100001b     ;xmm6 = 0 (32) | 0 (32) | argb (32) <-> argb (32)
        pextrq [rsi], xmm6, 0       ; coloco los 2 pixeles procesados en la imagen de salida           
        add rsi, 8                  ; muevo el puntero
        sub rcx, 1
        cmp rcx, 0
        jne loop_temperature        ; repito el proceso hasta llegar al final del height

    ret
