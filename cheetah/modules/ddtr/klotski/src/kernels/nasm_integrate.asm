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

SIZE_OF_FLOAT equ 4
SIZE_OF_LONG equ 8
SIZE_OF_ZMM equ 64

DATA_OUT_POINTERS equ 0               ;DATA_OUT_POINTERS
DATA_SUB_POINTERS equ 8               ;DATA_SUB_POINTERS
DMSHIFTS_LOCATION equ 16                ;DMSHIFTS_LOCATION
DSAMPS_PER_BAND_LOCATION equ 24         ;DSAMPS_PER_BAND_LOCATION
NCHANS equ 32                          ;NCHANS
NBANDS equ 40                          ;NBANDS
NDMS equ 48                            ;NDMS
NITER equ 56                           ;NITER
STACK_SIZE equ 64                      ;STACK_SIZE
RDX_REG equ 72

GLOBAL nasm_integrate
nasm_integrate:

        ;rdi: data_out_pointers
        ;rsi: data_sub_pointers
        ;rdx: dmshift_pointers
        ;rcx: dsamps_band
        ;r8: stack_variables
        ;r9: total_size_of_stack

        ;push non volatile registers to stack

        mov r10, rdx
        push rdx
        push rbx
        push rsi
        push rdi
        push r12
        push r13
        push r14
        push r15
        push rbp
        sub rsp, r9 ; Allocate memory in the stack

        mov qword [rsp+RDX_REG], r10

        xor r15, r15
stack_loop:
        mov r14, qword [r8+r15]
        mov qword [rsp+r15], r14
        add r15, 8
        cmp r15, STACK_SIZE
        jle stack_loop



;moving all the dmshift values to stack
        xor r10, r10 ; set band = 0
loop0:
        mov rax, SIZE_OF_LONG
        mul r10
        mov r14, qword [rsp+RDX_REG]
        mov r12, qword [r14+rax]
        xor r11, r11 ; set dm_index=0
loop1:
        mov rax, SIZE_OF_FLOAT
        mul r11
        mov r14d, dword [r12+rax] ; r14 = dm_shift value
        mov r13, rax
        mov r15, qword [rsp+NDMS]
        mov rax, SIZE_OF_FLOAT
        mul r15
        mul r10
        add rax, r13
        mov r15, qword [rsp+DMSHIFTS_LOCATION]
        add rax, r15
        mov dword [rsp+rax], r14d
        inc r11
        mov r15, qword [rsp+NDMS]
        cmp r11, r15
        jl loop1
        inc r10
        cmp r10, qword [rsp+NBANDS]
        jl loop0

        xor r10, r10 ; set band = 0
loop2:
        mov rax, SIZE_OF_LONG
        mul r10
        mov r12, qword [rsi+rax]
        mov r15, qword [rsp+DATA_SUB_POINTERS]
        add rax, r15
        mov qword [rsp+rax], r12
        mov r15, qword [rsp+NBANDS]
        inc r10
        cmp r10, r15
        jl loop2

        xor r10, r10 ; set dm_index = 0
loop3:
        mov rax, SIZE_OF_LONG
        mul r10
        mov r12, qword [rdi+rax]
        mov r14, qword [rsp+DATA_OUT_POINTERS]
        add rax, r14
        mov qword [rsp+rax], r12
        mov r15, qword [rsp+NDMS]
        inc r10
        cmp r10, r15
        jl loop3


        xor r10, r10 ; set band = 0
loop4:
        mov rax, SIZE_OF_FLOAT
        mul r10
        mov r12d, dword [rcx+rax]
        mov r14, qword [rsp+DSAMPS_PER_BAND_LOCATION]
        add rax, r14
        mov dword [rsp+rax], r12d
        mov r15, qword [rsp+NBANDS]
        inc r10
        cmp r10, r15
        jl loop4
;------------------------------------------------------------------------------------

        xor r10, r10 ; dm_index = 0
loop5:
        xor r11, r11 ; samp = 0

loop6:
        vxorpd zmm0, zmm0, zmm0
        xor r12, r12 ; band = 0
loop7:
        mov rax, SIZE_OF_FLOAT
        mul r12
        mov r14, qword [rsp+DSAMPS_PER_BAND_LOCATION]
        add rax, r14
        xor r13, r13
        mov r13d, dword [rsp+rax] ; dsamps for the band
        mov rax, SIZE_OF_FLOAT
        mul r13
        mov r13, rax; r13= size_of_dsamps

        mov r14, qword [rsp+NDMS]
        mov rax, SIZE_OF_FLOAT
        mul r14
        mul r12
        mov r9, rax
        mov rax, SIZE_OF_FLOAT
        mul r10
        add rax, r9
        mov r14, qword [rsp+DMSHIFTS_LOCATION]
        add rax, r14
        xor r9,r9
        mov rax, SIZE_OF_FLOAT
        mul r9
        mov r9, rax ; r9=shift_size

        mov rax, SIZE_OF_LONG
        mul r12
        mov r14, qword [rsp+DATA_SUB_POINTERS]
        add rax, r14
        mov r14, qword [rsp+rax]
        mov rax, r13
        mul r10
        add r14, rax
        mov rax, SIZE_OF_ZMM
        mul r11
        add r14, rax

        vmovdqu16 zmm1, zword [r14+r9]
        vpaddd zmm0, zmm0, zmm1

        inc r12
        mov r14, qword [rsp+NBANDS]
        cmp r12, r14
        jl loop7


        vcvtdq2ps zmm0, zmm0

        mov r14, qword [rsp+NCHANS]
        movd xmm6, r14d
        vpbroadcastd zmm6, xmm6
        vcvtdq2ps zmm6, zmm6
        vdivps zmm0, zmm0, zmm6


        mov r14, qword [rsp+DATA_OUT_POINTERS]
        mov rax, SIZE_OF_LONG
        mul r10
        add rax, r14
        mov r14, qword [rsp+rax]
        mov rax, SIZE_OF_ZMM
        mul r11
        vmovdqu32 zword [r14+rax], zmm0
        inc r11
        mov r14, qword [rsp+NITER]
        cmp r11, r14
        jl loop6

        inc r10
        mov r14, qword [rsp+NDMS]
        cmp r10, r14
        jl loop5
        jmp done

done:
        mov r14, qword [rsp+STACK_SIZE]
        add rsp, r14

        ;clean up
        pop rbp
        pop r15
        pop r14
        pop r13
        pop r12
        pop rdi
        pop rsi
        pop rbx
        pop rdx
        ret