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

SIZE_OF_SHORT equ 2
SIZE_OF_FLOAT equ 4
SIZE_OF_YMM equ 32
SIZE_OF_ZMM equ 64

;These not the exact values but the address of the location
;where these are stored in stack

DATA_SIZE equ 0
OVERLAP_SIZE equ 8
DATAOUT_SIZE equ 16
COUNTS_ARRAY_SIZE equ 24
INDEX_SHIFTS_SIZE equ 32
INDEX_SIZE equ 40
BASE_SIZE equ 48
TEMP_SIZE equ 56
DMINDEX_SHIFTS_SIZE equ 64
START_DM_SHIFTS_SIZE equ 72
TSAMPS_SIZE equ 80
DATA_LOCATION equ 88
OVERLAP_LOCATION equ 96
DATAOUT_LOCATION equ 104
COUNTS_ARRAY_LOCATION equ 112
INDEX_SHIFTS_LOCATION equ 120
INDEX_LOCATION equ 128
BASE_LOCATION equ 136
TEMP_LOCATION equ 144
DMINDEX_SHIFTS_LOCATION equ 152
START_DM_SHIFTS_LOCATION equ 160
NDMS equ 168
TSAMPS equ 176
DSAMPS equ 184
NSAMPS equ 192
NITER_SIZE equ 200
NCHANS equ 208
SAMPS_PER_ITERATION equ 216
NUM_ITERATIONS equ 224
DSAMPS_OUTPUT_SIZE equ 232
EDGE_SAMPLE equ 240
TOTAL_SIZE equ 248
RDI_REG equ 256
RSI_REG equ 264
RDX_REG equ 272
RCX_REG equ 280
R8_REG equ 288
R9_REG equ 296
RSP_8 equ 304
RSP_16 equ 312
RSP_24 equ 320
RSP_32 equ 328
CURRENT_CHANNEL equ 336
COUNTS_OFFSET equ 344
CURRENT_SHIFT equ 352
SAMPS_PER_ITERATION_INT_SIZE equ 360
SAMPS_PER_ITERATION_SHORT_SIZE equ 368

; actual data location

GLOBAL nasm_dedisperse
nasm_dedisperse:

        ;rdi: VariablesArray
        ;rsi: total_size
        ;rdx: dataIn
        ;rcx: dataOut
        ;r8: counts_array
        ;r9: total_shift
        ;rsp+8: total_index
        ;rsp+16: total_base
        ;rsp+24: shift_per_dmindex
        ;rsp+32: start_dm_shifts

        mov r11, rdx
        mov r10, rsp
        push rdx
        push rbx
        push rsi
        push rdi
        push r12
        push r13
        push r14
        push r15
        push rbp
        sub rsp, rsi ; Allocate memory in the stack
        mov qword [rsp+RDI_REG], rdi
        mov qword [rsp+RSI_REG], rsi
        mov qword [rsp+RCX_REG], rcx
        mov qword [rsp+RDX_REG], r11 ; r11 = rdx
        mov qword [rsp+R9_REG], r9
        mov qword [rsp+R8_REG], r8
        mov r11, qword [r10+8]
        mov qword [rsp+RSP_8], r11
        mov r11, qword [r10+16]
        mov qword [rsp+RSP_16], r11
        mov r11, qword [r10+24]
        mov qword [rsp+RSP_24], r11
        mov r11, qword [r10+32]
        mov qword [rsp+RSP_32], r11

;----------------------- copy stack------------------------------------
        mov rsi, rdi
        mov rdi, rsp
        mov rcx,TOTAL_SIZE+8
        rep movsb
;------------------------done copy--------------------------------------

;----------------------- copy counts array------------------------------------
        mov rsi, qword [rsp+R8_REG]
        mov r14, qword [rsp+COUNTS_ARRAY_LOCATION]
        mov rdi, rsp
        add rdi, r14
        mov rcx,qword [rsp+COUNTS_ARRAY_SIZE]
        rep movsb
;------------------------done counts array------------------------------------


;; move dmindex_shifts to cache
        xor r8, r8
dmindex_shifts_loop:
        mov r14, qword [rsp+RSP_24]
        add r14, r8
        mov r15d, dword[r14]
        mov rax, SIZE_OF_FLOAT
        mul r15
        mov r14, qword [rsp+DMINDEX_SHIFTS_LOCATION]
        add r14, r8
        mov dword[rsp+r14], eax
        add r8, SIZE_OF_FLOAT
        mov r14, qword [rsp+DMINDEX_SHIFTS_SIZE]
        cmp r8, r14
        jl dmindex_shifts_loop

;; move dmindex_shifts to cache
;        xor r8, r8
;start_dm_shifts_loop:
;        mov r14, qword [rsp+RSP_32]
;        add r14, r8
;        mov r15d, dword[r14]
;        mov r14, qword [rsp+START_DM_SHIFTS_LOCATION]
;        add r14, r8
;        mov dword[rsp+r14], r15d
;        add r8, SIZE_OF_FLOAT
;        mov r14, qword [rsp+START_DM_SHIFTS_SIZE]
;        cmp r8, r14
;        jl start_dm_shifts_loop

;----------------------- copy start_dm_shifts------------------------------------
        mov rsi, qword [rsp+RSP_32]
        mov r14, qword [rsp+START_DM_SHIFTS_LOCATION]
        mov rdi, rsp
        add rdi, r14
        mov rcx, qword [rsp+START_DM_SHIFTS_SIZE]
        rep movsb
;------------------------done start_dm_shifts------------------------------------


;----------------------- set dataout to zeros------------------------------------
        mov rdi, qword [rsp+DATAOUT_LOCATION]
        add rdi, rsp
        mov al, 0x0
        mov rcx, qword [rsp+DATAOUT_SIZE]
        rep stosb
;--------------------------------------------------------------------------------


;----------------------- copy base------------------------------------
        mov rsi, qword [rsp+RSP_16]
        mov r14, qword [rsp+BASE_LOCATION]
        mov rdi, rsp
        add rdi, r14
        mov rcx, qword [rsp+INDEX_SIZE]
        rep movsb
;------------------------done copy base------------------------------------

;----------------------- copy index------------------------------------
        mov rsi, qword [rsp+RSP_8]
        mov r14, qword [rsp+INDEX_LOCATION]
        mov rdi, rsp
        add rdi, r14
        mov rcx, qword [rsp+INDEX_SIZE]
        rep movsb
;------------------------done copy index------------------------------------

;----------------------- copy index_shift------------------------------------
        mov rsi, qword [rsp+R9_REG]
        mov r14, qword [rsp+INDEX_SHIFTS_LOCATION]
        mov rdi, rsp
        add rdi, r14
        mov rcx, qword [rsp+INDEX_SIZE]
        rep movsb
;------------------------done copy index_shift------------------------------------

; Moving FrequencyTime data to the stack
        xor r8, r8 ; set channel = 0
channel_loop1:
        mov r14, qword [rsp+TSAMPS_SIZE]
        mov rax, r14 ; rax = TSAMPS_SIZE (total samples int FT block)
        mul r8 ; rax = r8*rax ~ rax = channel*TSAMPS_SIZE
        mov r15, rax
        mov rax, SIZE_OF_FLOAT
        mul r8
        mov r14, qword [rsp+START_DM_SHIFTS_LOCATION]
        add rax, r14
        mov r14d, dword[rsp+rax]
        mov rax, SIZE_OF_SHORT
        mul r14
        add rax, r15
        mov rdx, qword [rsp+RDX_REG]
        vmovdqu16 zmm0, zword [rdx+rax] ; copy the value data_in[channel*TSAMPS + 64*sample]
        mov rax, SIZE_OF_ZMM ; setting rax= 64 (zmm size)
        mul r8 ; rax  = 64*channel
        mov r14, qword [rsp+OVERLAP_LOCATION]
        add rax, r14
        vmovdqu16 zword [rsp+rax], zmm0 ; copy data_in[channel*TSAMPS + 32*sample] to stack
        inc r8 ; channel++
        mov r14, qword [rsp+NCHANS]
        cmp r8, r14 ; break when channel==NCHANS
        jl channel_loop1

        mov r15, qword [rsp+NUM_ITERATIONS]
        mov rax, SIZE_OF_ZMM
        mul r15
        mov qword [rsp+SAMPS_PER_ITERATION_INT_SIZE], rax
        mov rax, SIZE_OF_YMM
        mul r15
        mov qword [rsp+SAMPS_PER_ITERATION_SHORT_SIZE], rax

start_dedispersion:
        xor r10, r10 ; local sample number
        xor r11, r11 ; global sample number
sample_loop:
        xor r8, r8 ; r8=0 channel
        xor r15, r15
        mov qword [rsp+COUNTS_OFFSET], r15

channel_loop2:
;move data to the data location
        mov rax, SIZE_OF_ZMM
        mul r8
        mov r14, qword [rsp+OVERLAP_LOCATION]
        add r14, rax
        vmovdqu16 zmm0, zword [rsp+r14]
        mov r14, qword [rsp+DATA_LOCATION]
        vmovdqu16 zword [rsp+r14], zmm0

        mov rax, qword [rsp+TSAMPS_SIZE]
        mul r8
        mov r14, rax
        mov rax, qword [rsp+SAMPS_PER_ITERATION_SHORT_SIZE]
        mul r11
        add rax, SIZE_OF_ZMM ;zmm size to take care of overlap
        add r14, rax
        mov r15, qword [rsp+START_DM_SHIFTS_LOCATION]
        mov rax, SIZE_OF_FLOAT
        mul r8
        add rax, r15
        mov r15d, dword [rsp+rax]
        mov rax, SIZE_OF_SHORT
        mul r15
        add r14, rax
; move the data from dataout to temp location
;----------------------- copy dataout to temp location------------------------------------
        mov rsi, qword [rsp+RDX_REG]
        add rsi, r14
        mov rdi, qword [rsp+DATA_LOCATION]
        add rdi, SIZE_OF_ZMM
        add rdi, rsp
        mov rcx, qword [rsp+SAMPS_PER_ITERATION_SHORT_SIZE]
        rep movsb
;------------------------done copy dataout to temp location------------------------------------
        mov rax, SIZE_OF_ZMM
        mul r8
        mov r14, qword [rsp+OVERLAP_LOCATION]
        add r14, rax

        mov r15, qword [rsp+DATA_LOCATION]
        mov rax, qword [rsp+SAMPS_PER_ITERATION_SHORT_SIZE]
        add rax, r15
        vmovdqu16 zmm0, zword [rsp+rax]
        vmovdqu16 zword [rsp+r14], zmm0

        ;; print data
        ;cmp r11, 2
        ;jne conn
        ;cmp r8, 0
        ;jne conn
        ;jmp print_data
;conn:
;-----------------------------------------------------------------------------

        mov qword [rsp+CURRENT_CHANNEL], r8
        mov rax, SIZE_OF_FLOAT
        mul r8
        mov r14, qword [rsp+COUNTS_ARRAY_LOCATION]
        add r14, rax
        xor r15, r15
        mov r15d, dword [rsp+r14]
        mov rax, 4
        mul r15
        mov qword [rsp+NITER_SIZE], rax
        mov r12, -1; r12=-1 current_base
        xor r13, r13; count=0
niter_loop:
        mov r14, qword [rsp+BASE_LOCATION] ; r14 = &base[0]
        mov r15, qword [rsp+COUNTS_OFFSET]
        add r14, r15
        mov rax, r13
        add r14, rax ; r14 = &base[c]
        mov r9d, dword [rsp+r14] ; r9 = base[c]

        cmp r9, r12
        je current_base
        mov r12, r9
        mov rax, qword [rsp+SAMPS_PER_ITERATION_INT_SIZE]
        mul r9 ; rax = 256*r9
        mov r14, qword [rsp+DATAOUT_LOCATION]
        add r14, rax ; r14 = DATAOUT_LOCATION+128*r9

; move the data from dataout to temp location
;----------------------- copy dataout to temp location------------------------------------
        mov rsi, r14
        add rsi, rsp
        mov rdi, qword [rsp+TEMP_LOCATION]
        add rdi, rsp
        mov rcx, qword [rsp+SAMPS_PER_ITERATION_INT_SIZE]
        rep movsb
;------------------------done copy dataout to temp location------------------------------------

current_base:

        mov r14, qword [rsp+INDEX_SHIFTS_LOCATION]
        mov r15, qword [rsp+COUNTS_OFFSET]
        add r14, r15
        mov rax, r13
        add r14, rax
        mov r9d, dword [rsp+r14] ; shift for the channel
        mov r14, qword [rsp+DATA_LOCATION]
        mov rax, SIZE_OF_SHORT
        mul r9
        add r14, rax
        mov rsi, r14 ; save r14

        mov r14, qword [rsp+INDEX_LOCATION]
        mov r15, qword [rsp+COUNTS_OFFSET]
        add r14, r15
        mov rax, r13
        add r14, rax
        mov r9d, dword [rsp+r14]

        mov rax, qword [rsp+SAMPS_PER_ITERATION_INT_SIZE]
        mul r9
        mov r14, qword [rsp+DATAOUT_LOCATION]
        add r14, rax

        xor rdi, rdi
        xor rbx, rbx
temp_loop1:
        mov rax, rbx
        add rax, rsi
        vmovdqu16 ymm15, yword [rsp+rax]
        vpmovzxwd zmm0, ymm15

        mov rax, rdi
        mov r15, qword [rsp+TEMP_LOCATION]
        add rax, r15

        vmovdqu16 zmm1, zword [rsp+rax]
        vpaddd zmm0, zmm0, zmm1



        mov rax, rdi
        add rax, r14

        vmovdqu16 zword [rsp+rax], zmm0
        add rdi, SIZE_OF_ZMM
        add rbx, SIZE_OF_YMM
        mov r15, qword [rsp+SAMPS_PER_ITERATION_INT_SIZE]
        cmp rdi, r15
        jl temp_loop1


        add r13, SIZE_OF_FLOAT
        mov r14, qword [rsp+NITER_SIZE]
        cmp r13, r14
        jl niter_loop

        mov rax, qword [rsp+NITER_SIZE]
        mov r14, qword [rsp+COUNTS_OFFSET]
        add r14, rax
        mov qword [rsp+COUNTS_OFFSET], r14


        inc r8
        mov r14, qword [rsp+NCHANS]
        cmp r8, r14
        jl channel_loop2


        mov rax, qword [rsp+NITER_SIZE]
        mov r14, qword [rsp+COUNTS_OFFSET]
        sub r14, rax
        mov qword [rsp+COUNTS_OFFSET], r14

;fill the empty dm_indexs
        xor r13, r13
niter_loop3:
        mov r14, qword [rsp+INDEX_LOCATION]
        mov r15, qword [rsp+COUNTS_OFFSET]
        add r14, r15
        mov rax, r13
        add r14, rax
        mov r8d, dword [rsp+r14]
        mov r14, qword [rsp+NITER_SIZE]
        mov r9, r13
        add r9, SIZE_OF_FLOAT
        cmp r14, r9
        jne not_equal
        mov r9, qword [rsp+NDMS]
        jmp cont
not_equal:
        mov r14, qword [rsp+INDEX_LOCATION]
        mov r15, qword [rsp+COUNTS_OFFSET]
        add r14, r15
        mov rax, r13
        add r14, rax
        add r14, SIZE_OF_FLOAT
        mov r9d, dword [rsp+r14]
cont:
        mov rax, qword [rsp+SAMPS_PER_ITERATION_INT_SIZE]
        mul r8
        mov r14, qword [rsp+DATAOUT_LOCATION]
        add r14, rax

; move the data from dataout to temp location
;----------------------- copy dataout to temp location------------------------------------
        mov rsi, r14
        add rsi, rsp
        mov rdi, qword [rsp+TEMP_LOCATION]
        add rdi, rsp
        mov rcx, qword [rsp+SAMPS_PER_ITERATION_INT_SIZE]
        rep movsb
;------------------------done copy dataout to temp location------------------------------------

dmindex_loop3:

;----------------------- copy dataout to temp location------------------------------------
        mov rsi, qword [rsp+TEMP_LOCATION]
        add rsi, rsp
        mov rdi, r14
        add rdi, rsp
        mov rcx, qword [rsp+SAMPS_PER_ITERATION_INT_SIZE]
        rep movsb
;------------------------done copy dataout to temp location------------------------------------
        inc r8
        mov rax, qword [rsp+SAMPS_PER_ITERATION_INT_SIZE]
        add r14, rax
        cmp r8, r9
        jl  dmindex_loop3
        add r13, 4
        mov r14, qword [rsp+NITER_SIZE]
        cmp r13, r14
        jl niter_loop3


; copy the data to the output memory

        xor r13, r13
dmindex_loop4:
        mov rax, qword [rsp+SAMPS_PER_ITERATION_INT_SIZE]
        mul r13
        mov r14, qword [rsp+DATAOUT_LOCATION]
        add rax, r14
        mov rsi, rax ; save location in data_out

        mov rax, qword [rsp+SAMPS_PER_ITERATION_INT_SIZE]
        mul r11
        mov r14, rax ; r14 = global_location
        ; account for the shift per dm

        mov rax, SIZE_OF_FLOAT
        mul r13
        mov r15, qword [rsp+DMINDEX_SHIFTS_LOCATION]
        add rax, r15
        mov r15d, dword[rsp+rax]
        mov qword [rsp+CURRENT_SHIFT], r15
        mov rcx, qword [rsp+RCX_REG]


        vxorpd zmm1, zmm1, zmm1
        mov rbx, qword [rsp+CURRENT_SHIFT]; rbx = shift_size
        xor rdi, rdi
temp_loop4:
        mov rax, rdi
        add rax, rsi
        vmovdqu16 zmm0, zword [rsp+rax]
        vmovdqu16 zword [rsp+rax], zmm1

        mov rax, rdi
        add rax, r14
        mov r15, rbx
        cmp rax, r15 ; cmp global_location , shift
        jge continue5
        sub r15, rax
        cmp r15, SIZE_OF_ZMM
        jge donot_copy
        mov rax, r15
        mov r15, qword [rsp+TEMP_LOCATION]
        vmovdqu16 zword [rsp+r15+64], zmm1
        vmovdqu16 zword [rsp+r15], zmm0
        add r15, rax
        vmovdqu16 zmm0, zword [rsp+r15]

        mov r9, qword [rsp+SAMPS_PER_ITERATION_INT_SIZE]
        mov rax, rbx
        idiv r9
        mul r9
        mov r9, rax
        mov rax, r14
        mov r15, rbx
        add rax, r15
        sub rax, r9
        jmp copy
continue5:
        mov r15, rbx
        sub rax, r15
        mov r15, qword [rsp+DSAMPS_OUTPUT_SIZE]
        sub r15, SIZE_OF_ZMM
        cmp rax, r15
        jg continue6
        mov r15, rbx
        add rax, r15
        jmp copy
continue6:
        add r15, SIZE_OF_ZMM
        cmp rax, r15
        jge donot_copy
        sub r15, rax
        mov rax, qword [rsp+TEMP_LOCATION]
        vmovdqu16 zword [rsp+rax], zmm1
        vmovdqu16 zword [rsp+rax+64], zmm0
        add rax, r15
        vmovdqu16 zmm0, zword [rsp+rax]
        mov rax, qword [rsp+DSAMPS_OUTPUT_SIZE]
        sub rax, SIZE_OF_ZMM
        mov r15, rbx
        add rax, r15
copy:
        mov r15, rbx
        sub rax, r15
        mov r15, rax
        mov rax, qword [rsp+DSAMPS_OUTPUT_SIZE]
        mul r13
        add rax, r15

        vmovdqu16 zmm2, zword [rcx+rax]
        vpaddd zmm2, zmm2, zmm0

        vmovdqu16 zword [rcx+rax], zmm2
donot_copy:
        add rdi, SIZE_OF_ZMM
        mov r15, qword [rsp+SAMPS_PER_ITERATION_INT_SIZE]
        cmp rdi, r15
        jl temp_loop4

        inc r13
        mov r14, qword [rsp+NDMS]
        cmp r13, r14
        jl  dmindex_loop4


        inc r10
        inc r11
        mov r14, qword [rsp+NSAMPS]
        mov rax, qword [rsp+SAMPS_PER_ITERATION]
        mul r10
        cmp rax, r14
        jl continue4
        mov r10, 0
continue4:
        mov r15, qword [rsp+DSAMPS]
        mov rax, qword [rsp+SAMPS_PER_ITERATION]
        mul r11

        cmp rax, r15
        jl sample_loop

        mov rax, qword [rsp+EDGE_SAMPLE]
        cmp rax, 0
        je done
        sub rax, 1
        mov qword [rsp+EDGE_SAMPLE], rax
        mov rax, qword [rsp+SAMPS_PER_ITERATION]
        mov r8, 32
        idiv r8d
        mov r9, rax
        mul r11
        mov r11, rax
        mov rax, r9
        mul r10
        mov r10, rax
        mov rax, 32
        mov qword [rsp+SAMPS_PER_ITERATION], rax
        mov rax, 128
        mov qword [rsp+SAMPS_PER_ITERATION_INT_SIZE], rax
        mov rax, 64
        mov qword [rsp+SAMPS_PER_ITERATION_SHORT_SIZE], rax
        jmp sample_loop

done:
        mov r14, qword [rsp+TOTAL_SIZE]
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



print:
        mov rdi, qword [rsp + RDI_REG]
        vmovdqu16 zword [rdi], zmm0
        mov qword [rdi+64], rax
        jmp done

print_data:
        xor r8, r8
        mov rdi, qword [rsp + RDI_REG]
        mov rsi, qword [rsp + DATA_LOCATION]
print_loop:
        vmovdqu16 zmm0, zword [rsp+rsi]
        vmovdqu16 zword [rdi], zmm0
        add rdi, 64
        add rsi, 64
        add r8, 64
        mov rax, qword [rsp + DATA_SIZE]
        cmp r8, rax
        jl print_loop

        jmp done

print_r14:
        mov rdi, qword [rsp + RDI_REG]
        mov qword [rdi], rsi
        jmp done
