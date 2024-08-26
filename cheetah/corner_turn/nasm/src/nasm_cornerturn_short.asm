;
; The MIT License (MIT)
;
; Copyright (c) 2016-2022 The SKA organisation
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

NUMBER_OF_ELEMENTS equ 32
ELEMENTS_SIZE equ NUMBER_OF_ELEMENTS*2
DATAIN_SIZE equ 2048 ; 32*32*sizeof(short)
DATAOUT_SIZE equ 2048 ; 32*32*sizeof(short)
DATAIN_LOCATION equ 0
DATAOUT_LOCATION equ DATAIN_SIZE
NUMBER_OF_ARGUMENTS equ 6
ARGUMENTS_SIZE equ NUMBER_OF_ARGUMENTS*8
ARGUMENTS_LOCATION equ DATAIN_SIZE + DATAOUT_SIZE
NUMBER_OF_NON_VOLATILE_REGISTERS_USED equ 4
COPY_OF_NON_VOLATILE_REGISTERS_LOCATION equ DATAIN_SIZE + DATAOUT_SIZE + ARGUMENTS_LOCATION
COPY_OF_NON_VOLATILE_REGISTERS_SIZE equ 8*NUMBER_OF_NON_VOLATILE_REGISTERS_USED
TOTAL_SIZE equ DATAIN_SIZE + DATAOUT_SIZE + ARGUMENTS_SIZE + COPY_OF_NON_VOLATILE_REGISTERS_SIZE

GLOBAL nasm_cornerturn_short
nasm_cornerturn_short:

;      rdi : Location of dataIn
;      rsi : location of dataOut
;      rdx : number of channels
;      rcx : number of samples
;      r8  : channel
;      r9  : sample

        push rbx
        push rsi
        push rdi
        push r12
        push r13
        push r14
        push r15

        sub rsp, TOTAL_SIZE ; Allocate memory in the stack

        mov qword [rsp+ARGUMENTS_LOCATION], rdi
        mov qword [rsp+ARGUMENTS_LOCATION+8], rsi
        mov qword [rsp+ARGUMENTS_LOCATION+16], rdx
        mov qword [rsp+ARGUMENTS_LOCATION+24], rcx
        mov qword [rsp+ARGUMENTS_LOCATION+32], r8
        mov qword [rsp+ARGUMENTS_LOCATION+40], r9

        ; Moving DataIn to the stack
        xor r11, r11 ; set sample_offset = 0
loop0:  mov r14, qword [rsp+ARGUMENTS_LOCATION+16]
        mov r9, [r14]
        mov rax, r9 ; rax = NUMBER_OF_CHANNELS
        xor r15, r15
        mov r14, qword [rsp+ARGUMENTS_LOCATION+40]
        mov r9, [r14]
        add r15, r9
        add r15, r11
        mul r15
        mov r14, qword [rsp+ARGUMENTS_LOCATION+32]
        mov r9, [r14]
        add rax, r9
        mov r14, 2
        mul r14
        vmovdqu ymm1, yword [rdi+rax] ; dataIn[(sample+sample_offset)*number_of_channels + channel]
        vmovdqu ymm2, yword [rdi+rax+32]
        mov rax, NUMBER_OF_ELEMENTS ; rax = NUMBER_OF_ELEMENTS
        mul r11 ; rax = NUMBER_OF_ELEMENTS*sample_offset
        mov r14, 2
        mul r14 ; rax = 2*NUMBER_OF_ELEMENTS*sample_offse
        vmovdqu yword [rsp+DATAIN_LOCATION+rax], ymm1 ;
        vmovdqu yword [rsp+DATAIN_LOCATION+rax+32], ymm2 ;
        inc r11 ;
        cmp r11, NUMBER_OF_ELEMENTS
        jne loop0

        xor r11, r11 ; set sample = 0
loop1:  xor r10, r10 ; set channel = 0
loop2:  mov rax, NUMBER_OF_ELEMENTS ; rax = 16
        mul r11 ; rax = sample*16
        add rax, r10
        mul r14 ; rax = sizeof(short)*sample*16
        mov r13w, word [rsp + DATAIN_LOCATION + rax]
        mov rax, NUMBER_OF_ELEMENTS
        mul r10
        add rax, r11
        mul r14
        mov word [rsp + DATAOUT_LOCATION + rax], r13w
        inc r10
        cmp r10, NUMBER_OF_ELEMENTS
        jne loop2
        inc r11
        cmp r11, NUMBER_OF_ELEMENTS
        jne loop1

        xor r10, r10 ; set channel_offset = 0
loop3:  mov rax, NUMBER_OF_ELEMENTS ; rax = NUMBER_OF_ELEMENTS
        mul r10 ; rax = NUMBER_OF_ELEMENTS*channel_offset
        mov r14, 2
        mul r14 ; rax = NUMBER_OF_ELEMENTS*channel_offset*2
        vmovdqu ymm1, yword [rsp+DATAOUT_LOCATION+rax]
        vmovdqu ymm2, yword [rsp+DATAOUT_LOCATION+rax+32]
        mov r14, qword [rsp+ARGUMENTS_LOCATION+24]
        mov r9, [r14]
        mov rax, r9
        mov r14, qword [rsp+ARGUMENTS_LOCATION+32]
        mov r9, [r14]
        mov r15, r9
        add r15, r10
        mul r15
        mov r14, qword [rsp+ARGUMENTS_LOCATION+40]
        mov r9, [r14]
        add rax, r9
        mov r14, 2
        mul r14
        vmovdqu yword [rsi+rax], ymm1
        vmovdqu yword [rsi+rax+32], ymm2
        inc r10 ;
        cmp r10, NUMBER_OF_ELEMENTS
        jne loop3

done:
        add rsp, TOTAL_SIZE
        pop r15
        pop r14
        pop r13
        pop r12
        pop rdi
        pop rsi
        pop rbx
        ret