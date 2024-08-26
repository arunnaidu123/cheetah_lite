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

;extern "C" void nasm_filter_spdt( std::size_t* stack_variables
;                                , std::size_t total_size
;                                , std::size_t* data_in_pointers
;                                , float* temp_cands
;                                , unsigned int* widths_array
;                                , double* mst_array
;                                );

DATA_IN_POINTERS_SIZE equ 0
SCRATCH_SIZE equ 8
TEMP_SIZE equ 16
TEMP_CORNERTURNED_SIZE equ 24
SUM_ARRAY_SIZE equ 32
WIDTHS_ARRAY_SIZE equ 40
MEAN_ARRAY_SIZE equ 48
STD_ARRAY_SIZE equ 56
THRESHOLD_ARRAY_SIZE equ 64
DATA_IN_POINTERS_LOCATION equ 72
SCRATCH_LOCATION equ 80
TEMP_LOCATION equ 88
TEMP_CORNERTURNED_LOCATION equ 96
SUM_ARRAY_LOCATION equ 104
WIDTHS_ARRAY_LOCATION equ 112
MEAN_ARRAY_LOCATION equ 120
STD_ARRAY_LOCATION equ 128
THRESHOLD_ARRAY_LOCATION equ 136
NUMBER_OF_WIDTHS equ 144
MAX_WIDTH equ 152
START_DMINDX equ 160
SAMPLE_SIZE_PER_DMINDX equ 168
TOTAL_SIZE equ 176
RDI_REG equ 184
RSI_REG equ 192
RCX_REG equ 200
RDX_REG equ 208
R9_REG equ 216
R8_REG equ 224
RSP_8 equ 232
RSP_16 equ 240
NUM_CANDIDATES equ 248

GLOBAL nasm_filter_spdt
nasm_filter_spdt:

    ;rdi: stack_variables
    ;rsi: total_size
    ;rdx: data_in_pointers
    ;rcx: temp_cands
    ;r8: widths_array
    ;r9: mst_array
    ;RSP_8: overlap
    ;RSP_16: sum_array

    mov r10, qword [rsp+8]
    mov r11, qword [rsp+16]
    push rbx
    push rsi
    push rdi
    push r12
    push r13
    push r14
    push r15

    sub rsp, rsi ; Allocate memory in the stack

    mov qword [rsp+RSP_8], r10
    mov qword [rsp+RSP_16], r11
    mov qword [rsp+RDI_REG], rdi
    mov qword [rsp+RSI_REG], rsi
    mov qword [rsp+RCX_REG], rcx
    mov qword [rsp+RDX_REG], rdx
    mov qword [rsp+R9_REG], r9
    mov qword [rsp+R8_REG], r8
    mov qword [rsp+NUM_CANDIDATES], 0

    xor r15, r15
stack_loop:
    mov r14, qword [rdi+r15]
    mov qword [rsp+r15], r14
    add r15, 8
    cmp r15, TOTAL_SIZE
    jle stack_loop


    xor r10, r10
pointers_loop:
    mov r14, qword [rsp+RDX_REG]
    mov r11, qword [r14+r10]
    mov r15, qword [rsp+DATA_IN_POINTERS_LOCATION]
    add r15, r10
    mov qword [rsp+r15], r11
    add r10, 8
    mov r14, qword [rsp+DATA_IN_POINTERS_SIZE]
    cmp r10, r14
    jl pointers_loop



; initilize scratch
    xor r10, r10
scratch_initialization_loop:
    mov r15, qword [rsp+RSP_8]
    add r15, r10
    vmovdqu32 ymm14, yword [r15]
    mov r14, qword [rsp+SCRATCH_LOCATION]
    add r14, r10
    vmovdqu32 yword [rsp+r14], ymm14
    add r10, 32
    mov r15, qword [rsp+SCRATCH_SIZE]
    sub r15, 256
    cmp r10, r15
    jl scratch_initialization_loop



    ; zmm0 sum_array
    ; zmm1 snr_array
    ; zmm2 mean_array
    ; zmm3 std_array
    ; zmm4 threshold_array
    ; zmm5 width_value array

    vxorpd zmm0, zmm0, zmm0
    xor r10, r10
sum_init_loop:
    mov r15, qword [rsp+RSP_16]
    add r15, r10
    vmovdqu32 zmm0, zword [r15]
    mov r14, qword [rsp+SUM_ARRAY_LOCATION]
    add r14, r10
    vmovdqu32 zword [rsp+r14], zmm0
    add r10, 64
    mov r14, qword [rsp+SUM_ARRAY_SIZE]
    cmp r10, r14
    jl sum_init_loop

    xor r10, r10
width_copy_loop:
    mov r15d, dword [r8+r10]
    mov r14, qword [rsp+WIDTHS_ARRAY_LOCATION]
    add r14, r10
    mov dword [rsp+r14], r15d
    add r10, 4
    mov r14, qword [rsp+NUMBER_OF_WIDTHS]
    mov rax, 4
    mul r14
    cmp r10, rax
    jl width_copy_loop

    vxorpd zmm1, zmm1, zmm1
    vmovdqu32 zmm2, zword [r9]
    vmovdqu32 zmm4, zword [r9+64]

    xor r10, r10
    mov r14, qword [rsp+STD_ARRAY_LOCATION]
    mov r15, qword [rsp+STD_ARRAY_SIZE]
std_value_copy:
    vmovdqu32 zmm14, zword [r9+r10+128]
    vmovdqu32 zword [rsp+r14], zmm14
    add r14, 64
    add r10, 64
    cmp r10, r15
    jl std_value_copy


    mov r14, qword [rsp+MAX_WIDTH]
    mov rax, 32
    mul r14
    mov r8, rax ; data_position= 8*max_width*sizeof(float)

    xor r11, r11 ; sample=r11
sample_loop:
    xor r9, r9; dm_indx
dm_indx_loop0:
    mov r14, qword [rsp+DATA_IN_POINTERS_LOCATION]
    add r14, r9
    mov r14, qword [rsp+r14]
    vmovdqu16 ymm14, yword [r14+r11]
    mov r14, qword [rsp+TEMP_LOCATION]
    mov rax, 4
    mul r9
    add r14, rax
    vmovdqu32 yword [rsp+r14], ymm14
    add r9, 8
    mov r14, qword [rsp+DATA_IN_POINTERS_SIZE]
    cmp r9, r14
    jl dm_indx_loop0

; corner turning
;-----------------------------------------------------------------------
    xor r9, r9; sample_element in temp cornered
    mov r14, qword [rsp+TEMP_LOCATION]
    mov r15, qword [rsp+TEMP_CORNERTURNED_LOCATION]
element_sample_loop:
    xor r10, r10
element_index_loop:
    mov rax, 8
    mul r10
    add rax, r9
    add rax, r14
    mov r13d, dword [rsp+rax]
    mov rax, 8
    mul r9
    add rax, r10
    add rax, r15
    mov dword [rsp+rax], r13d
    add r10, 4
    cmp r10, 32
    jl element_index_loop
    add r9, 4
    cmp r9, 32
    jl element_sample_loop
;------------------------------------------------------------------------------


    xor r9, r9
    mov r15, qword [rsp+TEMP_CORNERTURNED_LOCATION]

copy_loop:
    mov r10, r15
    add r10, r9
    vmovdqu32 ymm14, yword [rsp+r10]

    mov r14, qword [rsp+SCRATCH_LOCATION]
    mov r10, r14
    add r10, r8
    add r10, r9
    vmovdqu32 yword [rsp+r10], ymm14
    add r9, 32
    mov r14, qword [rsp+TEMP_CORNERTURNED_SIZE]
    cmp r9, r14
    jl copy_loop

    xor r9, r9 ; r9=width
width_loop:
    mov r14, qword [rsp+SUM_ARRAY_LOCATION]
    mov rax, 16
    mul r9
    add r14, rax
    vmovdqu32 zmm0, zword [rsp+r14]
    mov r14, qword [rsp+STD_ARRAY_LOCATION]
    add r14, rax
    vmovdqu32 zmm3, zword [rsp+r14]

    mov r14, qword [rsp+WIDTHS_ARRAY_LOCATION]
    add r14, r9
    xor r13, r13
    mov r13d, dword [rsp+r14] ; r13=width_value
    movd xmm5, r13d
    vpbroadcastd ymm5, xmm5
    vcvtdq2pd zmm5, ymm5
    xor r10, r10 ; sub_sample = 0
sub_sample_loop:
    mov rax, 32
    mul r10
    add rax, r8
    mov r12, rax
    mov rax, 32
    mul r13
    cmp r12, rax
    jl warp
    mov r12, r8
    sub r12, rax
    jmp continue
warp:
    mov r14, qword [rsp+SCRATCH_SIZE]
    sub r14, rax
    add r14, r8
    mov r12, r14
continue:
    mov rax, 32
    mul r10
    add rax, r8
    mov r14, qword [rsp+SCRATCH_LOCATION]
    add r14, rax
    vmovdqu32 ymm15, yword [rsp+r14]
    vcvtps2pd zmm15, ymm15
    mov rax, 32
    mul r10
    add rax, r12
    mov r14, qword [rsp+SCRATCH_LOCATION]
    add r14, rax
    vmovdqu32 ymm6, yword [rsp+r14]
    vcvtps2pd zmm6, ymm6

    vaddpd zmm0, zmm0, zmm15
    vsubpd zmm0, zmm0, zmm6

    vdivpd zmm1, zmm0, zmm5
    vsubpd zmm1, zmm1, zmm2

    vdivpd zmm1, zmm1, zmm3
    vcmppd k1, zmm1, zmm4, 0x0d
    kmovw eax,k1

    cmp rax, 0
    je continue1
    vcvtpd2ps ymm1, zmm1
    mov r14d, 100 ;
    movd xmm7, r14d
    vpbroadcastd ymm7, xmm7
    vcvtdq2ps ymm7, ymm7
    vmulps ymm1, ymm1, ymm7
    vcvtps2dq ymm1, ymm1
    mov r14, qword [rsp+TEMP_LOCATION]
    vmovdqu32 yword [rsp+r14], ymm1

    mov edi, 0 ; max_snr
    mov esi, 0 ; max_dmindex
    xor r15, r15
element_loop:
    mov eax, dword [rsp+r14]
    cmp eax, edi
    jl continue2
    mov edi, eax
    mov esi, r15d
continue2:
    add r14, 4
    inc r15
    cmp r15, 8
    jl element_loop

    mov rcx, qword [rsp+RCX_REG]
    mov r14, qword [rsp+NUM_CANDIDATES]
    mov rax, 16
    mul r14
    mov dword [rcx+rax], edi
    mov r14, qword [rsp+START_DMINDX]
    add esi, r14d


    mov dword [rcx+rax+4], esi
    mov dword [rcx+rax+8], r13d
    mov r14, rax
    mov rax, r11
    mov r15, 4
    div r15d
    add eax, r10d
    mov dword [rcx+r14+12], eax
    mov r14, qword [rsp+NUM_CANDIDATES]
    inc r14
    mov qword [rsp+NUM_CANDIDATES], r14
continue1:
    inc r10
    cmp r10, 8
    jl sub_sample_loop

    mov r14, qword [rsp+SUM_ARRAY_LOCATION]
    mov rax, 16
    mul r9
    add r14, rax
    vmovdqu32 zword [rsp+r14], zmm0

    add r9, 4
    mov r14, qword [rsp+NUMBER_OF_WIDTHS]
    mov rax, 4
    mul r14
    cmp r9, rax
    jl width_loop

    add r8, 256
    mov r14, qword [rsp+SCRATCH_SIZE]
    cmp r8, r14
    jne continue3
    mov r8, 0
continue3:
    add r11, 32
    mov r14, qword [rsp+SAMPLE_SIZE_PER_DMINDX]
    cmp r11, r14
    jl sample_loop


; copy sum_array from buffer to vector
    vxorpd zmm0, zmm0, zmm0
    xor r10, r10
sum_copy_loop:
    mov r14, qword [rsp+SUM_ARRAY_LOCATION]
    add r14, r10
    vmovdqu32 zmm0, zword [rsp+r14]
    mov r15, qword [rsp+RSP_16]
    add r15, r10
    vmovdqu32 zword [r15], zmm0
    add r10, 64
    mov r14, qword [rsp+SUM_ARRAY_SIZE]
    cmp r10, r14
    jl sum_copy_loop
; copy overlap from stack to array
;    cmp r8, 0
;    jne move_on
;    mov r8, qword [rsp+SCRATCH_SIZE]
;move_on:
;    sub r8, 256
    add r8, 256
    mov r15, qword [rsp+SCRATCH_SIZE]
    cmp r8, r15
    jne move_on
    mov r8, 0
move_on:
    xor r10, r10
overlap_copy_loop:
    mov r14, qword [rsp+SCRATCH_LOCATION]
    add r14, r8
    vmovdqu32 ymm14, yword [rsp+r14]
    mov r15, qword [rsp+RSP_8]
    add r15, r10
    vmovdqu32 yword [r15], ymm14
    add r8, 32
    mov r14, qword [rsp+SCRATCH_SIZE]
    cmp r8, r14
    jne continue7
    mov r8, 0
continue7:
    add r10, 32
    mov r15, qword [rsp+SCRATCH_SIZE]
    sub r15, 256
    cmp r10, r15
    jl overlap_copy_loop

done:
    mov r14, qword [rsp+NUM_CANDIDATES]
    mov rdi, qword [rsp+RDI_REG]
    mov qword [rdi], r14

    mov rsi, qword [rsp+RSI_REG]
    add rsp, rsi
    pop r15
    pop r14
    pop r13
    pop r12
    pop rdi
    pop rsi
    pop rbx
    ret

print:
        mov rdi, qword [rsp + RDI_REG]
        vmovdqu16 zword [rdi+8], zmm0
        jmp done

print_r14:
        mov rdi, qword [rsp + RDI_REG]
        mov qword [rdi], rax
        jmp done