;
; The MIT License (MIT)
;
; Copyright (c) 2016-2023 The SKA organisation
;
; Permission is hereby granted, free of charge, to any person obtaining a copy
; of this software and associated documentation files (the "Software"), to deal
; in the Software without restriction, including without limitation the rights
; to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
; copies of the Software, and to permit persons to whom the Software is
; furnished to do so, subject to the following conditions:
;
; The above copyright notice and this permission notice shall be included in all
; copies or substantial portions of the Software.
;
; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
; IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
; FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
; AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
; LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
; OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
; SOFTWARE.
;

; Simple integration kernel which takes in pointer to the subbanded dm trials vector
; integrates all the subbands and wirtes to the dmtrial vector

; constants used for ease of reading the code.
SIZE_OF_LONG equ 8
SIZE_OF_INT equ 4

; actual data location
DATA_LOCATION equ 128

GLOBAL nasm_integrate_klotski_bruteforce
nasm_integrate_klotski_bruteforce:

        ;rdi: DataOut
        ;rsi: Array of pointers
        ;rdx: number of pointers
        ;rcx: number of samples
        ;r8: number of channels
        ;r9: total size of stack

        ;push non volatile registers to stack
        push rbx
        push rsi
        push rdi
        push r12
        push r13
        push r14
        push r15
        mov rbx, rdx
        sub rsp, r9 ; Allocate memory in the stack

; move all the pointers to start of the vectors to stack
        xor r10, r10 ; set index = 0
loop0:  mov rax, SIZE_OF_LONG
        mul r10
        mov r14, qword [rsi+rax]
        mov qword [rsp+rax], r14
        inc r10
        cmp r10, rbx
        jl loop0

; loop over all the subbands and integrate
        xor r11, r11
loop1:  xor r10, r10
        vxorpd zmm0, zmm0, zmm0 ; zero zmm0
loop2:  mov rax, SIZE_OF_LONG
        mul r10
        mov r14, qword [rsp+rax] ; move the location of next suband the r14
        mov rax, SIZE_OF_INT
        mul r11
        vmovdqu32 zmm1, zword [r14+rax] ; move zmm vector from the sunband to zmm1
        vpaddd zmm0, zmm0, zmm1 ; mm0 = zmm0 + zmm1
        inc r10
        cmp r10, rbx
        jl loop2
        vcvtdq2ps zmm0, zmm0 ; convert int to float
        movd xmm6, r8d ; nchans -> xmm6 (last 32 bits)
        vpbroadcastd zmm6, xmm6 ; make a vector zmm6 with all the elements equal to nchans
        vcvtdq2ps zmm6, zmm6 ; conver zmm6 from int to float
        vdivps zmm0, zmm0, zmm6 ; zmm0 = zmm0/zmm6
        vmovdqu32 zword [rdi+rax], zmm0 ; move the zmm0 to output vector.
        mov rax, 16
        add r11, rax
        cmp r11, rcx
        jl loop1

done:
        add rsp, r9 ; release stack

        ;clean up
        pop r15
        pop r14
        pop r13
        pop r12
        pop rdi
        pop rsi
        pop rbx
        ret