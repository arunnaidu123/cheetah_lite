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

; constants used for ease of reading the code.
SIZE_OF_SHORT equ 2
SIZE_OF_FLOAT equ 4
SIZE_OF_XMM equ 16
SIZE_OF_YMM equ 32
SIZE_OF_ZMM equ 64

;These not the exact values but the address of the location
;where these are stored in stack
TSAMPS_SIZE equ 0
TSTRIDE_SIZE equ 8
NBLOCKS_PER_CHANNEL equ 16
NCHANS equ 24
DATA_SIZE equ 32
SHIFTS_SIZE equ 40
START_SHIFTS_SIZE equ 48
START_SHIFTS_LOCATION equ 56
CURRENT_SHIFTS_LOCATION equ 64
NSAMPS_SIZE equ 72
DSAMPS equ 80
NSAMPS equ 88
NDMS equ 96
FLAG equ 104
TOTAL_SIZE equ 112
RDX_REG equ 120
R8_REG equ 128

; actual data location
DATA_LOCATION equ 256

GLOBAL nasm_dedisperse_klotski_bruteforce
nasm_dedisperse_klotski_bruteforce:

        push rbx
        push rsi
        push rdi
        push r12
        push r13
        push r14
        push r15
        sub rsp, r9 ; Allocate memory in the stack

        mov r14, qword [r8+TSAMPS_SIZE]
        mov qword [rsp+TSAMPS_SIZE], r14

        mov r14, qword [r8+TSTRIDE_SIZE]
        mov qword [rsp+TSTRIDE_SIZE], r14

        mov r14, qword [r8+NBLOCKS_PER_CHANNEL]
        mov qword [rsp+NBLOCKS_PER_CHANNEL], r14

        mov r14, qword [r8+NCHANS]
        mov qword [rsp+NCHANS], r14

        mov r14, qword [r8+DATA_SIZE]
        mov qword [rsp+DATA_SIZE], r14

        mov r14, qword [r8+SHIFTS_SIZE]
        mov qword [rsp+SHIFTS_SIZE], r14

        mov r14, qword [r8+START_SHIFTS_SIZE]
        mov qword [rsp+START_SHIFTS_SIZE], r14

        mov r14, qword [r8+START_SHIFTS_LOCATION]
        mov qword [rsp+START_SHIFTS_LOCATION], r14

        mov r14, qword [r8+CURRENT_SHIFTS_LOCATION]
        mov qword [rsp+CURRENT_SHIFTS_LOCATION], r14

        mov r14, qword [r8+NSAMPS_SIZE]
        mov qword [rsp+NSAMPS_SIZE], r14

        mov r14, qword [r8+DSAMPS]
        mov qword [rsp+DSAMPS], r14

        mov r14, qword [r8+NSAMPS]
        mov qword [rsp+NSAMPS], r14

        mov r14, qword [r8+NDMS]
        mov qword [rsp+NDMS], r14

        mov r14, qword [r8+FLAG]
        mov qword [rsp+FLAG], r14

        mov r14, qword [r8+TOTAL_SIZE]
        mov qword [rsp+TOTAL_SIZE], r14

        mov qword [rsp+RDX_REG], rdx

        mov qword [rsp+R8_REG], r8


;----------------------------------------------------------------------
;Copy start shifts per DM
        xor r10,r10 ; r10 = 0 (setting the iterator to 0)
c_loop0:
        mov rax, SIZE_OF_FLOAT ; rax = 4
        mul r10 ; rax = r10*4
        mov r15d, dword [rcx+rax] ; copy the shifts to ymm15
        mov r14, qword [rsp+START_SHIFTS_LOCATION]
        add r14, rax
        mov dword [rsp+r14], r15d ; move ymm15 to stack
        inc r10 ; r10++
        mov r14, qword [rsp+NCHANS]
        cmp r10, r14 ; mak sure r10 doesnot 1024 -> NCHANS*4/32
        jl c_loop0
;------------------------------------------------------------------------

; Moving FrequencyTime data to the stack
        xor r8, r8 ; set channel = 0
cloop:  mov r14, qword [rsp+TSAMPS_SIZE]
        mov rax, r14 ; rax = TSAMPS_SIZE (total samples int FT block)
        mul r8 ; rax = r8*rax ~ rax = channel*TSAMPS_SIZE
        mov r9, rax ; saving the value in r9 = rax = channel*TSAMPS_SIZE
        xor r10,r10 ; set sample = 0
sloop:
        mov rax, SIZE_OF_ZMM ; set rax=SIZE_OF_ZMM
        mul r10 ; rax = SIZE_OF_ZMM*sample
        add rax, r9 ; rax = channel*TSAMPS_SIZE + SIZE_OF_ZMM*sample
;---------dedisperse before copy----------------------------------------------
        mov rbx, rax
        mov rax, SIZE_OF_FLOAT
        mul r8
        mov r14, qword [rsp+START_SHIFTS_LOCATION]
        add r14, rax
        mov ecx, dword [rsp+r14]
        mov rax, SIZE_OF_SHORT
        mul rcx
        add rax, rbx
;------------------------------------------------------------------------------
        vmovdqu16 zmm15, zword [rdi+rax] ; copy the value data_in[channel*TSAMPS + 32*sample]
        mov r14, qword [rsp+TSTRIDE_SIZE]
        mov rax, r14 ; setting rax= TSTRIDE
        mul r8 ; rax = TSTRIDE*r8 = TSTRIDE*channel
        mov r15, rax ; r15 = rax = TSTRIDE*channel
        mov rax, SIZE_OF_ZMM ; rax = SIZE_OF_ZMM
        mul r10 ; rax = SIZE_OF_ZMM*sample
        add rax, r15 ; rax = SIZE_OF_ZMM*sample + TSTRIDE*channel
        vmovdqu16 zword [rsp+rax+DATA_LOCATION], zmm15 ; copy data_in[channel*TSAMPS + SIZE_OF_ZMM*sample] to stack
        inc r10 ; sample++
        mov r14, qword [rsp+NBLOCKS_PER_CHANNEL]
        cmp r10, r14; come out of the loop when r10 reached to NBLOCKS_PER_CHANNEL
        jne sloop
        inc r8 ; channel++
        mov r14, qword [rsp+NCHANS]
        cmp r8, r14 ; break when channel==NCHANS
        jne cloop


;----------------------------------------------------------------------
;Copy shifts per DM
        xor r10,r10 ; r10 = 0 (setting the iterator to 0)
loop0:
        mov rax, SIZE_OF_FLOAT ; rax = SIZE_OF_XMM
        mul r10 ; rax = r10*SIZE_OF_XMM
        mov r15d, dword [rsi+rax] ; copy the shifts to xmm15
        mov r14, qword [rsp+DATA_SIZE]
        add rax, r14 ; rax = rax+DATA_SIZE
        mov dword [rsp+rax+DATA_LOCATION], r15d ; move xmm15 to stack
        inc r10 ; r10++
        mov r14, qword [rsp+NCHANS]
        cmp r10, r14 ; mak sure r10 doesnot 1024 -> NCHANS*4/32
        jl loop0
;;-----------------------------------------------------------------------

        xor r11, r11 ; r11 16byte stride in the data_in vector stride int
        xor r12, r12
loop1:
;  Dedispersion
        xor r15, r15 ; r15=0 ~ dm_indx=0;
loop2:
        ;estimate the shifts for each DM index
        xor r10,r10 ; read the dm_shifts from the stack in blocks of 8 (32 bytes)
loop3:
        mov rax, SIZE_OF_XMM ; rax = SIZE_OF_XMM (stride for for each block)
        mul r10 ;rax = SIZE_OF_XMM*r10 (multiply number for each block)
        mov r14, qword [rsp+DATA_SIZE]
        add rax, r14 ; rax = rax+DATA_SIZE (location of dm_shifts/dm)
        vxorpd ymm0, ymm0, ymm0;
        movdqu xmm0, oword [rsp+rax+DATA_LOCATION] ; copy the a block of DM sifts to ymm0
        movd xmm1, r15d ; xmm1 = r15d (last 32 bits of xmm is set to r15d)
        vpbroadcastd ymm1, xmm1 ; broadcast the lower 32 bits to all the elements in the vector
        vcvtdq2ps ymm1, ymm1 ; (float)(dm_indx) converting int to float
        vmulps ymm2, ymm1, ymm0 ; dm_indx*shift multiplying shifts with the dm_index
        vcvttps2dq ymm2, ymm2 ; (int)(dm_indx) truncate
        mov r14, qword [rsp+CURRENT_SHIFTS_LOCATION]
        mov rax, SIZE_OF_XMM
        mul r10
        add r14, rax
        movupd oword [rsp+r14], xmm2 ; move the shifts to CURRENT_location
        inc r10 ; r10++
        mov r14, qword [rsp+NCHANS]
        mov rax, SIZE_OF_FLOAT
        mul r10
        cmp rax, r14 ; total blocks =32 coressponds to 32 channels
        jl loop3

;------------------------------------------------------------------------------
        xor rbx, rbx ; channel=0
        vxorpd zmm0, zmm0, zmm0; setting ymm7=0
        vxorpd zmm1, zmm1, zmm1; setting ymm9=0
loop4:
        mov r14, qword [rsp+CURRENT_SHIFTS_LOCATION]
        mov r8, r14 ; r8 = CURRENT_SHIFTS_LOCATION
        mov rax, SIZE_OF_FLOAT; rax = 4
        mul rbx; rax = 4*rbx ~ 4*channel
        add r8, rax ; r8 = 4*channel + CURRENT_SHIFTS_LOCATION
        mov r8d, [rsp+r8] ; r8d = shift[channel]
        mov rax, SIZE_OF_YMM ; rax = 32
        mul r12 ; rax = 32*sample
        add r8, rax ; 32*sample + shift[channel]
        mov rax, SIZE_OF_SHORT
        mul r8
        mov r8, rax
        mov r14, qword [rsp+NSAMPS_SIZE]
        cmp r8, r14 ; check if r8<NSAMPS_SIZE
        jl continue ;
        sub r8, r14 ; if r8>NSAMPS_SIZE r8 = r8-NSAMPS_SIZE

continue:
        mov r14, qword [rsp+TSTRIDE_SIZE]
        mov rax, r14; rax=TSTRIDE_SIZE
        mul rbx ; TSTRIDE_SIZE*channel
        add rax, r8; rax = TSTRIDE_SIZE*channel+ sample_size(shift[channel] + SIZE_OF_YMM*sample)
        ; recheck the logic
        vmovdqu ymm6, yword [rsp+rax+DATA_LOCATION] ; copy 16 bytes from the data location
        vmovdqu ymm7, yword [rsp+rax+SIZE_OF_YMM+DATA_LOCATION] ; copy 16 bytes from the data location

        vpmovzxwd zmm6, ymm6 ; convert uint16_t to uint_t
        vpmovzxwd zmm7, ymm7 ; convert uint16_t to uint_t

        vpaddd zmm0, zmm0, zmm6 ; zmm0 = zmm0 + zmm6
        vpaddd zmm1, zmm1, zmm7 ; zmm1 = zmm1 + zmm7


        inc rbx
        mov r14, qword [rsp+NCHANS]
        cmp rbx, r14
        jl loop4 ; end the loop on channels

        mov r14, qword [rsp+DSAMPS]
        mov rax, r14
        mul r15 ; rax = dm_indx*DSAMPS
        mov r8, SIZE_OF_FLOAT
        mul r8
        mov r8, rax
        mov rax, 128 ;(32*4(float))
        mul r11

        add rax, r8
        mov r14, qword [rsp+FLAG]
        cmp r14, 0
        je do_not_replace
;---------klotski replace code----------------------------------------------
        mov r14, qword [rsp+RDX_REG]
        vmovdqu16 zmm2, zword [r14+rax]
        vpaddd zmm0, zmm0, zmm2
        vmovdqu16 zword [r14+rax], zmm0 ; move sum to the destination data_out[dm_indx*DSAMPS+sample]
        add rax, SIZE_OF_ZMM
        vmovdqu16 zmm2, zword [r14+rax]
        vpaddd zmm1, zmm1, zmm2
        vmovdqu16 zword [r14+rax], zmm1
        jmp done_replace
do_not_replace:
        mov r14, qword [rsp+RDX_REG]
        vmovdqu16 zword [r14+rax], zmm0 ; move sum to the destination data_out[dm_indx*DSAMPS+sample]
        add rax, SIZE_OF_ZMM
        vmovdqu16 zword [r14+rax], zmm1
done_replace:
        inc r15
        mov r14, qword [rsp+NDMS]
        cmp r15, r14
        jl loop2 ; end the loop over dm index


       ; replace the data in the location with the next chuck
        xor rbx, rbx ; channel=0
        vxorpd zmm15, zmm15, zmm15; setting ymm15=0
loop5:
        mov r14, qword [rsp+TSAMPS_SIZE]
        mov rax, r14
        mul rbx
        mov r13, rax
        mov rax, SIZE_OF_ZMM
        mul r11
        add rax, r13
;----------dedisperse before copy-----------------------------------
        mov r15, rax
        mov rax, SIZE_OF_FLOAT
        mul rbx
        mov r14, qword [rsp+START_SHIFTS_LOCATION]
        add r14, rax
        mov ecx, dword [rsp+r14]
        mov rax, SIZE_OF_SHORT
        mul rcx
        add rax, r15
;-------------------------------------------------------------------
        mov r14, qword [rsp+NSAMPS_SIZE]
        add rax, r14
        vmovdqu16 zmm15, zword [rdi+rax]

        mov r14, qword [rsp+TSTRIDE_SIZE]
        mov rax, r14
        mul rbx
        mov r13, rax
        mov rax, SIZE_OF_ZMM
        mul r12
        add rax, r13
        vmovdqu16 zword [rsp+rax+DATA_LOCATION], zmm15

        cmp r12, 0
        jne continue1
        mov r14, qword [rsp+NSAMPS_SIZE]
        add rax, r14
        vmovdqu16 zword [rsp+rax+DATA_LOCATION], zmm15
continue1:
        inc rbx
        mov r14, qword [rsp+NCHANS]
        cmp rbx, r14
        jl loop5 ; loop over channels

        inc r12
        inc r11

        mov rax, SIZE_OF_YMM
        mul r11

        mov r14, qword [rsp+DSAMPS]
        cmp rax, r14
        jge done ; finish if the number of channels dsamps

        mov rax, SIZE_OF_YMM
        mul r12
        mov r14, qword [rsp+NSAMPS]
        cmp rax, r14
        jl loop1 ; finish if the number of samps are reached

        mov r12, 0
        jmp loop1 ; ser r12=0 if r12 = nsamps (makking a ring buffer)



done:
        mov r14, qword [rsp+TOTAL_SIZE]
        add rsp, r14
        ;clean up
        pop r15
        pop r14
        pop r13
        pop r12
        pop rdi
        pop rsi
        pop rbx
        ret

print:
        mov r8, qword [rsp+R8_REG]
        mov r14, qword [rsp+NCHANS]
        mov qword [r8], r14
        jmp done
