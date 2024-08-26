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

SIZE_OF_YMM equ 32
SIZE_OF_ZMM equ 64

GLOBAL nasm_downsample_klotski_bruteforce
nasm_downsample_klotski_bruteforce:

        ;rdi: DataIn
        ;rsi: number_of_elements

        xor r10, r10 ; set element = 0
element_loop:
        mov rax, SIZE_OF_ZMM
        mul r10
        vmovdqu ymm1, yword [rdi+rax]
        vmovdqu ymm2, yword [rdi+rax+32]
        vphaddw ymm3, ymm1, ymm2
        vpermq ymm3, ymm3, 0xd8
        vpsraw ymm3, ymm3, 0x01
        mov rax, SIZE_OF_YMM
        mul r10
        vmovdqu yword [rdi+rax], ymm3
        inc r10
        mov rax, SIZE_OF_YMM
        mul r10
        cmp rax, rsi
        jl element_loop
        ret